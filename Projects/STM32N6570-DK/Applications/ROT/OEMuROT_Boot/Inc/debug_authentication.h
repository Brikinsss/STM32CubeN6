/**
  ******************************************************************************
  * @file    debug_authentication.h
  * @author  GPM Application Team
  * @brief   Header for debug_authentication.c module
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
#ifndef DEBUG_AUTHENTICATION_H
#define DEBUG_AUTHENTICATION_H

#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions ------------------------------------------------------- */
HAL_StatusTypeDef debug_authentication(void);
HAL_StatusTypeDef OTP_Get_Lifecycle_Status(uint8_t* secure_boot, uint8_t* prov_done);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_AUTHENTICATION_H */
