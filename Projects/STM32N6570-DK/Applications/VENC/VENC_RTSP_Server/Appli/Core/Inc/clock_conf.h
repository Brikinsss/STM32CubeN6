/**
  ******************************************************************************
  * @file           : clock_conf.h
  * @brief          : Clock configuration header
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

#ifndef _CLOCK_CONF_H_
#define _CLOCK_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32n6xx_hal.h"
#include "stm32n6570_discovery.h"

/* Exported types ------------------------------------------------------------*/
typedef struct clock_config_t
{
  uint32_t                 powerSupply;
  uint32_t                 regulatorVoltage;
  SMPSVoltage_TypeDef      smpsVoltage;
  uint32_t                 cpuClkSource;
  uint32_t                 sysClkSource;
  RCC_OscInitTypeDef       oscConfig;
  RCC_OscInitTypeDef       RCC_OscInitStruct;
  RCC_ClkInitTypeDef       RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;
} clock_config_t;

/* Exported functions --------------------------------------------------------*/
/* (none declared here) */

#ifdef __cplusplus
}
#endif

#endif /* _CLOCK_CONF_H_ */


