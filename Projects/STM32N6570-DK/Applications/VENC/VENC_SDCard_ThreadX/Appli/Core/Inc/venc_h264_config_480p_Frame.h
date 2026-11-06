/**
******************************************************************************
* @file          venc_h264_config_480p_Frame.h
* @author        MCD Application Team
 * @brief   VENC H.264 configuration for 480p frames
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

#ifndef __VENC_H264_CONFIG_480P_FRAME_H__
#define __VENC_H264_CONFIG_480P_FRAME_H__


#ifdef __cplusplus
extern "C" {
#endif

/* Frame dimensions and framerate -------------------------------------------*/
/** Frame height in pixels */
#define VENC_HEIGHT    480U
/** Frame width in pixels  */
#define VENC_WIDTH     480U
/** Target framerate (frames per second) */
#define FRAMERATE      30U

/* Hardware mode ------------------------------------------------------------*/
/** Enable hardware handshake / slice / stream mode (0 = disabled) */
#define VENC_HW_MODE_ENABLE     0U

/* Buffer placement ---------------------------------------------------------*/
/** Location macro for VENC internal buffer */
#define VENC_BUFFER_LOCATION     IN_PSRAM
/** Location macro for input frame buffer */
#define INPUT_FRAME_LOCATION     IN_PSRAM


#ifdef __cplusplus
}
#endif

#endif /* __VENC_H264_CONFIG_480P_FRAME_H__ */


