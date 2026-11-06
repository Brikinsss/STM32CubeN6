/**
  ******************************************************************************
  * @file           : pcm_app.c
  * @brief          : Encoder support (audio PCM capture and callbacks)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "app_rtsp_over_rtp.h"
#include "utils.h"
#include "audio_conf.h"
#include "plugin_audio.h"
#include "tx_api.h"
#include <string.h>


/* Audio in buffer - placed in non-cached memory */
int16_t PCM_Buffer[CAPTURE_BUFFER_NB_SAMPLES] ALIGN_32 __NON_CACHEABLE;

/* Frame pointer and counters shared with ISR -> mark volatile */
static volatile int16_t *pcm_frame;
static volatile uint32_t pcm_frame_number;

/* Simple jitter buffer: fixed-size ring of full PCM frames */
#ifndef AUDIO_JITTER_BUFFER_FRAMES
#define AUDIO_JITTER_BUFFER_FRAMES  8U
#endif

static uint8_t s_audio_jb[AUDIO_JITTER_BUFFER_FRAMES][CAPTURE_NB_BYTES];
static volatile uint32_t jb_head = 0;  /* write index */
static volatile uint32_t jb_tail = 0;  /* read index  */
static volatile uint32_t jb_count = 0; /* number of frames in buffer */
static volatile uint32_t jb_drops = 0; /* overwritten (oldest) frames */

static inline void audio_jb_reset(void)
{
  jb_head = 0;
  jb_tail = 0;
  jb_count = 0;
  jb_drops = 0;
}

static inline void audio_jb_push(const uint8_t *src)
{
  /* If full, drop oldest to make room (overwrite policy) */
  if (jb_count == AUDIO_JITTER_BUFFER_FRAMES)
  {
    jb_tail = (jb_tail + 1U) % AUDIO_JITTER_BUFFER_FRAMES;
    jb_count--;
    jb_drops++;
  }

  memcpy(s_audio_jb[jb_head], src, CAPTURE_NB_BYTES);
  jb_head = (jb_head + 1U) % AUDIO_JITTER_BUFFER_FRAMES;
  jb_count++;
}

/* Private function prototypes -----------------------------------------------*/
void Error_Handler(void);

/* Public API ----------------------------------------------------------------*/

/**
 * @brief  Initialize audio capture hardware.
 * @note   Uses BSP audio driver for digital microphone input.
 * @retval BSP status code (BSP_ERROR_NONE on success)
 */
int32_t audio_init(void)
{
  uint32_t audioState;
  int32_t err;
  BSP_AUDIO_Init_t AudioInit;

  /* Test audio input state */
  err = BSP_AUDIO_IN_GetState(1, &audioState);
  if (err != BSP_ERROR_NONE)
  {
    printf("BSP_AUDIO_IN_GetState failed !!\n");
    return err;
  }

  if (audioState != AUDIO_IN_STATE_RESET)
  {
    printf("audioState != AUDIO_IN_STATE_RESET !!!\n");
    return BSP_ERROR_BUSY;
  }

  AudioInit.Device        = AUDIO_IN_DEVICE_DIGITAL_MIC;
  AudioInit.SampleRate    = AUDIO_FREQUENCY;
  AudioInit.BitsPerSample = AUDIO_RESOLUTION_16B;
  AudioInit.ChannelsNbr   = NB_MICS;
  AudioInit.Volume        = 80; /* Not used */

  err = BSP_AUDIO_IN_Init(1, &AudioInit);
  if (err != BSP_ERROR_NONE)
  {
    printf("BSP_AUDIO_IN_Init failed !!\n");
  }

  return err;
}

/**
 * @brief  Start audio capture (record).
 * @retval BSP status code (BSP_ERROR_NONE on success)
 */
int32_t audio_start(void)
{
  int32_t err;

  pcm_frame_number = 0;
  audio_jb_reset();

  /* Start record. Buffer length in bytes */
  err = BSP_AUDIO_IN_Record(1, (uint8_t *)PCM_Buffer, CAPTURE_BUFFER_NB_SAMPLES * sizeof(int16_t));
  if (err != BSP_ERROR_NONE)
  {
    printf("BSP_AUDIO_IN_Record failed !!\n");
  }

  return err;
}

/**
 * @brief  Stops audio capture
 * @retval BSP status code (BSP_ERROR_NONE on success)
 */
int32_t audio_stop(void)
{
  return BSP_AUDIO_IN_Stop(1);
}

/* IRQ Handler ---------------------------------------------------------------*/
/**
 * @brief  GPDMA channel IRQ handler forwards to BSP audio IRQ handler.
 */
void GPDMA1_Channel0_IRQHandler(void)
{
  BSP_AUDIO_IN_IRQHandler(1, AUDIO_IN_DEVICE_DIGITAL_MIC);
}

/* BSP audio callbacks ------------------------------------------------------*/

/**
 * @brief  BSP audio-in transfer complete callback.
 * @param  Instance  Audio instance (expected 1)
 * @retval None
 */
void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
{
  if (Instance == 1U)
  {
    /* Point to second half of the double buffer */
    pcm_frame = PCM_Buffer + (CAPTURE_BUFFER_NB_SAMPLES / 2);
    pcm_frame_number++;
    /* Copy full frame into jitter buffer */
    audio_jb_push((const uint8_t *)pcm_frame);
    tx_event_flags_set(&demo_test_events, DEMO_AUDIO_DATA_READY_EVENT, TX_OR);
  }
}

/**
 * @brief  BSP audio-in half-transfer complete callback.
 * @param  Instance  Audio instance (expected 1)
 * @retval None
 */
void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
{
  if (Instance == 1U)
  {
    /* Point to first half of the double buffer */
    pcm_frame = PCM_Buffer;
    pcm_frame_number++;
    /* Copy full frame into jitter buffer */
    audio_jb_push((const uint8_t *)pcm_frame);
    tx_event_flags_set(&demo_test_events, DEMO_AUDIO_DATA_READY_EVENT, TX_OR);
  }
}

/**
 * @brief  BSP audio-in error callback.
 * @param  Instance  Audio instance
 * @retval None
 */
void BSP_AUDIO_IN_Error_CallBack(uint32_t Instance)
{
  (void)Instance;
  printf("BSP_AUDIO_IN_Error_CallBack\n");
}

/**
 * @brief  Starts the audio encoding process.
 * @retval 0
 */
uint32_t AUDIO_APP_EncodingStart(void)
{
  audio_init();
  audio_start();
  return 0;
}

uint32_t AUDIO_APP_EncodingStop(void)
{
  return 0U;
}



/**
 * @brief  Return the last PCM frame sequence number produced by ISR.
 * @retval pcm frame sequence number
 */
uint32_t AUDIO_APP_LastPcmFrameNumber(void)
{
  return pcm_frame_number;
}

/**
 * @brief  Get pointer to the last ready PCM data and its size in bytes.
 * @param  data  Output pointer to PCM buffer (points to half-buffer region)
 * @param  size  Output size in bytes
 * @retval 0
 *
 * @note   This function checks for buffer overflow (producer/consumer mismatch).
 */
int32_t AUDIO_APP_GetData(uint8_t **data, size_t *size)
{
  *data = NULL;
  *size = 0;
  
  /* Serve oldest frame from jitter buffer if available with IRQ-safe update */
  UINT old_posture = tx_interrupt_control(TX_INT_DISABLE);
  if (jb_count > 0U)
  {
    *data = s_audio_jb[jb_tail];
    *size = CAPTURE_NB_BYTES;
    jb_tail = (jb_tail + 1U) % AUDIO_JITTER_BUFFER_FRAMES;
    jb_count--;
  }
  tx_interrupt_control(old_posture);
  
  /* Update report producer/consumer  drops */
  if (jb_drops)
  {
    printf("AUDIO jitter buffer overrun: %lu frames dropped\n", (unsigned long)jb_drops);
    jb_drops = 0;
  }
  
  return 0U;
}

/* Return number of PCM frames currently buffered in jitter ring. */




