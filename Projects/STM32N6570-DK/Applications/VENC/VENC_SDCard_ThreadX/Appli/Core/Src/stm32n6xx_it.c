/**
  ******************************************************************************
  * @file    stm32n6xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "main.h"
#include "stm32n6xx_it.h"
#include "stm32n6570_discovery_camera.h"
#include "stm32n6570_discovery_sd.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim6;
/******************************************************************************/
/*            Cortex-M55 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles Non maskable interrupt (NMI).
  * @note   Default implementation: loop indefinitely for debugging.
  */
void NMI_Handler(void)
{
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory management fault.
  * @note   Indicates MPU or memory access violation.
  */
void MemManage_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief  This function handles Secure Fault exception.
  * @note   Enter infinite loop on secure fault to facilitate debugging.
  */
void SecureFault_Handler(void)
{
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault (pre-fetch/memory access fault).
  */
void BusFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @note   Kept empty by default.
  */
void DebugMon_Handler(void)
{
  while (1)
  {
  }
}
/******************************************************************************/
/*                 STM32N6xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32n6xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles TIM6 interrupt request.
  * @note   HAL_TIM_IRQHandler will call HAL callbacks (e.g., HAL_TIM_PeriodElapsedCallback).
  */
void TIM6_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim6);
}

/**
  * @brief  This function handles CSI (Camera Sensor Interface) interrupt.
  * @note   Forwards handling to HAL DCMIPP CSI IRQ handler provided by BSP/HAL.
  */
void CSI_IRQHandler(void)
{
  HAL_DCMIPP_CSI_IRQHandler(&hcamera_dcmipp);
}

/**
  * @brief  This function handles DCMIPP interrupt request.
  * @note   Forwards to BSP camera IRQ handler for camera instance 0.
  */
void DCMIPP_IRQHandler(void)
{
  BSP_CAMERA_IRQHandler(0);
}


