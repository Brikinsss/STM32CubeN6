/**
  ******************************************************************************
  * @file    otp_interface.c
  * @author  MCD Application Team
  * @brief   Contains One-time programmable access functions
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

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "common_interface.h"
#include "openbl_core.h"

#include "openbl_mem.h"

#include "app_openbootloader.h"
#include "otp_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static BSEC_HandleTypeDef hBsec = {0U};
static uint32_t OtpValueIdx;
static uint32_t OtpStatusIdx;
static uint32_t OtpIdx;

/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Initialize the OTP driver, Set BSEC instance and enable needed clocks.
  * @retval None.
  */
void OPENBL_OTP_Init(void)
{
  hBsec.Instance = BSEC;

  /* Enable BSEC & SYSCFG clocks to ensure BSEC data accesses */
  __HAL_RCC_BSEC_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();
}

/**
  * @brief  De-initialize the OTP driver.
  * @retval None.
  */
void OPENBL_OTP_DeInit(void)
{
  __HAL_RCC_BSEC_CLK_DISABLE();
  __HAL_RCC_SYSCFG_CLK_DISABLE();
}

/**
  * @brief  Read the OTP area.
  * @retval Returns the OTP structure that contains the read values.
  */
Otp_Partition_t OPENBL_OTP_Read(void)
{
  uint32_t otp_value;
  uint32_t sticky_lock_value;
  uint32_t otp_perm_lock_value;
  uint32_t lock_state;
  uint32_t otp_read_error;
  uint32_t lifecycle_state = 0U;
  HAL_StatusTypeDef status;
  Otp_Partition_t otp = {0U};

  /* Get the OTP version */
  otp.Version = OPENBL_OTP_VERSION;

  /* Get security status */
  status = HAL_BSEC_GetDeviceLifeCycleState(&hBsec, &lifecycle_state);

  /* Check the status */
  if (status != HAL_OK)
  {
    /* Save OTP security as invalid */
    lifecycle_state = HAL_BSEC_INVALID_STATE;
  }

  /* Set the OTP security */
  if (lifecycle_state == HAL_BSEC_OPEN_STATE)
  {
    otp.GlobalState = BSEC_SEC_OTP_OPEN;
  }
  else if (lifecycle_state == HAL_BSEC_CLOSED_STATE)
  {
    otp.GlobalState = BSEC_SEC_OTP_CLOSED;
  }
  else
  {
    otp.GlobalState = BSEC_SEC_OTP_INVALID;
  }

  /* Get the OTP values */
  for (OtpValueIdx = 0U, OtpStatusIdx = OtpValueIdx + 1U, OtpIdx = (OtpValueIdx / 2U);
       OtpValueIdx < OTP_PART_SIZE;
       OtpValueIdx += 2U, OtpStatusIdx = OtpValueIdx + 1U, OtpIdx = (OtpValueIdx / 2U))
  {
    /* Reset the read status variable */
    otp_read_error = 0U;

    /* Read the OTP word */
    status = HAL_BSEC_OTP_Read(&hBsec, OtpIdx, &otp_value);

    /* Save the OTP value */
    otp.OtpPart[OtpValueIdx] = otp_value;

    /* Check the status */
    if (status != HAL_OK)
    {
      /* Save status as read error */
      otp_read_error += OTP_READ_ERROR;

      /* Save the OTP value */
      otp.OtpPart[OtpStatusIdx] = otp_read_error;

      /* Update the OTP value to 0 as read error */
      otp.OtpPart[OtpValueIdx] = 0U;

      /* Continue to the next OTP */
      continue;
    }

    /* Get the fuse lock status */
    status = HAL_BSEC_OTP_GetState(&hBsec, OtpIdx, &lock_state);

    /* Check the status */
    if (status != HAL_OK)
    {
      /* Save status as lock error */
      otp_read_error += OTP_LOCK_ERROR;

      /* Save the OTP value */
      otp.OtpPart[OtpStatusIdx] = otp_read_error;

      /* Continue to the next OTP */
      continue;
    }

    /* Check the sticky lock status */
    if ((lock_state & HAL_BSEC_FUSE_PROG_LOCKED) == HAL_BSEC_FUSE_PROG_LOCKED)
    {
      sticky_lock_value = 1U;
    }
    else
    {
      sticky_lock_value = 0U;
    }

    /* Check the permanent lock status */
    if ((lock_state & HAL_BSEC_FUSE_LOCKED) == HAL_BSEC_FUSE_LOCKED)
    {
      otp_perm_lock_value = 1U;
    }
    else
    {
      otp_perm_lock_value = 0U;
    }

    /* Set the OTP status */
    otp.OtpPart[OtpStatusIdx] = (sticky_lock_value << OTP_STICKY_LOCK_POS) + (otp_perm_lock_value << OTP_PERM_LOCK_POS);
  }

  return otp;
}

/**
  * @brief  Write in OTP area.
  * @param  Otp the otp structure that contains the values to be written.
  * @retval Returns OTP_OK if no error else returns OTP_ERROR.
  */
int OPENBL_OTP_Write(Otp_Partition_t Otp)
{
  uint32_t otp_value;
  uint32_t otp_perm_lock_value;
  uint32_t otp_sticky_lock_value;
  uint32_t otp_update_request;
  HAL_StatusTypeDef status;

  for (OtpValueIdx = 0U, OtpStatusIdx = OtpValueIdx + 1U, OtpIdx = (OtpValueIdx / 2U);
       OtpValueIdx < OTP_PART_SIZE;
       OtpValueIdx += 2U, OtpStatusIdx = OtpValueIdx + 1U, OtpIdx = (OtpValueIdx / 2U))
  {
    /* Get the request update bit */
    otp_update_request = (uint8_t)((Otp.OtpPart[OtpStatusIdx] & OTP_UPDATE_REQUEST_MASK) >> OTP_UPDATE_REQUEST_POS);

    /* Check if there is request update */
    if (otp_update_request != OTP_UPDATE_REQUEST)
    {
      /* Continue to the next OTP as no update on this one */
      continue;
    }

    /* ### Program the otp value ### */
    /* Get the otp value */
    otp_value = Otp.OtpPart[OtpValueIdx];

    /* Get the permanent lock value */
    otp_perm_lock_value = (uint8_t)((Otp.OtpPart[OtpStatusIdx] & OTP_PERM_LOCK_MASK) >> OTP_PERM_LOCK_POS);

    /* Check if there is a permanent lock */
    if (otp_perm_lock_value == OTP_PERM_LOCK)
    {
      /* Program the permanent lock */
      otp_perm_lock_value = HAL_BSEC_LOCK_PROG;
    }

    /* Skip write value if value = 0 */
    if (otp_value != 0U || (otp_perm_lock_value == HAL_BSEC_LOCK_PROG))
    {
      if (otp_perm_lock_value == HAL_BSEC_LOCK_PROG)
      {
        /* Write the otp value with permanent lock */
        (void)HAL_BSEC_OTP_Program(&hBsec, OtpIdx, otp_value, HAL_BSEC_LOCK_PROG);
      }
      else if ((otp_perm_lock_value != HAL_BSEC_LOCK_PROG))
      {
        /* Write the otp value */
        (void)HAL_BSEC_OTP_Program(&hBsec, OtpIdx, otp_value, HAL_BSEC_NORMAL_PROG);
      }
    }

    /* ### Program the otp status ### */
    /* Status frame useful values */
    /* bit 0  = read error detected, 1 => invalid value */
    /* bit 26 = lock error */
    /* bit 27 = sticky programming lock */
    /* bit 28 = shadow write sticky lock */
    /* bit 29 = shadow read sticky lock */
    /* bit 30 = permanent write lock */
    /* bit 31 = request update */
    /* ############################## */

    /* Get the sticky lock value (1 = bit 27, 2 = bit 28, 4 = bit 29) */
    otp_sticky_lock_value = (uint8_t)((Otp.OtpPart[OtpStatusIdx] & OTP_STICKY_LOCK_MASK) >> OTP_STICKY_LOCK_POS);

    /* Check if there is a sticky lock */
    if ((otp_sticky_lock_value & OTP_STICKY_LOCK_ALL) != 0U)
    {
      /* Program the sticky lock */
      status = HAL_BSEC_OTP_Lock(&hBsec, OtpIdx, otp_sticky_lock_value);

      /* Check the status */
      if (status != HAL_OK)
      {
        return OTP_ERROR;
      }
    }
  }

  return OTP_OK;
}
