/**
  ******************************************************************************
  * @file    bl2_nv_service.c
  * @author  MCD Application Team
  * @brief   This file provides all the Non volatile firmware functions.
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
#include "stm32n6xx_hal.h"
#include "string.h"
#include "platform/include/plat_nv_counters.h"
#include "bootutil/bootutil_log.h"
#include "low_level_otp.h"

#define NV_CNT_ID_MAX_VALUE 3   /* Identifier of the non-volatile counter maximum value */
#define NV_CNT_MAX_VALUE 128    /* Non volatile counter maximum value */

/* Functions Definition ------------------------------------------------------*/

/**
  * @brief Initialize Non-Volatile Counter
  * This function initializes the non-volatile counter.
  * @return HAL_StatusTypeDef - status of the initialization
  */
HAL_StatusTypeDef plat_init_nv_counter(void)
{
  /* Enable BSEC & SYSCFG clocks to ensure BSEC data accesses */
  __HAL_RCC_BSEC_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  return HAL_OK;
}

/**
 * @brief Set Non-Volatile Counter
 *
 * This function sets the value of a specified non-volatile counter.
 *
 * @param CounterId The identifier of the non-volatile counter to be set.
 * @param Data The value to set the counter to.
 * @param Updated Pointer to a variable that will be updated with the new counter value.
 *
 * @return HAL_StatusTypeDef - status of the operation
 */
HAL_StatusTypeDef plat_set_nv_counter(enum nv_counter_t CounterId,
                                      uint32_t Data, uint32_t *Updated)
{
  BSEC_HandleTypeDef sBsecHandler = {0};
  uint32_t otp_value[4] = {0};
  uint32_t otp_read_value[4] = {0};
  int32_t i;

  sBsecHandler.Instance = BSEC;

  /* Reset Updated flag */
  *Updated = 0U;

  BOOT_LOG_INF("Counter %d set to 0x%x", CounterId, (unsigned int)Data);

  /* Verify inputs */
  if ((CounterId > NV_CNT_ID_MAX_VALUE) || (Data > NV_CNT_MAX_VALUE))
  {
    return HAL_ERROR;
  }

  /* Create otp values */
  otp_value[3] = (1 << (Data - 97));
  otp_value[2] = (1 << (Data - 65));
  otp_value[1] = (1 << (Data - 33));
  otp_value[0] = (1 << (Data - 1));

  /* Read previous otp values */
  for (i = 0; i < 4; i++)
  {
    if (HAL_BSEC_OTP_Read(&sBsecHandler, OTP_NVCNT_NUMBER + 4 * CounterId + i, &(otp_read_value[i])) != HAL_OK)
    {
      return HAL_ERROR;
    }
  }

  /* Take previous value into account */
  otp_value[3] |= otp_read_value[3];
  otp_value[2] |= otp_read_value[2];
  otp_value[1] |= otp_read_value[1];
  otp_value[0] |= otp_read_value[0];

  /* Program the NVCNT OTP fuses */
  for (i = 0; i < 4; i++)
  {
    if (HAL_BSEC_OTP_Program(&sBsecHandler, OTP_NVCNT_NUMBER + 4 * CounterId + i, otp_value[i], HAL_BSEC_NORMAL_PROG) != HAL_OK)
    {
      return HAL_ERROR;
    }
  }

  return HAL_OK;
}

/**
 * @brief Read Non-Volatile Counter
 *
 * This function reads the value of a specified non-volatile counter.
 *
 * @param CounterId The identifier of the non-volatile counter to be read.
 * @param size The size of the data to be read.
 * @param val Pointer to a buffer where the read value will be stored.
 *
 * @return HAL_StatusTypeDef - status of the read operation
 */
HAL_StatusTypeDef plat_read_nv_counter(enum nv_counter_t CounterId,
                                             uint32_t size, uint8_t *val)
{
  BSEC_HandleTypeDef sBsecHandler = {0};
  uint32_t data = 0;
  uint32_t otp_value[4] = {0};
  int32_t i;
  uint64_t val1 = 0, val2 = 0;
  uint32_t val3 = 0;

  if (CounterId > NV_CNT_ID_MAX_VALUE)
  {
    return HAL_ERROR;
  }

  sBsecHandler.Instance = BSEC;

  for (i = 0; i < 4; i++)
  {
    if (HAL_BSEC_OTP_Read(&sBsecHandler, OTP_NVCNT_NUMBER + 4 * CounterId + i, &(otp_value[i])) != HAL_OK)
    {
      return HAL_ERROR;
    }
  }

  val1 = otp_value[1];
  val1 = val1 << 32;
  val1 |= otp_value[0];

  val2 = otp_value[3];
  val2 = val2 << 32;
  val2 |= otp_value[2];

  for (i = 63; i >= 0; i--)
  {
    val3 = (i > 0) ? (val1 >> i) : val1;
    if (val3)
    {
      data = i + 1;
      break;
    }
  }

  for (i = 63; i >= 0; i--)
  {
    val3 = (i > 0) ? (val2 >> i) : val2;
    if (val3)
    {
      data = i + 65;
      break;
    }
  }

  memcpy(val, &data, sizeof(uint32_t));

  BOOT_LOG_INF("Counter %d get 0x%x", CounterId, (unsigned int)data);

  return HAL_OK;
}
