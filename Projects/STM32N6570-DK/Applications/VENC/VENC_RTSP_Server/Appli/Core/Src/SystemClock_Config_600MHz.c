#include "main.h"
#include "stm32n6570_discovery_audio.h"
#include "stm32n6570_discovery_bus.h"

/**
* @brief  System Clock Configuration
*         The system Clock is configured as follow :
*            CPU Clock source               = IC1 (from PLL1)
*            CPUCLK(Hz)                     = 600000000
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
  /* In debug mode, clock may be unconfigured on app entry. Configure it here. */
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Configure the system Power Supply */
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
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_CPUCLK | RCC_CLOCKTYPE_SYSCLK);
    RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_HSI;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /* HSI selected as PLL sources
     PLL1: ((HSI / M) * N) / P1 = ((64MHz / 4) * 75) / 1 = 1200MHz
           IC1 divider = 2  -> CPUCLK = 600MHz
           IC2 divider = 3  -> SYSCLK AXI = 400MHz
           IC6 divider = 2  -> SYSCLK NPU = 600MHz
           IC11 divider = 2 -> SYSCLK AXISRAM3/4/5/6 = 600MHz
     PLL2: ((64MHz / 8) * 125) / P1 = 1000MHz (when selected)
     PLL3: BYPASS (HSI path)
     PLL4: ((64MHz / 32) * 40) / P1 = 80MHz */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL1.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL1.PLLM = 4;
  RCC_OscInitStruct.PLL1.PLLN = 75;
  RCC_OscInitStruct.PLL1.PLLP1 = 1;
  RCC_OscInitStruct.PLL1.PLLP2 = 1;
  RCC_OscInitStruct.PLL1.PLLFractional = 0;

  /* PLL2: 64MHz x 125 / 8 = 1000MHz */
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL2.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL2.PLLM = 8;
  RCC_OscInitStruct.PLL2.PLLFractional = 0;
  RCC_OscInitStruct.PLL2.PLLN = 125;
  RCC_OscInitStruct.PLL2.PLLP1 = 1;
  RCC_OscInitStruct.PLL2.PLLP2 = 1;

  /* PLL3: bypass */
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_BYPASS;
  RCC_OscInitStruct.PLL3.PLLSource = RCC_PLLSOURCE_HSI;

  /* PLL4:bypass */
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_BYPASS;
  RCC_OscInitStruct.PLL4.PLLSource = RCC_PLLSOURCE_HSI;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Interconnect clock routing
     - IC1 (CPUCLK source)  : PLL1 -> divider 2 -> 600 MHz (sysa_ck)
     - IC2 (AXI SYSCLK)     : PLL1 -> divider 3 -> 400 MHz (sysb_ck)
     - IC6 (NPU SYSCLK)     : PLL1 -> divider 2 -> 600 MHz (sysc_ck)
     - IC11 (AXISRAM SYSCLK): PLL1 -> divider 2 -> 600 MHz (sysd_ck)

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
  RCC_ClkInitStruct.IC1Selection.ClockDivider = 2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_IC2_IC6_IC11;
  RCC_ClkInitStruct.IC2Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC2Selection.ClockDivider = 3;
  RCC_ClkInitStruct.IC6Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC6Selection.ClockDivider = 2;
  RCC_ClkInitStruct.IC11Selection.ClockSelection = RCC_ICCLKSOURCE_PLL1;
  RCC_ClkInitStruct.IC11Selection.ClockDivider = 2;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
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
     - IC4 divider = 6 -> 1200 / 6 = 200 MHz */
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /* SDMMC2 clock from IC4 = PLL1 / 6 = 200MHz (200000000 Hz) */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SDMMC2;
  PeriphClkInit.Sdmmc2ClockSelection = RCC_SDMMC2CLKSOURCE_IC4;
  PeriphClkInit.ICSelection[RCC_IC4].ClockSelection = RCC_ICCLKSOURCE_PLL1;
  PeriphClkInit.ICSelection[RCC_IC4].ClockDivider = 6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    while (1);
  }
}

/**
 * @brief  Configure DCMIPP kernel clock:
 * - Source: IC17 from PLL1 (1200 MHz)
 * - IC17 divider = 4 -> 1200 / 4 = 300 MHz (ck_ker_dcmipp)
 * Also configure IC18 from PLL1:
 * - IC18 divider = 60 -> 1200 / 60 = 20 MHz
 */
HAL_StatusTypeDef MX_DCMIPP_ClockConfig(DCMIPP_HandleTypeDef *hdcmipp)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /* DCMIPP kernel clock: IC17 = PLL1 / 4 = 300MHz (300000000 Hz) */
  PeriphClkInit.PeriphClockSelection |= RCC_PERIPHCLK_DCMIPP;
  PeriphClkInit.DcmippClockSelection = RCC_DCMIPPCLKSOURCE_IC17;
  PeriphClkInit.ICSelection[RCC_IC17].ClockSelection = RCC_ICCLKSOURCE_PLL1;
  PeriphClkInit.ICSelection[RCC_IC17].ClockDivider = 4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* DCMIPP auxiliary clock: IC18 = PLL1 / 60 = 20MHz (20000000 Hz) */
  LL_RCC_IC18_SetSource(LL_RCC_ICCLKSOURCE_PLL1);
  LL_RCC_IC18_SetDivider(60);
  LL_RCC_IC18_Enable();

  return HAL_OK;
}

/**
 * @brief  Configure MDF1 clock according to sample rate.
 * @param  hmdf       MDF handle (unused here, kept for API compatibility)
 * @param  SampleRate Audio sample rate used to record the audio stream.
 * @retval HAL_StatusTypeDef HAL_OK on success, HAL_ERROR on failure
 */
HAL_StatusTypeDef MX_MDF1_ClockConfig(MDF_HandleTypeDef *hmdf, uint32_t SampleRate)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hmdf);

    HAL_StatusTypeDef        status = HAL_OK;
    RCC_OscInitTypeDef       RCC_OscInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /* Default: no oscillator change except PLL3 used for MDF1 when required */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
    RCC_OscInitStruct.PLL1.PLLState  = RCC_PLL_NONE;
    RCC_OscInitStruct.PLL2.PLLState  = RCC_PLL_NONE;

    /* Configure PLL3 as MDF1 clock source (HSI-based) */
    RCC_OscInitStruct.PLL3.PLLState      = RCC_PLL_ON;
    RCC_OscInitStruct.PLL3.PLLSource     = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL3.PLLFractional = 0U;
    RCC_OscInitStruct.PLL3.PLLM         = 8U; /* 64/8 = 8 MHz */

    if ((SampleRate == AUDIO_FREQUENCY_11K) ||
        (SampleRate == AUDIO_FREQUENCY_22K) ||
        (SampleRate == AUDIO_FREQUENCY_44K))
    {
        /* For 11/22/44 kHz use high PLL multiplier and dividers */
        RCC_OscInitStruct.PLL3.PLLN   = 192U; /* 8 * 192 = 1536 MHz */
        RCC_OscInitStruct.PLL3.PLLP1  = 4U;
        RCC_OscInitStruct.PLL3.PLLP2  = 2U;   /* 1536/8 = 192 MHz */
    }
    else if (SampleRate == AUDIO_FREQUENCY_96K)
    {
        /* For 96 kHz */
        RCC_OscInitStruct.PLL3.PLLN   = 172U; /* 8 * 172 = 1376 MHz */
        RCC_OscInitStruct.PLL3.PLLP1  = 2U;
        RCC_OscInitStruct.PLL3.PLLP2  = 1U;   /* 1376/2 = 688 MHz */
    }
    else
    {
        /* For 8/16/32/48 kHz */
        RCC_OscInitStruct.PLL3.PLLN   = 172U; /* 8 * 172 = 1376 MHz */
        RCC_OscInitStruct.PLL3.PLLP1  = 7U;
        RCC_OscInitStruct.PLL3.PLLP2  = 4U;   /* 1376/28 ˜ 49.14 MHz */
    }

    RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_NONE;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        status = HAL_ERROR;
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_MDF1;
    PeriphClkInitStruct.Mdf1ClockSelection   = RCC_MDF1CLKSOURCE_IC8;
    PeriphClkInitStruct.ICSelection[RCC_IC8].ClockSelection = RCC_ICCLKSOURCE_PLL3;

    if ((SampleRate == AUDIO_FREQUENCY_11K) ||
        (SampleRate == AUDIO_FREQUENCY_22K) ||
        (SampleRate == AUDIO_FREQUENCY_44K))
    {
        PeriphClkInitStruct.ICSelection[RCC_IC8].ClockDivider = 17U;
    }
    else if (SampleRate == AUDIO_FREQUENCY_96K)
    {
        PeriphClkInitStruct.ICSelection[RCC_IC8].ClockDivider = 7U;
    }
    else /* 8K, 16K, 32K, 48K */
    {
        PeriphClkInitStruct.ICSelection[RCC_IC8].ClockDivider = 1U;
    }

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @brief  Configure SAI1 clock according to sample rate.
 * @note   The SAI1 clock configuration uses HSI as the base clock in this platform.
 * @param  hsai       SAI handle (unused here, kept for API compatibility)
 * @param  SampleRate Audio sample rate used to play the audio stream.
 * @retval HAL_StatusTypeDef HAL_OK on success, HAL_ERROR on failure
 */
HAL_StatusTypeDef MX_SAI1_ClockConfig(SAI_HandleTypeDef *hsai, uint32_t SampleRate)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsai);

    HAL_StatusTypeDef        ret = HAL_OK;
    RCC_OscInitTypeDef       RCC_OscInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /* Assume HSI is running; configure PLL4 when needed */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
    RCC_OscInitStruct.PLL1.PLLState  = RCC_PLL_NONE;
    RCC_OscInitStruct.PLL2.PLLState  = RCC_PLL_NONE;
    RCC_OscInitStruct.PLL3.PLLState  = RCC_PLL_NONE;

    RCC_OscInitStruct.PLL4.PLLState      = RCC_PLL_ON;
    RCC_OscInitStruct.PLL4.PLLSource     = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL4.PLLFractional = 0U;
    RCC_OscInitStruct.PLL4.PLLM          = 8U; /* 64/8 = 8 MHz */

    if ((SampleRate == AUDIO_FREQUENCY_11K) ||
        (SampleRate == AUDIO_FREQUENCY_22K) ||
        (SampleRate == AUDIO_FREQUENCY_44K))
    {
        RCC_OscInitStruct.PLL4.PLLN  = 192U; /* 8 * 192 = 1536 MHz */
        RCC_OscInitStruct.PLL4.PLLP1 = 4U;
        RCC_OscInitStruct.PLL4.PLLP2 = 2U;   /* 1536/8 = 192 MHz */
    }
    else
    {
        /* 8K, 16K, 24K, 32K, 48K or 96K */
        RCC_OscInitStruct.PLL4.PLLN  = 172U; /* 8 * 172 = 1376 MHz */
        RCC_OscInitStruct.PLL4.PLLP1 = 7U;
        RCC_OscInitStruct.PLL4.PLLP2 = 4U;   /* 1376/28 ˜ 49.14 MHz */
    }

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        ret = HAL_ERROR;
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
    PeriphClkInitStruct.Sai1ClockSelection   = RCC_SAI1CLKSOURCE_IC7;
    PeriphClkInitStruct.ICSelection[RCC_IC7].ClockSelection = RCC_ICCLKSOURCE_PLL4;

    if ((SampleRate == AUDIO_FREQUENCY_11K) ||
        (SampleRate == AUDIO_FREQUENCY_22K) ||
        (SampleRate == AUDIO_FREQUENCY_44K))
    {
        PeriphClkInitStruct.ICSelection[RCC_IC7].ClockDivider = 17U;
    }
    else
    {
        PeriphClkInitStruct.ICSelection[RCC_IC7].ClockDivider = 1U;
    }

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        ret = HAL_ERROR;
    }

    return ret;
}
