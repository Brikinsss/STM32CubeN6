/**
 ******************************************************************************
 * @file    Appli/Core/Inc/venc_jpeg_config_5MP_Slice_noPSRAM.h
 * @author  MCD Application Team / Antigravity
 * @brief   VENC JPEG configuration for 5MP (2592x1944) slice mode (no PSRAM)
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

#ifndef __VENC_JPEG_CONFIG_5MP_SLICE_NO_PSRAM_H__
#define __VENC_JPEG_CONFIG_5MP_SLICE_NO_PSRAM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Frame dimensions and framerate -------------------------------------------*/
/** Frame height in pixels (Sensor native height cropped/downscaled to 32-line slice boundary: 60 * 32 = 1920) */
#define VENC_HEIGHT                     1944U
/** Frame width in pixels (Sensor native maximum width) */
#define VENC_WIDTH                      2592U
/** Target framerate (frames per second) */
#define FRAMERATE                       10U

/* Hardware mode ------------------------------------------------------------*/
/** Enable hardware handshake / slice / stream mode (1 = enabled) */
#define VENC_HW_MODE_ENABLE             1U

/* Buffer placement ---------------------------------------------------------*/
/** Location macro for VENC internal buffer */
#define VENC_BUFFER_LOCATION            IN_UNCACHED_RAM

/** Location macro for input frame buffer */
#define INPUT_FRAME_LOCATION            IN_RAM

/* Pixel / format configuration ---------------------------------------------*/
/** DCMIPP pixel packer format */
#define DCMIPP_FORMAT                   DCMIPP_PIXEL_PACKER_FORMAT_YUV420_2
/** VENC input pixel format */
#define VENC_INPUT_FORMAT               JPEGENC_YUV420_SEMIPLANAR
/** Bytes per pixel (YUV420 = 12 bits per pixel => 1.5 bytes) */
#define DCMIPP_BYTES_PER_PIXELS         (1.5f)

/**
 * Pool size for encoder buffers (bytes).
 * For JPEG, NO reference frames are needed! 64KB is plenty for EWL wrapper.
 */
#define VENC_POOL_SIZE                  (64U * 1024U)

/* Size of output JPEG bitstream buffer (800 KB fits within 1024 KB NOCACHE_Region without PSRAM) */
#ifndef VENC_OUTPUT_BUFFER_SIZE
#define VENC_OUTPUT_BUFFER_SIZE         (800U * 1024U)
#endif

/* JPEG Quality level (0 - 9, 3 is optimal for 5MP slice mode in internal SRAM) */
#ifndef VENC_JPEG_QLEVEL
#define VENC_JPEG_QLEVEL                2U
#endif

#ifdef __cplusplus
}
#endif

#endif /* __VENC_JPEG_CONFIG_5MP_SLICE_NO_PSRAM_H__ */
