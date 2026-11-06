/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32n6xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  /* System interrupt init*/

  HAL_PWREx_EnableVddIO4();

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
* @brief LTDC MSP Initialization
* This function configures the hardware resources used in this example
* @param hltdc: LTDC handle pointer
* @retval None
*/
void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hltdc->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspInit 0 */
    /* Typical PCLK is 25 MHz so the PLL1 is configured to provide this clock */
    /* LTDC - PLL4 */
    /* Configure LTDC clock to IC16 with PLL4  */
    
    /* LCD clock configuration */
    /* Typical PCLK is 25 MHz so the PLL4 is configured to provide this clock */
    /* LCD clock configuration */
    /* PLL1_VCO Input = HSI_VALUE = 64 Mhz */
    /* PLL1_VCO Output = PLL1_VCO Input * PLLN / DIVM = 64 Mhz * 25 = 1600 */
    /* PLLLCDCLK = PLL1_VCO Output/64 = 1600/64 = 25Mhz */
    /* LTDC clock frequency = PLLLCDCLK = 25 Mhz */
  /* USER CODE END LTDC_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.LtdcClockSelection = RCC_LTDCCLKSOURCE_IC16;
    PeriphClkInitStruct.ICSelection[RCC_IC16].ClockSelection = RCC_ICCLKSOURCE_PLL4;
    PeriphClkInitStruct.ICSelection[RCC_IC16].ClockDivider = 64;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_LTDC_CLK_ENABLE();

    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**LTDC GPIO Configuration
    PH3     ------> LTDC_B4
    PH6     ------> LTDC_B5
    PD15     ------> LTDC_R2
    PB14     ------> LTDC_HSYNC
    PB13     ------> LTDC_CLK
    PB2     ------> LTDC_B2
    PB15     ------> LTDC_G4
    PE11     ------> LTDC_VSYNC
    PD8     ------> LTDC_R7
    PH4     ------> LTDC_R4
    PD9     ------> LTDC_R1
    PG6     ------> LTDC_B3
    PA1     ------> LTDC_G2
    PB11     ------> LTDC_G6
    PA15(JTDI)     ------> LTDC_R5
    PG15     ------> LTDC_B0
    PG1     ------> LTDC_G1
    PB12     ------> LTDC_G5
    PA7     ------> LTDC_B1
    PG0     ------> LTDC_R0
    PA2     ------> LTDC_B7
    PG12     ------> LTDC_G0
    PB4(NJTRST)     ------> LTDC_R3
    PG8     ------> LTDC_G7
    PA8     ------> LTDC_B6
    PG13     ------> LTDC_DE
    PA0     ------> LTDC_G3
    PG11     ------> LTDC_R6
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_13|GPIO_PIN_2|GPIO_PIN_15
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_15|GPIO_PIN_1|GPIO_PIN_0
                          |GPIO_PIN_12|GPIO_PIN_8|GPIO_PIN_13|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_15|GPIO_PIN_7|GPIO_PIN_2
                          |GPIO_PIN_8|GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LCD;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN LTDC_MspInit 1 */
  /* USER CODE END LTDC_MspInit 1 */

  }

}

/**
* @brief LTDC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hltdc: LTDC handle pointer
* @retval None
*/
void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* hltdc)
{
  if(hltdc->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspDeInit 0 */

  /* USER CODE END LTDC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LTDC_CLK_DISABLE();

    /**LTDC GPIO Configuration
    PH3     ------> LTDC_B4
    PH6     ------> LTDC_B5
    PD15     ------> LTDC_R2
    PB14     ------> LTDC_HSYNC
    PB13     ------> LTDC_CLK
    PB2     ------> LTDC_B2
    PB15     ------> LTDC_G4
    PE11     ------> LTDC_VSYNC
    PD8     ------> LTDC_R7
    PH4     ------> LTDC_R4
    PD9     ------> LTDC_R1
    PG6     ------> LTDC_B3
    PA1     ------> LTDC_G2
    PB11     ------> LTDC_G6
    PA15(JTDI)     ------> LTDC_R5
    PG15     ------> LTDC_B0
    PG1     ------> LTDC_G1
    PB12     ------> LTDC_G5
    PA7     ------> LTDC_B1
    PG0     ------> LTDC_R0
    PA2     ------> LTDC_B7
    PG12     ------> LTDC_G0
    PB4(NJTRST)     ------> LTDC_R3
    PG8     ------> LTDC_G7
    PA8     ------> LTDC_B6
    PG13     ------> LTDC_DE
    PA0     ------> LTDC_G3
    PG11     ------> LTDC_R6
    */
    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_4);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_15|GPIO_PIN_8|GPIO_PIN_9);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_14|GPIO_PIN_13|GPIO_PIN_2|GPIO_PIN_15
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_4);

    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_11);

    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_6|GPIO_PIN_15|GPIO_PIN_1|GPIO_PIN_0
                          |GPIO_PIN_12|GPIO_PIN_8|GPIO_PIN_13|GPIO_PIN_11);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1|GPIO_PIN_15|GPIO_PIN_7|GPIO_PIN_2
                          |GPIO_PIN_8|GPIO_PIN_0);

  /* USER CODE BEGIN LTDC_MspDeInit 1 */

  /* USER CODE END LTDC_MspDeInit 1 */
  }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
