/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_host_video.h
  * @author  MCD Application Team
  * @brief   USBX Host applicative header file
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
#ifndef __UX_HOST_VIDEO_H__
#define __UX_HOST_VIDEO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_host_class_video.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

/* USER CODE BEGIN EFP */
VOID video_thread_entry(ULONG thread_input);
UINT ux_host_video_stream_negotiate(UX_HOST_CLASS_VIDEO *video);
VOID ux_host_video_stream_read();
VOID video_transfer_request_completion(UX_HOST_CLASS_VIDEO_TRANSFER_REQUEST *video_transfer_request);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */
typedef enum {
  VIDEO_IDLE = 0,
  VIDEO_START,
  VIDEO_READ,
  VIDEO_WAIT,
}video_streaming_stateTypeDef;

/* USER CODE END PD */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif
#endif  /* __UX_HOST_VIDEO_H__ */