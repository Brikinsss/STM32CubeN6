/**
******************************************************************************
* @file          plugin_audio.h
* @author        MCD Application Team
* @brief         Implement bitrate monitoring
*******************************************************************************
* @attention
*
* Copyright (c) 2019(-2025) STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
********************************************************************************
*/


#ifndef plugin_audio_h
#define plugin_audio_h
/* Includes ------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stddef.h>

  
/**
 * @brief  Return the last PCM frame sequence number produced by the audio ISR.
 * @retval uint32_t Last PCM frame number (incrementing counter)
 */
uint32_t AUDIO_APP_LastPcmFrameNumber(void);

/**
 * @brief  Get pointer to the most recent ready PCM data and its size.
 * @param  data  Output pointer that will point to the PCM buffer (half-buffer)
 * @param  size  Output size in bytes of the PCM buffer region pointed by data
 * @retval uint32_t Status code (0 = success)
 */
int32_t AUDIO_APP_GetData(uint8_t **data, size_t *size);

/**
 * @brief  Start audio encoding pipeline (optional implementation).
 * @retval uint32_t Status code (0 = success)
 */
uint32_t AUDIO_APP_EncodingStart(void);


/**
 * @brief  Stop the video encoding pipeline and release resources.
 * @retval UINT Status code
 */
uint32_t AUDIO_APP_EncodingStop(void);

#ifdef __cplusplus
};
#endif
#endif /* st_monitor_h */


