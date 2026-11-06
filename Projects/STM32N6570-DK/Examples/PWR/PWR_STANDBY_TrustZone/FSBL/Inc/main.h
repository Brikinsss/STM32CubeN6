/**
  ******************************************************************************
  * @file    PWR/PWR_STANDBY_TrustZone/FSBL/Inc/main.h
  * @author  GPM Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAIN_H
#define MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32n6xx_hal.h"
#include "extmem.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#if defined (__ARM_FEATURE_CMSE) &&  (__ARM_FEATURE_CMSE == 3U)
  #if defined ( __ICCARM__ )
  #  define CMSE_NS_CALL  __cmse_nonsecure_call
  #  define CMSE_NS_ENTRY __cmse_nonsecure_entry
  #else
  #  define CMSE_NS_CALL  __attribute((cmse_nonsecure_call))
  #  define CMSE_NS_ENTRY __attribute((cmse_nonsecure_entry))
  #endif
#endif
/* Exported types ------------------------------------------------------------*/
#if defined (__ARM_FEATURE_CMSE) &&  (__ARM_FEATURE_CMSE == 3U)
  /* Function pointer declaration in non-secure*/
#if defined ( __ICCARM__ )
typedef void (CMSE_NS_CALL *funcptr_NS)(void);
#else
typedef void CMSE_NS_CALL (*funcptr_NS)(void);
#endif
#endif
/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

#endif /* MAIN_H */
