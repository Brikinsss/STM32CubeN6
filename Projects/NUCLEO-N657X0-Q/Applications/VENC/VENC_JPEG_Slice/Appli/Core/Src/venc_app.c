/**
 ******************************************************************************
 * @file    Appli/Core/Src/venc_app.c
 * @author  MCD Application Team / Antigravity
 * @brief   VENC USB JPEG application for STM32N6xx: handles JPEG slice encoding and USB video streaming.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may obtain a copy of the License in the LICENSE file
 * in the root directory of this software component.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "ewl.h"
#include "jpegencapi.h"
#include "venc_app.h"
#include "imx335.h"
#include "stm32n6xx_ll_venc.h"
#include "stm32n6xx_nucleo.h"
#include "stm32n6570_discovery_camera.h"
#include "tx_api.h"
#include "utils.h"
#include "venc_jpeg_config.h"
#include "dcmipp_app.h"
#include "frame_rb.h"
#include "time_monitor.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct {
  uint32_t size;
  uint32_t * block_addr;
  uint32_t * aligned_block_addr;
} venc_output_frame_t;

/* Private define ------------------------------------------------------------*/
#define VENC_APP_QUEUE_SIZE        15U
#define VENC_OUTPUT_BLOCK_NBR      2U

/* Private macro -------------------------------------------------------------*/
#define ALIGNED(ptr, bytes)       ((((uintptr_t)(ptr) + ((bytes) - 1U)) / (bytes)) * (bytes))

/* Private variables ---------------------------------------------------------*/
extern DCMIPP_HandleTypeDef hcamera_dcmipp;

static JpegEncIn encIn = {0};
static JpegEncOut encOut = {0};
static JpegEncInst encoder = {0};
static uint32_t frame_nb = 0;
uint32_t frame_received = 0;
static uint32_t last_frame_received = 0;
static uint32_t nbLineEvent = 0;
static uint32_t outputBlockSize;
static uint32_t *g_curr_block = NULL;

/* Input Frame : in internal ram */
venc_output_frame_t enc_queue_buf[VENC_APP_QUEUE_SIZE];

TX_EVENT_FLAGS_GROUP venc_app_flags;
TX_QUEUE enc_frame_queue;

uint32_t nb_encoded_frame = 0;

/* Private function prototypes -----------------------------------------------*/
static int encoder_prepare(void);
static int encode_frame(void);
static int encoder_end(void);
static int encoder_start(void);
static void encoder_reset(void);

/* Cache invalidation helper */
static void invalidate_dcache_region(const void *addr, uint32_t size)
{
  uintptr_t start;
  uintptr_t end;
  uint32_t aligned_size;

  if ((addr == NULL) || (size == 0U))
  {
    return;
  }

  start = ((uintptr_t)addr) & ~((uintptr_t)31U);
  end = ((uintptr_t)addr + (uintptr_t)size + 31U) & ~((uintptr_t)31U);
  aligned_size = (uint32_t)(end - start);
  SCB_InvalidateDCache_by_Addr((void *)start, (int32_t)aligned_size);
}

/* Cache clean helper: flushes dirty CPU cache lines to SRAM before DMA reads */
__attribute__((unused)) static void clean_dcache_region(const void *addr, uint32_t size)
{
  uintptr_t start;
  uintptr_t end;
  uint32_t aligned_size;

  if ((addr == NULL) || (size == 0U))
  {
    return;
  }

  start = ((uintptr_t)addr) & ~((uintptr_t)31U);
  end = ((uintptr_t)addr + (uintptr_t)size + 31U) & ~((uintptr_t)31U);
  aligned_size = (uint32_t)(end - start);
  SCB_CleanDCache_by_Addr((void *)start, (int32_t)aligned_size);
}

#if (defined(VENC_USE_SIMULATED_DATA) && (VENC_USE_SIMULATED_DATA == 1U))
static void fill_simulated_frame(uint8_t *pY, uint8_t *pUV, uint32_t width, uint32_t height, uint32_t frame_idx)
{
  if ((pY == NULL) || (pUV == NULL) || (width == 0U) || (height == 0U))
  {
    return;
  }

#if (VENC_SIM_PATTERN == VENC_SIM_PATTERN_COLORBAR)
  /* SMPTE 8-color bars: White, Yellow, Cyan, Green, Magenta, Red, Blue, Black */
  static const uint8_t bar_Y[8]  = { 235, 210, 170, 145, 106,  81,  41,  16 };
  static const uint8_t bar_U[8]  = { 128,  16, 166,  54, 202,  90, 240, 128 };
  static const uint8_t bar_V[8]  = { 128, 146,  16,  34, 222, 240, 110, 128 };

  uint32_t bar_width = width / 8U;
  if (bar_width == 0U) bar_width = 1U;

  /* Fill Y plane */
  for (uint32_t y = 0U; y < height; y++)
  {
    uint8_t *line_ptr = pY + y * width;
    for (uint32_t x = 0U; x < width; x++)
    {
      uint32_t bar = x / bar_width;
      if (bar > 7U) bar = 7U;
      line_ptr[x] = bar_Y[bar];
    }
  }

  /* Fill UV semi-planar (interleaved U, V, U, V...) */
  uint32_t uv_height = height / 2U;
  for (uint32_t y = 0U; y < uv_height; y++)
  {
    uint8_t *line_ptr = pUV + y * width;
    for (uint32_t x = 0U; x < width; x += 2U)
    {
      uint32_t bar = x / bar_width;
      if (bar > 7U) bar = 7U;
      line_ptr[x]     = bar_U[bar]; /* U */
      line_ptr[x + 1] = bar_V[bar]; /* V */
    }
  }

  /* Draw a moving 48x48 box across the screen to confirm live motion */
  uint32_t box_size = 48U;
  uint32_t max_x = (width > box_size) ? (width - box_size) : 0U;
  uint32_t max_y = (height > box_size) ? (height - box_size) : 0U;
  uint32_t box_x = (frame_idx * 8U) % (max_x ? max_x : 1U);
  uint32_t box_y = (frame_idx * 4U) % (max_y ? max_y : 1U);

  for (uint32_t dy = 0U; dy < box_size && (box_y + dy) < height; dy++)
  {
    uint8_t *line_ptr = pY + (box_y + dy) * width + box_x;
    for (uint32_t dx = 0U; dx < box_size && (box_x + dx) < width; dx++)
    {
      line_ptr[dx] = 240U; /* Bright white box */
    }
  }

#elif (VENC_SIM_PATTERN == VENC_SIM_PATTERN_SOLID_00)
  memset(pY, 0x00, width * height);
  memset(pUV, 0x80, width * height / 2U);

#elif (VENC_SIM_PATTERN == VENC_SIM_PATTERN_SOLID_FF)
  memset(pY, 0xFF, width * height);
  memset(pUV, 0x80, width * height / 2U);

#elif (VENC_SIM_PATTERN == VENC_SIM_PATTERN_GRADIENT)
  for (uint32_t y = 0U; y < height; y++)
  {
    uint8_t *line_ptr = pY + y * width;
    for (uint32_t x = 0U; x < width; x++)
    {
      line_ptr[x] = (uint8_t)((x * 255U) / width);
    }
  }
  memset(pUV, 0x80, width * height / 2U);

#elif (VENC_SIM_PATTERN == VENC_SIM_PATTERN_NOISE)
  for (uint32_t y = 0U; y < height; y++)
  {
    uint8_t *line_ptr = pY + y * width;
    for (uint32_t x = 0U; x < width; x++)
    {
      line_ptr[x] = ((x ^ y) & 0x08) ? 0xE0 : 0x20;
    }
  }
  memset(pUV, 0x80, width * height / 2U);
#endif
}
#endif

static void release_output_block(uint32_t *block_addr)
{
  (void)block_addr;
  frb_free(0xFFFFFFFFU);
}

/**
 * @brief  Checks if a video buffer overflow condition has occurred.
 * @retval true if overflow detected, false otherwise.
 */
bool IsVideoOverflow(void)
{
  bool videoOverflow = false;

  if (IsHwHanshakeMode())
  {
    videoOverflow = (nbLineEvent != 0U);
  }
  else
  {
    videoOverflow = (frame_received > last_frame_received + GetNbInputFrame());
    last_frame_received = frame_received;
  }
  return videoOverflow;
}

/**
 * @brief  VENC application thread function.
 * @param  arg Thread argument (unused).
 */
void venc_thread_func(ULONG arg)
{
  ULONG flags;
  uint8_t *outputBuffer;
  uint32_t outputBufferSize;

  if (tx_event_flags_create(&venc_app_flags, "venc_app_events") != TX_SUCCESS)
  {
    return;
  }

  if (tx_queue_create(&enc_frame_queue, "ENC frame queue", sizeof(venc_output_frame_t) / 4, enc_queue_buf, sizeof(enc_queue_buf)) != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Get address and size reserved for JPEG output bitstream */
  outputBuffer = GetOutputBuffer(&outputBufferSize);
  outputBlockSize = outputBufferSize / VENC_OUTPUT_BLOCK_NBR;

  if (frb_init(outputBuffer, outputBufferSize) == false)
  {
    Error_Handler();
  }

  /* Initialize camera */
  if (BSP_CAMERA_Init(0, CAMERA_R2592x1944, CAMERA_PF_RAW_RGGB10) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  IMX335_SetFramerate(Camera_CompObj, GetVideoFramerate());

  /* initialize VENC peripheral clock and reset */
  LL_VENC_Init();

  /* initialization done. Turn on the LEDs */
  BSP_LED_On(LED1);
  BSP_LED_On(LED2);

  if (encoder_prepare())
  {
    printf("JPEG Encoder init failed\n");
    Error_Handler();
  }

  {
    EWLHwConfig_t hwcfg = EWLReadAsicConfig();
    printf("VENC: HW maxEncodedWidth=%u, jpegEnabled=%u, busWidth=%u\n",
           (unsigned int)hwcfg.maxEncodedWidth, (unsigned int)hwcfg.jpegEnabled, (unsigned int)hwcfg.busWidth);
  }

  printf("VENC: venc_thread waiting for VIDEO_START_FLAG (JPEG %lux%lu %s)...\n",
         (unsigned long)hVencJpegInstance.cfgJpeg.codingWidth,
         (unsigned long)hVencJpegInstance.cfgJpeg.codingHeight,
         IsHwHanshakeMode() ? "Slice Mode" : "Frame Mode");

#if (defined(VENC_USE_SIMULATED_DATA) && (VENC_USE_SIMULATED_DATA == 1U))
  printf("VENC: [TEST MODE] Simulated data injection ENABLED (pattern=%u)\n", (unsigned int)VENC_SIM_PATTERN);
#endif

  while (1)
  {
    tx_event_flags_get(&venc_app_flags, VIDEO_START_FLAG, TX_AND, &flags, TX_WAIT_FOREVER);
    if (BSP_CAMERA_BackgroundProcess() != BSP_ERROR_NONE)
    {
      printf("Error in BSP image processing\n");
    }

    tx_event_flags_get(&venc_app_flags, FRAME_RECEIVED_FLAG, TX_AND_CLEAR, &flags, TX_WAIT_FOREVER);
    if (IsVideoOverflow())
    {
      printf("Video Overflow - Skip Frame\n");
      continue;
    }

    if (encode_frame())
    {
      printf("error encoding frame\n");
    }
    else
    {
      BSP_LED_Toggle(LED_GREEN);
    }
  }
}

/**
 * @brief  Prepare JPEG encoder: initialize encoder instance with configuration parameters.
 * @retval int 0 on success, -1 on failure
 */
static int encoder_prepare(void)
{
  JpegEncRet ret;

  frame_nb = 0U;

  ret = JpegEncInit(&hVencJpegInstance.cfgJpeg, &encoder);
  if (ret != JPEGENC_OK)
  {
    printf("JpegEncInit failed: %d\n", ret);
    return -1;
  }

  ret = JpegEncSetPictureSize(encoder, &hVencJpegInstance.cfgJpeg);
  if (ret != JPEGENC_OK)
  {
    printf("JpegEncSetPictureSize failed: %d\n", ret);
    return -1;
  }

  return 0;
}

/**
 * @brief Resets the encoder and DCMIPP pipe to recover from desync or errors.
 */
static void encoder_reset(void)
{
  if (HAL_DCMIPP_PIPE_Suspend(&hcamera_dcmipp, DCMIPP_PIPE1) != HAL_OK)
  {
    printf("HAL_DCMIPP_PIPE_Suspend failed\n");
  }

  /* Wait for end of current frame transfer */
  tx_thread_sleep(15U * TX_TIMER_TICKS_PER_SECOND / 1000U);

  /* VENC HW Reset */
  __HAL_RCC_VENC_FORCE_RESET();
  tx_thread_sleep(1U * TX_TIMER_TICKS_PER_SECOND / 1000U);
  __HAL_RCC_VENC_RELEASE_RESET();
  tx_thread_sleep(1U * TX_TIMER_TICKS_PER_SECOND / 1000U);
  LL_VENC_Init();

  /* Release previous encoder instance and re-initialize hardware registers */
  if (encoder != NULL)
  {
    JpegEncRelease(encoder);
    encoder = NULL;
  }
  if (encoder_prepare() != 0)
  {
    printf("encoder_prepare failed in encoder_reset\n");
  }

  /* Resume DCMIPP after VENC reset */
  if (HAL_DCMIPP_PIPE_Resume(&hcamera_dcmipp, DCMIPP_PIPE1) != HAL_OK)
  {
    printf("HAL_DCMIPP_PIPE_Resume failed\n");
  }
}

/**
 * @brief Starts the video capture and encoding pipeline.
 * @return int 0 on success, negative on error.
 */
static int encoder_start(void)
{
  if (dcmipp_config(GetInputFrame(NULL)))
  {
    return -1;
  }
  return 0;
}

/* Debug and instrumentation functions */
__weak void mark_frame(void * frame) { (void)frame; }
__weak void timeMonitor(void) {}
__weak void timeMonitorStart(void) {}
__weak void timeMonitorStop(void) {}

/**
 * @brief Encode a single captured frame to JPEG and queue for USB transmission.
 * @return 0 on success, -1 on failure.
 */
static int encode_frame(void)
{
  venc_output_frame_t frame_buffer = {0};
  JpegEncRet ret;
  uint32_t outBufSize;
  uint32_t buff_size;
  DCMIPP_FullPlanarDstAddressTypeDef planar_address;
  DCMIPP_SemiPlanarDstAddressTypeDef semi_planar_address;

  /* set input buffers from DCMIPP circular buffer */
  dcmipp_get_address((void *)GetNextFrame(nb_encoded_frame), &planar_address, &semi_planar_address);

  if (dcmipp_is_semiplanar())
  {
    encIn.busLum = semi_planar_address.YAddress;
    encIn.busCb  = semi_planar_address.UVAddress;
    encIn.busCr  = semi_planar_address.UVAddress;
    encIn.pLum   = (const u8 *)semi_planar_address.YAddress;
    encIn.pCb    = (const u8 *)semi_planar_address.UVAddress;
    encIn.pCr    = (const u8 *)semi_planar_address.UVAddress;
  }
  else
  {
    encIn.busLum = planar_address.YAddress;
    encIn.busCb  = planar_address.UAddress;
    encIn.busCr  = planar_address.VAddress;
    encIn.pLum   = (const u8 *)planar_address.YAddress;
    encIn.pCb    = (const u8 *)planar_address.UAddress;
    encIn.pCr    = (const u8 *)planar_address.VAddress;
  }

#if (defined(VENC_USE_SIMULATED_DATA) && (VENC_USE_SIMULATED_DATA == 1U))
  fill_simulated_frame((uint8_t *)encIn.busLum, (uint8_t *)encIn.busCb,
                       hVencJpegInstance.cfgJpeg.codingWidth,
                       hVencJpegInstance.cfgJpeg.codingHeight,
                       nb_encoded_frame);
  clean_dcache_region((const void *)encIn.busLum,
                      hVencJpegInstance.cfgJpeg.codingWidth * hVencJpegInstance.cfgJpeg.codingHeight);
  clean_dcache_region((const void *)encIn.busCb,
                      hVencJpegInstance.cfgJpeg.codingWidth * hVencJpegInstance.cfgJpeg.codingHeight / 2U);
#endif

  // /* Print first 16 bytes of DCMIPP processed Y and UV data */
  // if (nb_encoded_frame < 5U || (nb_encoded_frame % 60U) == 0U)
  // {
  //   const uint8_t *pY = (const uint8_t *)encIn.busLum;
  //   const uint8_t *pUV = (const uint8_t *)encIn.busCb;

  //   invalidate_dcache_region(pY, 16U);
  //   invalidate_dcache_region(pUV, 16U);

  //   printf("DCMIPP Frame #%lu [Y@0x%08lX]:", (unsigned long)nb_encoded_frame, (unsigned long)pY);
  //   for (uint32_t i = 0U; i < 16U; i++)
  //   {
  //     printf(" %02X", pY[i]);
  //   }
  //   printf("\n");

  //   printf("DCMIPP Frame #%lu [UV@0x%08lX]:", (unsigned long)nb_encoded_frame, (unsigned long)pUV);
  //   for (uint32_t i = 0U; i < 16U; i++)
  //   {
  //     printf(" %02X", pUV[i]);
  //   }
  //   printf("\n");
  // }

  encIn.frameHeader = 1; /* Complete JFIF/JPEG headers for every frame */
  encIn.lineBufWrCnt = 0;

  mark_frame((void *)encIn.busLum);

  /* Allocate output block from ring buffer */
  buff_size = outputBlockSize;
  frame_buffer.block_addr = (uint32_t *)frb_alloc(&buff_size);
  if (frame_buffer.block_addr == NULL)
  {
    printf("VENC: failed to allocate output buffer (req=%lu)\n", (unsigned long)outputBlockSize);
    return -1;
  }

  frame_buffer.aligned_block_addr = (uint32_t *)ALIGNED((uint32_t)frame_buffer.block_addr, 8);
  outBufSize = (buff_size > 8U) ? (buff_size - 8U) : 0U;

  encIn.pOutBuf    = (u8 *)frame_buffer.aligned_block_addr;
  encIn.busOutBuf  = (size_t)encIn.pOutBuf;
  encIn.outBufSize = outBufSize;

  /* Trigger Hardware JPEG Encode */
  ret = JpegEncEncode(encoder, &encIn, &encOut, NULL, NULL);

  timeMonitor();

  switch (ret)
  {
    case JPEGENC_FRAME_READY:
    case JPEGENC_OK:
      if (encOut.jfifSize == 0)
      {
        release_output_block(frame_buffer.block_addr);
        return -1;
      }
      invalidate_dcache_region((const void *)encIn.pOutBuf, encOut.jfifSize);
      frame_buffer.size = encOut.jfifSize;

      if (frb_push(frame_buffer.block_addr, frame_buffer.size, 0U) == false)
      {
        release_output_block(frame_buffer.block_addr);
        return -1;
      }
      if (tx_queue_send(&enc_frame_queue, (void *)&frame_buffer, TX_WAIT_FOREVER) != TX_SUCCESS)
      {
        return -1;
      }
      nb_encoded_frame++;
      if (nb_encoded_frame <= 5U || (nb_encoded_frame % 60U) == 0U)
      {
        const uint8_t *pJpeg = (const uint8_t *)encIn.pOutBuf;
        printf("VENC: encoded JPEG frame #%lu, size=%lu bytes [16B Header]:",
               (unsigned long)nb_encoded_frame, (unsigned long)frame_buffer.size);
        for (uint32_t i = 0U; i < 16U && i < frame_buffer.size; i++)
        {
          printf(" %02X", pJpeg[i]);
        }
        printf("\n");
      }
      break;

    case JPEGENC_HW_TIMEOUT:
    case JPEGENC_HW_RESET:
    case JPEGENC_HW_BUS_ERROR:
    case JPEGENC_SYSTEM_ERROR:
      printf("DCMIPP and VENC desync (frame#%lu, ret=%d, asic_status=0x%08lx), restart\n",
             frame_nb, ret, (unsigned long)LL_VENC_ReadRegister(1UL));
      release_output_block(frame_buffer.block_addr);
      encoder_reset();
      break;

    default:
      printf("error encoding JPEG frame %d\n", ret);
      release_output_block(frame_buffer.block_addr);
      return -1;
  }

  frame_nb++;
  return 0;
}

/**
 * @brief  End encoding session: stop camera and flush resources.
 * @retval int 0 on success, -1 on failure
 */
static int encoder_end(void)
{
  BSP_CAMERA_Stop(0);
  tx_queue_flush(&enc_frame_queue);
  frb_reset();
  g_curr_block = NULL;
  return 0;
}

/**
 * @brief Callback when a full camera frame is captured.
 */
void BSP_CAMERA_FrameEventCallback(uint32_t instance)
{
  if (instance == DCMIPP_PIPE1)
  {
    nbLineEvent = 0;
    frame_received++;
    if (frame_received == 1 || (frame_received % 60) == 0)
    {
      printf("CAM: FrameEventCallback frame #%lu\n", frame_received);
    }
    if (TX_SUCCESS != tx_event_flags_set(&venc_app_flags, FRAME_RECEIVED_FLAG, TX_OR))
    {
      Error_Handler();
    }

    /* Signal DCMIPP for next frame address */
    dcmipp_set_memory_address(GetNextFrame(frame_received));
  }
}

/**
 * @brief Callback when a camera line event occurs.
 */
void BSP_CAMERA_LineEventCallback(uint32_t instance)
{
  (void)instance;
  nbLineEvent++;
}

/**
 * @brief Starts the video encoding process.
 */
void VENC_APP_EncodingStart(void)
{
  printf("VENC: VENC_APP_EncodingStart called (JPEG)\n");
  frb_reset();
  tx_queue_flush(&enc_frame_queue);
  g_curr_block = NULL;
  frame_nb = 0U;
  frame_received = 0U;
  last_frame_received = 0U;
  nbLineEvent = 0U;
  nb_encoded_frame = 0U;
  timeMonitorStart();

  encoder_start();
  tx_event_flags_set(&venc_app_flags, VIDEO_START_FLAG, TX_OR);
}

/**
 * @brief Retrieves encoded video data for USB transmission.
 */
INT VENC_APP_GetData(UCHAR **data, ULONG *size)
{
  UCHAR *frb_data;
  uint32_t frb_size;
  uint32_t timeStamp = 0U;

  if (g_curr_block)
  {
    frb_return_frame();
    g_curr_block = NULL;
  }

  venc_output_frame_t frame_block;
  if (tx_queue_receive(&enc_frame_queue, (void *)&frame_block, TX_WAIT_FOREVER) != TX_SUCCESS)
  {
    *data = NULL;
    *size = 0;
    return (-1);
  }

  frb_size = 0U;
  frb_data = (UCHAR *)frb_pull(&frb_size, &timeStamp);
  if ((frb_data == NULL) || (frb_data != (UCHAR *)frame_block.block_addr) || (frb_size != frame_block.size))
  {
    *data = NULL;
    *size = 0;
    return -1;
  }

  *data = (UCHAR *)frame_block.aligned_block_addr;
  *size = frame_block.size;
  g_curr_block = frame_block.block_addr;
  return (0);
}

/**
 * @brief Peek stream-start blob (None needed for JPEG, frames are self-contained).
 */
INT VENC_APP_PeekStreamStartBlob(UCHAR **data, ULONG *size)
{
  if (data != NULL)
  {
    *data = NULL;
  }
  if (size != NULL)
  {
    *size = 0U;
  }
  return -1;
}

/**
 * @brief Clear stream-start blob.
 */
void VENC_APP_ClearStreamStartBlob(void)
{
}

/**
 * @brief Stops the video encoding process.
 */
UINT VENC_APP_EncodingStop(void)
{
  ULONG flags;
  tx_event_flags_get(&venc_app_flags, VIDEO_START_FLAG, TX_AND_CLEAR, &flags, TX_WAIT_FOREVER);
  (void)encoder_end();
  frb_reset();
  tx_queue_flush(&enc_frame_queue);
  g_curr_block = NULL;
  timeMonitorStop();
  return (0);
}
