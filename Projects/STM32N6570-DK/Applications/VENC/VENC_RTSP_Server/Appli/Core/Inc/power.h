/**
 ******************************************************************************
 * @file    Appli/Core/Inc/power.h
 * @author  MCD Application Team
 * @brief   Power configuration header
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

#ifndef __POWER_H__
#define __POWER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "clock_conf.h"

/* Exported constants --------------------------------------------------------*/
#define POWER_CONFIG_MAX 20U

/* Power configuration bitflags */
typedef enum
{
  POWER_CONFIG_NONE                  = 0,
  POWER_CONFIG_800MHZ_400MHZ         = 1U << 0,
  POWER_CONFIG_600MHZ_400MHZ         = 1U << 1,
  POWER_CONFIG_400MHZ_400MHZ         = 1U << 2,
  POWER_CONFIG_100MHz_400MHZ         = 1U << 3,
  POWER_CONFIG_25MHZ_200MHZ          = 1U << 4,
  POWER_CONFIG_12_5MHz_400MHZ        = 1U << 5,
  POWER_CONFIG_12_5MHz_200MHZ        = 1U << 6
} power_config_t;

/* Default configuration if not provided by build system */
#ifndef POWER_CLOCK_DEFAULT_CONFIG
#define POWER_CLOCK_DEFAULT_CONFIG POWER_CONFIG_600MHZ_400MHZ
#endif

#ifndef PWR_SOURCE
#define PWR_SOURCE PWR_EXTERNAL_SOURCE_SUPPLY
#endif

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  Apply the default system clock and power configuration.
 * @retval None
 */
void power_default_system_clock_config(void);

/**
 * @brief  Apply an alternate system clock and power configuration.
 * @param  config  power_config_t configuration flags
 * @retval None
 */
void power_alternate_system_clock_config(power_config_t config);

#ifdef __cplusplus
}
#endif

#endif /* __POWER_H__ */


