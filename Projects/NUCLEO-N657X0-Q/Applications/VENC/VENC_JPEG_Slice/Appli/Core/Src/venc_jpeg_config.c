/**
 ******************************************************************************
 * @file    Appli/Core/Src/venc_jpeg_config.c
 * @author  MCD Application Team / Antigravity
 * @brief   App plugin instances and VENC/JPEG slice configuration
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

#include "utils.h"
#include "stm32n6xx_hal.h"
#include "venc_jpeg_config.h"

#ifndef DCMIPP_FORMAT
#define DCMIPP_FORMAT            DCMIPP_PIXEL_PACKER_FORMAT_YUV420_2
#endif

#ifndef VENC_INPUT_FORMAT
#define VENC_INPUT_FORMAT        JPEGENC_YUV420_SEMIPLANAR
#endif

#ifndef DCMIPP_BYTES_PER_PIXELS
#define DCMIPP_BYTES_PER_PIXELS  (1.5f)  /* YUV420 : 12 bits per pixel */
#endif

/* Pitch depends on chosen pixel packer format */
#if DCMIPP_FORMAT == DCMIPP_PIXEL_PACKER_FORMAT_YUV422_1
#define DCMIPP_PITCH             (2U * VENC_WIDTH)
#elif DCMIPP_FORMAT == DCMIPP_PIXEL_PACKER_FORMAT_YUV420_2
#define DCMIPP_PITCH             (VENC_WIDTH)
#elif DCMIPP_FORMAT == DCMIPP_PIXEL_PACKER_FORMAT_YUV420_3
#define DCMIPP_PITCH             (VENC_WIDTH)
#else
#error "DCMIPP_FORMAT is not supported"
#endif

#define CAM_MACROBLOCK_HEIGHT    (16U)

/* HW handshake mode aka SLICE */
#if (VENC_HW_MODE_ENABLE == 1U)
#define CAM_NB_BUFFERS           (2U)
#ifndef VENC_LINE_BUF_DEPTH
#define VENC_LINE_BUF_DEPTH      (2U) /* 2 lines of 16-pixel macroblocks = 32 lines */
#endif
#define INPUT_FRAME_SIZE         ((uint32_t)((VENC_WIDTH * CAM_MACROBLOCK_HEIGHT * CAM_NB_BUFFERS * VENC_LINE_BUF_DEPTH * 3U) / 2U))
#define NB_INPUT_FRAME           (1U)
#else /* Frame mode */
#define CAM_NB_BUFFERS           (1U)
#define VENC_LINE_BUF_DEPTH      (1U)
#define INPUT_FRAME_SIZE         ((uint32_t)((VENC_WIDTH * VENC_HEIGHT * 3U) / 2U))
#define NB_INPUT_FRAME           (2U)
#endif

/* Pool size for EWL wrapper */
#ifndef VENC_POOL_SIZE
#define VENC_POOL_SIZE           (64U * 1024U)
#endif

/* Output bitstream ring buffer size */
#ifndef VENC_OUTPUT_BUFFER_SIZE
#define VENC_OUTPUT_BUFFER_SIZE  (4U * 100U * 1024U)
#endif

#ifndef VENC_JPEG_QLEVEL
#define VENC_JPEG_QLEVEL         (7U)
#endif

/* Input circular line buffer (camera capture) */
uint8_t input_frame[NB_INPUT_FRAME][INPUT_FRAME_SIZE] ALIGN_32 INPUT_FRAME_LOCATION;

/* Encoder working pool (EWL wrapper) */
uint8_t ewl_pool[VENC_POOL_SIZE] ALIGN_32 VENC_BUFFER_LOCATION;

/* JPEG bitstream (encoded output buffer) */
uint8_t jpeg_bitstream[VENC_OUTPUT_BUFFER_SIZE] ALIGN_32 __NON_CACHEABLE;

/* Camera sensor native resolution */
cam_jpeg_cfg_t hCamJpegInstance = {
  .width  = 2592,
  .height = 1944,
};

/* DCMIPP configuration instance */
dcmipp_jpeg_cfg_t hDcmippJpegInstance = {
  .bytes_per_pixel = DCMIPP_BYTES_PER_PIXELS,
  .format          = DCMIPP_FORMAT,
  .pitch           = DCMIPP_PITCH,
};

/* VENC JPEG configuration instance */
venc_jpeg_cfg_t hVencJpegInstance = {
  .cfgJpeg = {
    .inputWidth             = VENC_WIDTH,
    .inputHeight            = VENC_HEIGHT,
    .xOffset                = 0,
    .yOffset                = 0,
    .codingWidth            = VENC_WIDTH,
    .codingHeight           = VENC_HEIGHT,
    .restartInterval        = 0,
    .qLevel                 = VENC_JPEG_QLEVEL,
    .qTableLuma             = NULL,
    .qTableChroma           = NULL,
    .frameType              = VENC_INPUT_FORMAT,
    .colorConversion        = {
      .type = JPEGENC_RGBTOYUV_BT601,
    },
    .rotation               = JPEGENC_ROTATE_0,
    .codingType             = JPEGENC_WHOLE_FRAME,
    .codingMode             = JPEGENC_420_MODE,
    .unitsType              = JPEGENC_NO_UNITS,
    .markerType             = JPEGENC_SINGLE_MARKER,
    .xDensity               = 1,
    .yDensity               = 1,
    .comLength              = 0,
    .pCom                   = NULL,
    .inputLineBufEn         = VENC_HW_MODE_ENABLE,
    .inputLineBufLoopBackEn = VENC_HW_MODE_ENABLE,
    .inputLineBufDepth      = VENC_LINE_BUF_DEPTH,
    .inputLineBufHwModeEn   = VENC_HW_MODE_ENABLE,
  },
  .frameSeqNum = 0,
  .frameHeader = 1,
};

/* EWL pool callbacks and helpers -------------------------------------------*/

/**
 * @brief  Provide encoder working buffer to EWL (External Wrapper Layer).
 * @param  pool_ptr Output: pointer to EWL pool buffer pointer.
 * @param  size     Output: size of the pool buffer in bytes.
 */
void EWLPoolChoiceCb(u8 **pool_ptr, size_t *size)
{
  *pool_ptr = ewl_pool;
  *size = VENC_POOL_SIZE;
}

/**
 * @brief  Release EWL pool pointer.
 * @param  pool_ptr Pointer previously returned by EWLPoolChoiceCb (unused).
 */
void EWLPoolReleaseCb(u8 **pool_ptr)
{
  UNUSED(pool_ptr);
}

/**
 * @brief  Get pointer to input frame buffer and optionally its size.
 * @param  frameSize Optional output: size of input frame in bytes.
 * @retval uint8_t* Pointer to input frame buffer.
 */
uint8_t * GetInputFrame(uint32_t * frameSize)
{
  if (frameSize)
  {
    *frameSize = INPUT_FRAME_SIZE;
  }
  return input_frame[0];
}

/**
 * @brief  Get pointer to output buffer and its size.
 * @param  bufferSize Optional output: size of buffer in bytes.
 * @retval uint8_t* Pointer to the buffer.
 */
uint8_t * GetOutputBuffer(uint32_t * bufferSize)
{
  if (bufferSize)
  {
    *bufferSize = VENC_OUTPUT_BUFFER_SIZE;
  }
  return jpeg_bitstream;
}

/**
 * @brief  Get pointer to next input frame buffer for camera capture.
 * @param  frame_number Captured frame sequence number.
 * @retval uint8_t* Pointer to frame buffer.
 */
uint8_t * GetNextFrame(uint32_t frame_number)
{
  return input_frame[frame_number % NB_INPUT_FRAME];
}

/**
 * @brief  Get number of input frame buffers.
 * @retval uint32_t Buffer count.
 */
uint32_t GetNbInputFrame(void)
{
  return NB_INPUT_FRAME;
}

/**
 * @brief  Return whether hardware handshake (slice/stream) mode is enabled.
 * @retval true if HW handshake mode enabled, false otherwise.
 */
bool IsHwHanshakeMode(void)
{
#if (VENC_HW_MODE_ENABLE == 1U)
  return true;
#else
  return false;
#endif
}

/**
 * @brief  Return configured DCMIPP pixel packer format identifier.
 * @retval uint32_t DCMIPP format constant.
 */
uint32_t GetDCMIPPFormat(void)
{
  return (uint32_t)DCMIPP_FORMAT;
}

/**
 * @brief  Retrieve DCMIPP lines configuration (wrap and irq lines) for HW handshake mode.
 * @param  pWarpLines Output pointer to wrap lines constant (may be NULL).
 * @param  pIrqLines  Output pointer to irq lines constant (may be NULL).
 * @retval HAL_StatusTypeDef HAL_OK when valid configuration is returned, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef GetDCMIPPLinesConfig(uint32_t *pWarpLines, uint32_t *pIrqLines)
{
  uint32_t nbLinesCaptured = VENC_LINE_BUF_DEPTH * CAM_MACROBLOCK_HEIGHT;
  HAL_StatusTypeDef ret = HAL_OK;

  if (!IsHwHanshakeMode())
  {
    return HAL_ERROR;
  }

  /* In hardware handshake mode, capture N lines within a 2*N buffer */
  switch (nbLinesCaptured)
  {
    case 16U:
      if (pWarpLines) *pWarpLines = DCMIPP_WRAP_ADDRESS_32_LINES;
      if (pIrqLines)  *pIrqLines  = DCMIPP_MULTILINE_16_LINES;
      break;

    case 32U:
      if (pWarpLines) *pWarpLines = DCMIPP_WRAP_ADDRESS_64_LINES;
      if (pIrqLines)  *pIrqLines  = DCMIPP_MULTILINE_32_LINES;
      break;

    case 64U:
      if (pWarpLines) *pWarpLines = DCMIPP_WRAP_ADDRESS_128_LINES;
      if (pIrqLines)  *pIrqLines  = DCMIPP_MULTILINE_64_LINES;
      break;

    default:
      ret = HAL_ERROR;
      break;
  }

  return ret;
}

/**
 * @brief  Return number of lines captured by DCMIPP (used for buffer addressing).
 * @retval uint32_t Number of lines captured.
 */
uint32_t GetDCMIPPNbLinesCaptured(void)
{
  if (IsHwHanshakeMode())
  {
    return (uint32_t)(2U * VENC_LINE_BUF_DEPTH * CAM_MACROBLOCK_HEIGHT);
  }
  else
  {
    return (uint32_t)hVencJpegInstance.cfgJpeg.codingHeight;
  }
}

/**
 * @brief Returns the video frame rate (FPS) from the configuration.
 * @retval uint32_t The computed frames-per-second value.
 */
uint32_t GetVideoFramerate(void)
{
  return FRAMERATE;
}
