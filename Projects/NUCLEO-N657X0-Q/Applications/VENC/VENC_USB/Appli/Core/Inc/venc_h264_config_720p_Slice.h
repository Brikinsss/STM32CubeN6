/**
******************************************************************************
* @file          venc_h264_config_720p_Slice.h
* @author        MCD Application Team
 * @brief   VENC H.264 configuration for 720p slice mode
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

#ifndef __VENC_H264_CONFIG_720P_SLICE_H__
#define __VENC_H264_CONFIG_720P_SLICE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Frame dimensions and framerate -------------------------------------------*/
/** Frame height in pixels */
#define VENC_HEIGHT    720U
/** Frame width in pixels  */
#define VENC_WIDTH     1280U
/** Target framerate (frames per second) */
#define FRAMERATE      30U

/* Hardware mode ------------------------------------------------------------*/
/** Enable hardware handshake / slice / stream mode (1 = enabled) */
#define VENC_HW_MODE_ENABLE     1U

/* Buffer placement ---------------------------------------------------------*/
/** Location macro for VENC internal buffer */
#define VENC_BUFFER_LOCATION     IN_PSRAM
/** Location macro for input frame buffer */
#define INPUT_FRAME_LOCATION     IN_RAM
  
#define DCMIPP_FORMAT            DCMIPP_PIXEL_PACKER_FORMAT_YUV420_2
#define VENC_INPUT_FORMAT        H264ENC_YUV420_SEMIPLANAR
  
#ifdef __cplusplus
}
#endif

#endif /* __VENC_H264_CONFIG_720P_SLICE_H__ */


