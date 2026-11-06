/**
  ******************************************************************************
  * @file    AppliNonSecure/Src/main.c
  * @author  GPM Application Team
  * @brief   Non-secure main program.
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2024 STMicroelectronics
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "secure_nsc.h"

#include "common.h"
#include "com.h"
#include "fw_update_app.h"
#include "ns_data.h"
#include "appli_flash_layout.h"

/** @addtogroup STM32N6xx_HAL_Template
  * @{
  */

/** @addtogroup HAL
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/

/* Enable print of boot time (obtained through DWT).
   DWT usage requires product state is not closed/locked.
   OEMxRoT logs must be disabled for relevant boot time. */
/* #define PRINT_BOOT_TIME */

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t *pUserAppId;
const uint8_t UserAppId = 'A';

#ifdef PRINT_BOOT_TIME
static uint64_t time;
static uint32_t end;
#endif

/* Private function prototypes -----------------------------------------------*/
void FW_APP_PrintMainMenu(void);
void FW_APP_Run(void);

void SecureFault_Callback(void);
void SecureError_Callback(void);

#if defined(__ICCARM__)
/* New definition from EWARM V9, compatible with EWARM8 */
int iar_fputc(int ch);
#define PUTCHAR_PROTOTYPE int iar_fputc(int ch)
#elif defined ( __CC_ARM ) || defined(__ARMCC_VERSION)
/* ARM Compiler 5/6*/
int io_putchar(int ch);
#define PUTCHAR_PROTOTYPE int io_putchar(int ch)
#elif defined(__GNUC__)
#define PUTCHAR_PROTOTYPE int32_t uart_putc(int32_t ch)
#endif /* __ICCARM__ */

PUTCHAR_PROTOTYPE
{
  COM_Transmit((uint8_t*)&ch, 1, TX_TIMEOUT);
  return ch;
}

/* Redirects printf to DRIVER_STDIO in case of ARMCLANG*/
#if defined(__ARMCC_VERSION)
FILE __stdout;

/* __ARMCC_VERSION is only defined starting from Arm compiler version 6 */
int fputc(int ch, FILE *f)
{
  /* Send byte to USART */
  io_putchar(ch);

  /* Return character written */
  return ch;
}
#elif defined(__GNUC__)
/* Redirects printf to DRIVER_STDIO in case of GNUARM */
int _write(int fd, char *str, int len)
{
  int i;

  for (i = 0; i < len; i++)
  {
    /* Send byte to USART */
    uart_putc(str[i]);
  }

  /* Return the number of characters written */
  return len;
}
#elif defined(__ICCARM__)
size_t __write(int file, unsigned char const *ptr, size_t len)
{
  size_t idx;
  unsigned char const *pdata = ptr;

  for (idx = 0; idx < len; idx++)
  {
    iar_fputc((int)*pdata);
    pdata++;
  }
  return len;
}
#endif /*  __GNUC__ */

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
#ifdef PRINT_BOOT_TIME
  /* Get boot cycles */
  end = DWT->CYCCNT;
#endif

  /* Reset of all peripherals, Initializes the Flash interface and the systick. */
  HAL_Init();

#ifdef PRINT_BOOT_TIME
  /* Get Boot Time */
  time = ((uint64_t)(end) * 1000U / SystemCoreClock);
#endif

  /* Register SecureFault callback defined in non-secure and to be called by secure handler */
  SECURE_RegisterCallback(SECURE_FAULT_CB_ID, (void *)SecureFault_Callback);

  /* Register SecureError callback defined in non-secure and to be called by secure handler */
  SECURE_RegisterCallback(IAC_ERROR_CB_ID, (void *)SecureError_Callback);

  /*******************************************************************************
  *                          Common Configuration Routines                       *
  *******************************************************************************/

  pUserAppId = (uint8_t *)&UserAppId;

  COM_Init();

#ifdef PRINT_BOOT_TIME
  printf("\r\nBoot time : %u ms at %u MHz", (unsigned int)(time), (unsigned int)(SystemCoreClock/1000000U));
  printf("\r\n");
#endif
  printf("\r\n======================================================================");
  printf("\r\n=              (C) COPYRIGHT 2024 STMicroelectronics                 =");
  printf("\r\n=                                                                    =");
  printf("\r\n=                          User App #%c                               =", *pUserAppId);
  printf("\r\n======================================================================");
  printf("\r\n\r\n");

  /* User App firmware runs*/
  FW_APP_Run();
}

/**
  * @brief  Display the TEST Main Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
void FW_APP_PrintMainMenu(void)
{
  printf("\r\n=================== Main Menu ============================\r\n\n");
#if (NS_DATA_IMAGE_NUMBER == 1)
  printf("  Non-Secure Data --------------------------------------- 1\r\n\n");
#endif /* NS_DATA_IMAGE_NUMBER == 1 */
#if (DOWNLOAD_MENU == 1)
  printf("  New Fw Image ------------------------------------------ 2\r\n\n");
#endif /* DOWNLOAD_MENU == 1 */
  printf("  Selection :\r\n\n");
}

/**
  * @brief  Display the TEST Main Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
void FW_APP_Run(void)
{
  uint8_t key = 0U;

  /*##1- Print Main Menu message*/
  FW_APP_PrintMainMenu();

  while (1U)
  {
    /* Clean the input path */
    COM_Flush();

    /* Receive key */
    if (COM_Receive(&key, 1U, RX_TIMEOUT) == HAL_OK)
    {
      switch (key)
      {
#if (NS_DATA_IMAGE_NUMBER == 1)
        case '1' :
          NS_DATA_Run();
          break;
#endif /* NS_DATA_IMAGE_NUMBER == 1 */
#if (DOWNLOAD_MENU == 1)
        case '2' :
          FW_UPDATE_Run();
          break;
#endif /* DOWNLOAD_MENU == 1 */
        default:
          printf("Invalid Number !\r");
          break;
      }

      /* Print Main Menu message */
      FW_APP_PrintMainMenu();
    }
  }
}

/**
  * @brief  Callback called by secure code following a secure fault interrupt
  * @note   This callback is called by secure code thanks to the registration
  *         done by the non-secure application with non-secure callable API
  *         SECURE_RegisterCallback(SECURE_FAULT_CB_ID, (void *)SecureFault_Callback);
  * @retval None
  */
void SecureFault_Callback(void)
{
  /* Go to infinite loop when Secure fault generated by IDAU/SAU check */
  /* because of illegal access */
  while (1)
  {
  }
}

/**
  * @brief  Callback called by secure code following a IAC secure interrupt (IAC_IRQn)
  * @note   This callback is called by secure code thanks to the registration
  *         done by the non-secure application with non-secure callable API
  *         SECURE_RegisterCallback(IAC_ERROR_CB_ID, (void *)SecureError_Callback);
  * @retval None
  */
void SecureError_Callback(void)
{
  /* Go to infinite loop when Secure error generated by RIF IAC check */
  /* because of illegal access */
  while (1)
  {
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(file);
  UNUSED(line);

  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */
