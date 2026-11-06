/**
  ******************************************************************************
  * @file    main.h
  * @author  MDG Application Team
  * @brief   Header for main.c module
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
#ifndef MAIN_H
#define MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "string.h"
#include "stm32n6xx_hal.h"
#include "stm32n6570_discovery.h"
#include "stm32n6570_discovery_lcd.h"
#include "stm32n6570_discovery_sd.h"
#include "stm32n6570_discovery_ts.h"
#include "stm32n6570_discovery_camera.h"
#include "stm32_lcd.h"

/* Exported variables --------------------------------------------------------*/
extern unsigned char stlogo[];
extern __IO uint32_t UserButtonPressed ;

typedef struct
{
  void     (*DemoFunc)(void);
  uint8_t  DemoName[50];
  uint32_t DemoIndex;
} BSP_DemoTypedef;

void Touchscreen_demo1(void);
#if (USE_TS_MULTI_TOUCH == 1)
void Touchscreen_demo2(void);
void Touchscreen_demo3(void);
#endif /* USE_TS_MULTI_TOUCH == 1 */

void Lcd_Demo(void);
void SD_Demo(void);
void Camera_Demo(void);

void Touchscreen_DrawBackground_Circles(uint8_t state);
uint8_t TouchScreen_GetTouchPosition(void);

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

/* Exported functions ------------------------------------------------------- */

void  Error_Handler(void);
uint8_t CheckForUserInput(void);
void ClearUserInput(void);
#endif /* MAIN_H */
