/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_video.h
  * @author  MCD Application Team
  * @brief   USBX Device Video applicative header file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UX_DEVICE_VIDEO_H__
#define __UX_DEVICE_VIDEO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_device_class_video.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "ux_device_descriptors.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define PAYLOAD_HEADER_SIZE    0x0C
#define FID_NEF_FALSE          0x8C
#define FID_NEF_TRUE           0x8D
#define FID_EF_FALSE           0x8E
#define FID_EF_TRUE            0x8F

 /* Source Clock Reference:HSI48 clock*/
#define SOURCE_CLOCK_REF      0x2DC6C00

#define FID_false      0
#define FID_true       1
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
VOID USBD_VIDEO_Activate(VOID *video_instance);
VOID USBD_VIDEO_Deactivate(VOID *video_instance);
VOID USBD_VIDEO_StreamChange(UX_DEVICE_CLASS_VIDEO_STREAM *video_stream,
                             ULONG alternate_setting);
VOID USBD_VIDEO_StreamPayloadDone(UX_DEVICE_CLASS_VIDEO_STREAM *video_stream,
                                  ULONG length);
UINT USBD_VIDEO_StreamRequest(UX_DEVICE_CLASS_VIDEO_STREAM *video_stream,
                              UX_SLAVE_TRANSFER *transfer);
ULONG USBD_VIDEO_StreamGetMaxPayloadBufferSize(VOID);

UINT usb_video_data_read(UCHAR **data, ULONG *size);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* UVC uses only 26 first bytes */
typedef struct
{
  uint16_t bmHint;
  uint8_t bFormatIndex;
  uint8_t bFrameIndex;
  uint32_t dwFrameInterval;
  uint16_t wKeyFrameRate;
  uint16_t wPFrameRate;
  uint16_t wCompQuality;
  uint16_t wCompWindowSize;
  uint16_t wDelay;
  uint32_t dwMaxVideoFrameSize;
  uint32_t dwMaxPayloadTransferSize;
  uint32_t dwClockFrequency;
  uint8_t bmFramingInfo;
  uint8_t bPreferedVersion;
  uint8_t bMinVersion;
  uint8_t bMaxVersion;
  uint8_t bUsage;
  uint8_t bBitDepthLuma;
  uint8_t bmSettings;
  uint8_t bMaxNumberOfRefFramesPlus1;
  uint16_t bmRateControlModes;
  uint32_t bmLayoutPerStream;
} __PACKED USBD_VideoControlTypeDef;

/* USER CODE END PD */

#ifndef USBD_VIDEO_STREAM_NMNBER
#define USBD_VIDEO_STREAM_NMNBER  1
#endif

#ifndef USBD_VIDEO_PAYLOAD_BUFFER_NUMBER
#define USBD_VIDEO_PAYLOAD_BUFFER_NUMBER  4
#endif

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif
#endif  /* __UX_DEVICE_VIDEO_H__ */
