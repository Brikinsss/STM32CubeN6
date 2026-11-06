#include "main.h"
#include "stm32n6570_discovery.h"

/**
* @brief  System Clock Configuration
*         The system Clock is configured as follow :
*            CPU Clock source               = IC1 (from PLL1)
*            CPUCLK(Hz)                     = 800000000
*            System bus Clock source        = IC2 (from PLL1)
*            SYSCLK(Hz)                     = 400000000
*            HCLK(Hz)                       = 200000000
*            AHB Prescaler                  = 2
*            APB1 Prescaler                 = 1
*            APB2 Prescaler                 = 1
*            APB4 Prescaler                 = 1
*            APB5 Prescaler                 = 1
*            HSI Frequency(Hz)              = 64000000
*            PLL1 State                     = ON
*            PLL2 State                     = ON
*            PLL3 State                     = BYPASS
*            PLL4 State                     = ON
* @retval None
*/
void SystemClock_Config(void)
{
  /* In debug mode the clock may not be configured when entering the app.
     Configure it here. */
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  BSP_SMPS_Init(SMPS_VOLTAGE_OVERDRIVE);
  HAL_Delay(1);

  /* Configure the system power supply */
  if (HAL_PWREx_ConfigSupply(PWR_EXTERNAL_SOURCE_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set the main internal regulator output voltage to Scale0 (highest perf) */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE0) != HAL_OK)
  {
    Error_Handler();
  }

  /* If currently running from an internal clock (IC1 or IC2/IC6/IC11),
     switch temporarily to HSI before reconfiguring PLLs and interconnect clocks. */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct);
  if ((RCC_ClkInitStruct.CPUCLKSource == RCC_CPUCLKSOURCE_IC1) ||
      (RCC_ClkInitStruct.SYSCLKSource == RCC_SYSCLKSOURCE_IC2_IC6_IC11))
  {
    RCC_ClkInitStruct.ClockType   = (RCC_CLOCKTYPE_CPUCLK | RCC_CLOCKTYPE_SYSCLK);
    RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_HSI;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /* HSI selected as PLL sources
     PLL1: ((64MHz / 2) * 25) / P1 = ((32MHz) * 25) / 1 = 800MHz
           IC1 divider = 1  -> CPUCLK = 800MHz
           IC2 divider = 2  -> SYSCLK AXI = 400MHz
           IC6 divider = 2  -> SYSCLK NPU = 400MHz
           IC11 divider = 2 -> SYSCLK AXISRAM3/4/5/6 = 400MHz
     PLL2: ((64MHz / 8) * 125) / P1 = 1000MHz (when selected)
     PLL3: BYPASS (HSI path)
     PLL4: ((64MHz / 32) * 40) / P1 = 80MHz */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv              = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

  RCC_OscInitStruct.PLL1.PLLState      = RCC_PLL_ON;
  RCC_OscInitStruct.PLL1.PLLSource     = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL1.PLLM          = 2;
  RCC_OscInitStruct.PLL1.PLLN          = 25;
  RCC_OscInitStruct.PLL1.PLLP1         = 1;
  RCC_OscInitStruct.PLL1.PLLP2         = 1;
  RCC_OscInitStruct.PLL1.PLLFractional = 0;

  /* PLL2: 64 / 8 = 8 MHz; 8 * 125 = 1000 MHz; P1 = P2 = 1 -> 1000 MHz */
  RCC_OscInitStruct.PLL2.PLLState      = RCC_PLL_ON;
  RCC_OscInitStruct.PLL2.PLLSource     = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL2.PLLM          = 8;
  RCC_OscInitStruct.PLL2.PLLFractional = 0;
  RCC_OscInitStruct.PLL2.PLLN          = 125;
  RCC_OscInitStruct.PLL2.PLLP1         = 1;
  RCC_OscInitStruct.PLL2.PLLP2         = 1;

  /* PLL3: bypass */
  RCC_OscInitStruct.PLL3.PLLState  = RCC_PLL_BYPASS;
  RCC_OscInitStruct.PLL3.PLLSource = RCC_PLLSOURCE_HSI;

  /* PLL4: 64 / 32 = 2 MHz; 2 * 40 = 80 MHz; P1 = P2 = 1 -> 80 MHz */
  RCC_OscInitStruct.PLL4.PLLState      = RCC_PLL_ON;
  RCC_OscInitStruct.PLL4.PLLSource     = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL4.PLLM          = 32;
  RCC_OscInitStruct.PLL4.PLLFractional = 0;
  RCC_OscInitStruct.PLL4.PLLN          = 40;
  RCC_OscInitStruct.PLL4.PLLP1         = 1;
  RCC_OscInitStruct.PLL4.PLLP2         = 1;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Interconnect clock routing
     - IC1 (CPUCLK source)  : PLL1 -> divider 1 -> 800 MHz (sysa_ck)
     - IC2 (AXI SYSCLK)     : PLL1 -> divider 2 -> 400 MHz (sysb_ck)
     - IC6 (NPU SYSCLK)     : PLL1 -> divider 2 -> 400 MHz (sysc_ck)
     - IC11 (AXISRAM SYSCLK): PLL1 -> divider 2 -> 400 MHz (sysd_ck)

     Bus clocks
     - HCLK  = IC2 / 2  = 400 / 2 = 200 MHz
     - PCLK1 = HCLK / 1 = 200 MHz
     - PCLK2 = HCLK / 1 = 200 MHz
     - PCLK4 = HCLK / 1 = 200 MHz
     - PCLK5 = HCLK / 1 = 200 MHz
  */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_CPUCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK  | \
                                 RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK4  | RCC_CLOCKTYPE_PCLK5);

  RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_IC1;
  RCC_ClkInitStruct.IC1Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC1Selection.ClockDivider   = 1;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_IC2_IC6_IC11;

  RCC_ClkInitStruct.IC2Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC2Selection.ClockDivider   = 2;

  RCC_ClkInitStruct.IC6Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC6Selection.ClockDivider   = 2;

  RCC_ClkInitStruct.IC11Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC11Selection.ClockDivider   = 2;

  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;
  RCC_ClkInitStruct.APB5CLKDivider = RCC_APB5_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* SDMMC2 peripheral clock:
     - Source: IC4 from PLL1
     - IC4 divider = 4 -> 800 / 4 = 200 MHz */
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /* SDMMC2 clock from IC4 = PLL1 / 4 = 200MHz (200000000 Hz) */
  PeriphClkInit.PeriphClockSelection   = RCC_PERIPHCLK_SDMMC2;
  PeriphClkInit.Sdmmc2ClockSelection   = RCC_SDMMC2CLKSOURCE_IC4;
  PeriphClkInit.ICSelection[RCC_IC4].ClockSelection = RCC_ICCLKSOURCE_PLL1;
  PeriphClkInit.ICSelection[RCC_IC4].ClockDivider   = 4;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    while (1);
  }
}

/**
 * @brief   Configure DCMIPP kernel clock:
 * - Source: IC17 from PLL2 (1000 MHz)
 * - IC17 divider = 3 -> 1000 / 3 ≈ 333 MHz (ck_ker_dcmipp)
 * Also configure IC18 from PLL1:
 * - IC18 divider = 40 -> 800 / 40 = 20 MHz
 */
HAL_StatusTypeDef MX_DCMIPP_ClockConfig(DCMIPP_HandleTypeDef *hdcmipp)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_DCMIPP;
  PeriphClkInit.DcmippClockSelection = RCC_DCMIPPCLKSOURCE_IC17;
  PeriphClkInit.ICSelection[RCC_IC17].ClockSelection = RCC_ICCLKSOURCE_PLL2;
  PeriphClkInit.ICSelection[RCC_IC17].ClockDivider   = 3;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    return HAL_ERROR;
  }

  LL_RCC_IC18_SetSource(LL_RCC_ICCLKSOURCE_PLL1);
  LL_RCC_IC18_SetDivider(40);   /* 800 / 40 = 20 MHz */
  LL_RCC_IC18_Enable();

  return HAL_OK;
}

