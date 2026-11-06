/**
  ******************************************************************************
  * @file    AppliSecure/Src/secure_nsc.c
  * @author  GPM Application Team
  * @brief   This file contains the non-secure callable APIs (secure world)
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

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "main.h"
#include "secure_nsc.h"
#include <stdio.h>

/** @addtogroup STM32N6xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

extern ARM_DRIVER_FLASH Driver_EXT_FLASH0;

/* Global variables ----------------------------------------------------------*/
void *pSecureFault_Callback = NULL;   /* Pointer to secure fault callback in Non-secure */
void *pSecureError_Callback = NULL;   /* Pointer to secure error callback in Non-secure */

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Secure registration of non-secure callback.
  * @param  CallbackId  callback identifier
  * @param  func        pointer to non-secure function
  * @retval None
  */
CMSE_NS_ENTRY void SECURE_RegisterCallback(SECURE_CallbackIDTypeDef CallbackId, void *func)
{
  if(func != NULL)
  {
    switch(CallbackId)
    {
      case SECURE_FAULT_CB_ID:           /* SecureFault IT */
        pSecureFault_Callback = func;
        break;
      case IAC_ERROR_CB_ID:             /* Illegal access IT */
        pSecureError_Callback = func;
        break;
      default:
        /* unknown */
        break;
    }
  }
}

/**
  * @brief  Secure get flash information.
  * @param  None
  * @param  None
  * @retval None
  */
CMSE_NS_ENTRY void SECURE_GetInfo(ARM_FLASH_INFO* data)
{
  memcpy((void*)data, Driver_EXT_FLASH0.GetInfo(), sizeof(ARM_FLASH_INFO));
}

/**
  * @brief  Secure erase sector function.
  * @param  None
  * @param  None
  * @retval None
  */
CMSE_NS_ENTRY int32_t SECURE_EraseSector(uint32_t addr)
{
  return Driver_EXT_FLASH0.EraseSector(addr);
}

/**
  * @brief  Secure program data function.
  * @param  None
  * @param  None
  * @retval None
  */
CMSE_NS_ENTRY int32_t SECURE_ProgramData(uint32_t addr, const void *data, uint32_t cnt)
{
  return Driver_EXT_FLASH0.ProgramData(addr, data, cnt);
}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
