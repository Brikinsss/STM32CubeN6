/**
 ******************************************************************************
 * @file    Appli/Core/Inc/st_monitor_network.h
 * @author  MCD Application Team
 * @brief   Network monitoring interface
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

#ifndef __ST_MONITOR_NETWORK_H__
#define __ST_MONITOR_NETWORK_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/**
 * @brief  Monitor network/bitrate statistics.
 *
 * This function is intended as an entry point for network-related monitoring.
 * Implementations may aggregate network/frame metrics and report or log
 * statistics. The signature is void for compatibility with existing callers.
 *
 * @retval None
 */
void monitor_bitrate(void);

#ifdef __cplusplus
}
#endif

#endif /* __ST_MONITOR_NETWORK_H__ */


