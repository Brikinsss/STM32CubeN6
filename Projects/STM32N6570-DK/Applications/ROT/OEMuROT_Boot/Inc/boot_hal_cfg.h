/**
  ******************************************************************************
  * @file    boot_hal_cfg.h
  * @author  MCD Application Team
  * @brief   File fixing configuration flag specific for STM32N6xx platform
  *
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BOOT_HAL_CFG_H
#define BOOT_HAL_CFG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32n6xx_hal.h"

/* RTC clock */
#define RTC_CLOCK_SOURCE_LSI
#ifdef RTC_CLOCK_SOURCE_LSI
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00F9
#endif
#ifdef RTC_CLOCK_SOURCE_LSE
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00FF
#endif

/* Cache */
#define OEMUROT_CACHE_ENABLE /*!< Cache activated : instruction and data */

/* Tamper */
#define OEMUROT_INTERNAL_TAMPER_ENABLE /*!< Internal TAMPER activated */

#define NO_TAMPER            (0)             /*!< No tamper activated */
#define INTERNAL_TAMPER_ONLY (1)             /*!< Only Internal tamper activated */
#define OEMUROT_TAMPER_ENABLE INTERNAL_TAMPER_ONLY      /*!< TAMPER configuration flag  */

#ifdef OEMUROT_DEV_MODE
#define OEMUROT_ERROR_HANDLER_STOP_EXEC /*!< Error handler stops execution (else it resets) */
#else
#define OEMUROT_SECURE_BOOT /*!< Device lifecycle expected to be locked */
#endif /* OEMUROT_DEV_MODE */

/* Run time protections */
#define OEMUROT_MPU_PROTECTION    /*!< OEMUROT_Boot uses MPU to prevent execution outside of OEMUROT_Boot code */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
__NO_RETURN void Error_Handler(void);
#endif /* BOOT_HAL_CFG_H */
