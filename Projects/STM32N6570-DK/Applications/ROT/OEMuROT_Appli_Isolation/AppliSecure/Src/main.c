/**
  ******************************************************************************
  * @file    AppliSecure/Src/main.c
  * @author  GPM Application Team
  * @brief   Secure main program.
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
#include "stm32n6xx_it.h"
#include "low_level_ext_flash.h"
#include "appli_flash_layout.h"

/** @addtogroup STM32N6xx_HAL_Template
  * @{
  */
/** @addtogroup HAL
  * @{
  */
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Non-secure Vector table to jump to                                         */
/* Caution: address must correspond to non-secure address as it is mapped in  */
/*          the non-secure vector table                                       */
#define VTOR_TABLE_NS_START_ADDR 0x24064400 /* This define is updated automatically from OEMuROT_Boot project */
#define EXT_FLASH_BASE_ADDRESS         XSPI2_BASE          /* External Flash (XSPI2 + MCE2 - AES) */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern ARM_DRIVER_FLASH Driver_EXT_FLASH0;

/* Private function prototypes -----------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
void MPU_Config(void);
void LL_SECU_DisableCleanMpu(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

int main(void)
{
  funcptr_NS NonSecure_ResetHandler;

  /*!!! To boot in a secure way, the RoT has configured and activated the Memory Protection Unit
  In order to keep a secure environment execution, you should reconfigure the
  MPU to make it compatible with your application.
  This example provides a basic configuration for the Memory Protection Unit*/
  MPU_Config();

  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();

  /* Enable BusFault and SecureFault handlers (HardFault is default) */
  SCB->SHCSR |= (SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_SECUREFAULTENA_Msk);

  /* Reset of all peripherals, Initializes the Flash interface and the systick. */
  HAL_Init();

  /* All IOs are by default allocated to secure */
  /* Release them all to non-secure */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPION_CLK_ENABLE();
  __HAL_RCC_GPIOO_CLK_ENABLE();
  __HAL_RCC_GPIOP_CLK_ENABLE();
  __HAL_RCC_GPIOQ_CLK_ENABLE();

  HAL_GPIO_ConfigPinAttributes(GPIOA, GPIO_PIN_ALL, GPIO_PIN_NSEC);
  HAL_GPIO_ConfigPinAttributes(GPIOB, GPIO_PIN_ALL, GPIO_PIN_NSEC);
  HAL_GPIO_ConfigPinAttributes(GPIOC, GPIO_PIN_ALL, GPIO_PIN_NSEC);
  HAL_GPIO_ConfigPinAttributes(GPIOD, GPIO_PIN_ALL, GPIO_PIN_NSEC);
  HAL_GPIO_ConfigPinAttributes(GPIOE, GPIO_PIN_ALL, GPIO_PIN_NSEC);
  HAL_GPIO_ConfigPinAttributes(GPIOF, GPIO_PIN_ALL, GPIO_PIN_NSEC);
  HAL_GPIO_ConfigPinAttributes(GPIOG, GPIO_PIN_ALL, GPIO_PIN_NSEC);
  HAL_GPIO_ConfigPinAttributes(GPIOH, GPIO_PIN_ALL, GPIO_PIN_NSEC);
  HAL_GPIO_ConfigPinAttributes(GPION, GPIO_PIN_ALL, GPIO_PIN_NSEC);
  HAL_GPIO_ConfigPinAttributes(GPIOO, GPIO_PIN_ALL, GPIO_PIN_NSEC);
  HAL_GPIO_ConfigPinAttributes(GPIOP, GPIO_PIN_ALL, GPIO_PIN_NSEC);
  HAL_GPIO_ConfigPinAttributes(GPIOQ, GPIO_PIN_ALL, GPIO_PIN_NSEC);

  /* Set USART1 as configurable by non-secure */
  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_USART1, RIF_ATTRIBUTE_NSEC);

#if (DOWNLOAD_MENU == 1)
  Driver_EXT_FLASH0.Initialize(NULL);
#endif /* DOWNLOAD_MENU == 1 */

  /*************** Setup and jump to non-secure *******************************/

  /* Set non-secure vector table location */
  SCB_NS->VTOR = VTOR_TABLE_NS_START_ADDR;

  /* Set non-secure main stack (MSP_NS) */
  __TZ_set_MSP_NS((*(uint32_t *)VTOR_TABLE_NS_START_ADDR));

  /* Get non-secure reset handler */
  NonSecure_ResetHandler = (funcptr_NS)(*((uint32_t *)((VTOR_TABLE_NS_START_ADDR) + 4U)));

  /* Start non-secure state software application */
  NonSecure_ResetHandler();

  /* Non-secure software does not return, this code is not executed */
  while (1) {
    __NOP();
  }
}

void MPU_Config(void)
{
  /* Disable and clean previous MPU config */
  LL_SECU_DisableCleanMpu();

  MPU_Region_InitTypeDef default_config = {0};
  MPU_Attributes_InitTypeDef attr_config = {0};
  uint32_t primask_bit = __get_PRIMASK();

  /* Disable the MPU to allow configuration changes */
  HAL_MPU_Disable();

  /* Create attribute configuration for the MPU */
  attr_config.Attributes = INNER_OUTER(MPU_NOT_CACHEABLE);
  attr_config.Number = MPU_ATTRIBUTES_NUMBER0;
  HAL_MPU_ConfigMemoryAttributes(&attr_config);

  attr_config.Attributes = INNER_OUTER(MPU_NO_ALLOCATE);
  attr_config.Number = MPU_ATTRIBUTES_NUMBER1;
  HAL_MPU_ConfigMemoryAttributes(&attr_config);

  /* Common settings for all MPU regions configured below */
  default_config.Enable = MPU_REGION_ENABLE;
  default_config.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  default_config.AccessPermission = MPU_REGION_ALL_RW;
  default_config.IsShareable = MPU_ACCESS_NOT_SHAREABLE;

  /* Configure a non cacheable Flash region (Primary and Secondary slots: Appli Secure and Nonsecure, Data Secure and Nonsecure) */
  default_config.AttributesIndex = MPU_ATTRIBUTES_NUMBER1;
  default_config.Number = MPU_REGION_NUMBER0;
  default_config.BaseAddress = EXT_FLASH_BASE_ADDRESS + S_APPLI_OFFSET;
  default_config.LimitAddress = EXT_FLASH_BASE_ADDRESS + S_APPLI_SECONDARY_OFFSET + S_DATA_PARTITION_SIZE + NS_DATA_PARTITION_SIZE + NS_APPLI_PARTITION_SIZE + S_APPLI_PARTITION_SIZE - 1;
  HAL_MPU_ConfigRegion(&default_config);

  /* Configure The SRAM2 */
  default_config.AttributesIndex = MPU_ATTRIBUTES_NUMBER0;
  default_config.Number = MPU_REGION_NUMBER1;
  default_config.BaseAddress = SRAM2_AXI_BASE_S;
  default_config.LimitAddress =SRAM2_AXI_BASE_S + SRAM2_AXI_SIZE - 1;
  HAL_MPU_ConfigRegion(&default_config);

  /* Configure The SRAM1 */
  default_config.AttributesIndex = MPU_ATTRIBUTES_NUMBER0;
  default_config.Number = MPU_REGION_NUMBER2;
  default_config.BaseAddress = SRAM1_AXI_BASE_S;
  default_config.LimitAddress =SRAM1_AXI_BASE_S + SRAM1_AXI_SIZE - 1;
  HAL_MPU_ConfigRegion(&default_config);

  /* enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

  /* Exit critical section to lock the system and avoid any issue around MPU mechanisme */
  __set_PRIMASK(primask_bit);
}

/**
  * @brief  MPU cleaning
  * @param  None
  * @retval None
  */
void LL_SECU_DisableCleanMpu(void)
{
  uint8_t i;

  MPU->CTRL = 0;

  for(i = MPU_REGION_NUMBER0; i <= MPU_REGION_NUMBER15; i++)
  {
    HAL_MPU_DisableRegion(i);
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
