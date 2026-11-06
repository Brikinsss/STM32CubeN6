/**
  ******************************************************************************
  * @file           : audio_conf.h
  * @brief          : Audio configuration header
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component. If no LICENSE file is
  * provided, the software is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef _AUDIO_CONF_H_
#define _AUDIO_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32n6570_discovery_audio.h"

/* Exported macros -----------------------------------------------------------*/
#define AUDIO_FREQUENCY                 AUDIO_FREQUENCY_16K
#define AUDIO_CAPTURE_IT_MS             20U
#define NB_MICS                         1U
#define CAPTURE_NB_SAMPLES              (NB_MICS * AUDIO_CAPTURE_IT_MS * AUDIO_FREQUENCY / 1000U)
#define CAPTURE_NB_BYTES                (CAPTURE_NB_SAMPLES*sizeof(int16_t)) /* 16 bits / sample */
/* Number of 16-bit samples in capture buffer (double-buffer: half + full) */
#define CAPTURE_BUFFER_NB_SAMPLES       (2U * CAPTURE_NB_SAMPLES) /* Ping Pong buffers */



#define SECTION_DMA ".noncacheable"

#if defined(__ICCARM__)
/* IAR: place variable in section and align to 4 bytes */
#define VAR_DECLARE_ALIGN4_AT_SECTION(type, var, section) \
  _Pragma("data_alignment=4") type var @ section
#elif defined(__GNUC__)
/* GNU: use section attribute and alignment */
#define VAR_DECLARE_ALIGN4_AT_SECTION(type, var, section) \
  __attribute__((__section__(section))) type var __attribute__ ((aligned(4)))
#elif defined(__CC_ARM)
/* ARMCC: use section attribute and align directive */
#define VAR_DECLARE_ALIGN4_AT_SECTION(type, var, section) \
  __attribute__((__section__(section))) __align(4) type var
#else
#error "Unsupported compiler for VAR_DECLARE_ALIGN4_AT_SECTION"
#endif

#ifdef __cplusplus
}
#endif
#endif /* _AUDIO_CONF_H_ */
  

  

  






