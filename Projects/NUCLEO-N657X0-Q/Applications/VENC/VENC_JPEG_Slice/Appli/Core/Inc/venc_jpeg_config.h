/**
 ******************************************************************************
 * @file    Appli/Core/Inc/venc_jpeg_config.h
 * @author  MCD Application Team / Antigravity
 * @brief   VENC JPEG configuration header for slice mode
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

#ifndef __VENC_JPEG_CONFIG_H__
#define __VENC_JPEG_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>
#include <stdint.h>
#include "stm32n6xx_hal.h"
#include "stm32n6xx_ll_venc.h"
#include "jpegencapi.h"
#include "venc_jpeg_config_5MP_Slice_noPSRAM.h"

/* Exported types ------------------------------------------------------------*/

typedef struct cam_jpeg_cfg_t
{
  uint32_t width;  /**< frame width in pixels */
  uint32_t height; /**< frame height in pixels */
} cam_jpeg_cfg_t;

typedef struct dcmipp_jpeg_cfg_t
{
  float    bytes_per_pixel;
  uint32_t format; /**< pixel packer format identifier */
  uint32_t pitch;  /**< line pitch in bytes */
} dcmipp_jpeg_cfg_t;

typedef struct venc_jpeg_cfg_t
{
  JpegEncCfg cfgJpeg;
  uint32_t   frameSeqNum;
  uint32_t   frameHeader;
} venc_jpeg_cfg_t;

/* Backward compatibility aliases for dcmipp_app.c */
typedef cam_jpeg_cfg_t    cam_h264_cfg_t;
typedef dcmipp_jpeg_cfg_t dcmipp_h264_cfg_t;
typedef venc_jpeg_cfg_t   venc_h264_cfg_t;

/* Exported variables --------------------------------------------------------*/
extern cam_jpeg_cfg_t     hCamJpegInstance;
extern dcmipp_jpeg_cfg_t  hDcmippJpegInstance;
extern venc_jpeg_cfg_t    hVencJpegInstance;

#define hCamH264Instance    hCamJpegInstance
#define hDcmippH264Instance hDcmippJpegInstance
#define hVencH264Instance   hVencJpegInstance

/* Exported functions --------------------------------------------------------*/
void EWLPoolChoiceCb(u8 **pool_ptr, size_t *size);
void EWLPoolReleaseCb(u8 **pool_ptr);
uint8_t * GetInputFrame(uint32_t * framSize);
bool IsHwHanshakeMode(void);
uint32_t GetDCMIPPFormat(void);
HAL_StatusTypeDef GetDCMIPPLinesConfig(uint32_t *pWarpLines, uint32_t *pIrqLines);
uint8_t * GetNextFrame(uint32_t frame_number);
uint32_t GetNbInputFrame(void);
uint8_t * GetOutputBuffer(uint32_t * bufferSize);
uint32_t GetDCMIPPNbLinesCaptured(void);
uint32_t GetVideoFramerate(void);

#ifdef __cplusplus
}
#endif

#endif /* __VENC_JPEG_CONFIG_H__ */
