/**
  ******************************************************************************
  * @file    otp_interface.h
  * @author  MCD Application Team
  * @brief   Header for otp_interface.c module
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
#ifndef OTP_INTERFACE_H
#define OTP_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint32_t Version;
  uint32_t GlobalState;
  uint32_t OtpPart[1000U];
} Otp_Partition_t;

/* Exported constants --------------------------------------------------------*/
#define OTP_PART_SIZE                   (2U * 368U)
#define OTP_WORDS_NUMBER                368U
#define OPENBL_OTP_VERSION              2U
#define OTP_STICKY_PROG_LOCK_MASK       (1U << 27U)
#define OTP_STICKY_WRITE_LOCK_MASK      (1U << 28U)
#define OTP_STICKY_READ_LOCK_MASK       (1U << 29U)
#define OTP_STICKY_LOCK_MASK            0x38000000U
#define OTP_STICKY_LOCK_POS             27U
#define OTP_STICKY_LOCK_ALL             7U
#define OTP_PERM_LOCK_MASK              0x40000000U
#define OTP_PERM_LOCK_POS               30U
#define OTP_PERM_LOCK                   1U
#define OTP_UPDATE_REQUEST_MASK         0x80000000U
#define OTP_UPDATE_REQUEST_POS          31U
#define OTP_UPDATE_REQUEST              1U
#define OTP_LOCK_ERROR                  (1U << 26U)
#define OTP_READ_ERROR                  1U
#define BSEC_SEC_OTP_OPEN               0U
#define BSEC_SEC_OTP_CLOSED             1U
#define BSEC_SEC_OTP_INVALID            3U
#define OTP_ERROR                       -1
#define OTP_OK                          0

/* Exported macro ------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void OPENBL_OTP_Init(void);
void OPENBL_OTP_DeInit(void);
int OPENBL_OTP_Write(Otp_Partition_t Otp);
Otp_Partition_t OPENBL_OTP_Read(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPTIONBYTES_INTERFACE_H */
