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
extern DMA_NodeTypeDef Node_GPDMA1_Channel6;

extern DMA_QListTypeDef List_GPDMA1_Channel6;

extern DMA_HandleTypeDef handle_GPDMA1_Channel6;

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
extern DMA_NodeTypeDef pNode_GPDMACH0;

extern DMA_QListTypeDef pQueueLinkList_GPDMACH0;

extern DMA_HandleTypeDef handle_GPDMA1_Channel0;
/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
  * @brief MDF MSP Initialization
  * This function configures the hardware resources used in this example
  * @param hmdf: MDF handle pointer
  * @retval None
  */
void HAL_MDF_MspInit(MDF_HandleTypeDef* hmdf)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  DMA_NodeConfTypeDef NodeConfig;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(IS_MDF_INSTANCE(hmdf->Instance))
  {
    /* USER CODE BEGIN MDF1_MspInit 0 */

    /* USER CODE END MDF1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_MDF1;
    PeriphClkInitStruct.Mdf1ClockSelection = RCC_MDF1CLKSOURCE_IC8;
    PeriphClkInitStruct.ICSelection[RCC_IC8].ClockSelection = RCC_ICCLKSOURCE_PLL3;
    PeriphClkInitStruct.ICSelection[RCC_IC8].ClockDivider = 1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_MDF1_CLK_ENABLE();

    __HAL_RCC_GPIOE_CLK_ENABLE();
    /**MDF1 GPIO Configuration
    PE8     ------> MDF1_SDI0
    PE2     ------> MDF1_CCK0
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_MDF1;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* MDF1 DMA Init */
    /* GPDMA1_REQUEST_MDF1_FLT0 Init */
    NodeConfig.NodeType = DMA_GPDMA_LINEAR_NODE;
    NodeConfig.Init.Request = GPDMA1_REQUEST_MDF1_FLT0;
    NodeConfig.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
    NodeConfig.Init.Direction = DMA_PERIPH_TO_MEMORY;
    NodeConfig.Init.SrcInc = DMA_SINC_FIXED;
    NodeConfig.Init.DestInc = DMA_DINC_INCREMENTED;
    NodeConfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_HALFWORD;
    NodeConfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
    NodeConfig.Init.SrcBurstLength = 1;
    NodeConfig.Init.DestBurstLength = 1;
    NodeConfig.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT1;
    NodeConfig.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    NodeConfig.Init.Mode = DMA_NORMAL;
    NodeConfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;
    NodeConfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
    NodeConfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
    NodeConfig.SrcSecure = DMA_CHANNEL_SRC_SEC;
    NodeConfig.DestSecure = DMA_CHANNEL_DEST_SEC;
    if (HAL_DMAEx_List_BuildNode(&NodeConfig, &Node_GPDMA1_Channel6) != HAL_OK)
    {
      Error_Handler();
    }

    if (HAL_DMAEx_List_InsertNode(&List_GPDMA1_Channel6, NULL, &Node_GPDMA1_Channel6) != HAL_OK)
    {
      Error_Handler();
    }

    if (HAL_DMAEx_List_SetCircularMode(&List_GPDMA1_Channel6) != HAL_OK)
    {
      Error_Handler();
    }

    handle_GPDMA1_Channel6.Instance = GPDMA1_Channel6;
    handle_GPDMA1_Channel6.InitLinkedList.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
    handle_GPDMA1_Channel6.InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
    handle_GPDMA1_Channel6.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT0;
    handle_GPDMA1_Channel6.InitLinkedList.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    handle_GPDMA1_Channel6.InitLinkedList.LinkedListMode = DMA_LINKEDLIST_CIRCULAR;
    if (HAL_DMAEx_List_Init(&handle_GPDMA1_Channel6) != HAL_OK)
    {
      Error_Handler();
    }

    if (HAL_DMAEx_List_LinkQ(&handle_GPDMA1_Channel6, &List_GPDMA1_Channel6) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hmdf, hdma, handle_GPDMA1_Channel6);

    /* MDF1 interrupt Init */
    HAL_NVIC_SetPriority(MDF1_FLT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(MDF1_FLT0_IRQn);
    /* USER CODE BEGIN MDF1_MspInit 1 */

    /* USER CODE END MDF1_MspInit 1 */

  }

}

/**
  * @brief MDF MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param hmdf: MDF handle pointer
  * @retval None
  */
void HAL_MDF_MspDeInit(MDF_HandleTypeDef* hmdf)
{
  if(IS_MDF_INSTANCE(hmdf->Instance))
  {
    /* USER CODE BEGIN MDF1_MspDeInit 0 */

    /* USER CODE END MDF1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_MDF1_CLK_DISABLE();

    /**MDF1 GPIO Configuration
    PE8     ------> MDF1_SDI0
    PE2     ------> MDF1_CCK0
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_8|GPIO_PIN_2);

    /* MDF1 DMA DeInit */
    HAL_DMA_DeInit(hmdf->hdma);

    /* MDF1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(MDF1_FLT0_IRQn);
    /* USER CODE BEGIN MDF1_MspDeInit 1 */

    /* USER CODE END MDF1_MspDeInit 1 */
  }

}

/* USER CODE BEGIN 1 */
/**
* @brief SAI MSP Initialization
* This function configures the hardware resources used in this example
* @param hsai: SAI handle pointer
* @retval None
*/
void HAL_SAI_MspInit(SAI_HandleTypeDef* hsai)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  DMA_NodeConfTypeDef nodeconfig;
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  if (hsai->Instance == SAI1_Block_A)
  {
    /* Peripheral clock enable */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
    RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL2.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL2.PLLM = 8;
    RCC_OscInitStruct.PLL2.PLLN = 172;
    RCC_OscInitStruct.PLL2.PLLFractional = 0;
    RCC_OscInitStruct.PLL2.PLLP1 = 7;
    RCC_OscInitStruct.PLL2.PLLP2 = 4;
    RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
    PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_IC7;
    PeriphClkInitStruct.ICSelection[RCC_IC7].ClockSelection = RCC_ICCLKSOURCE_PLL2;
    PeriphClkInitStruct.ICSelection[RCC_IC7].ClockDivider = 1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_RCC_SAI1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    /**SAI1_A_Block_A GPIO Configuration
    PB7     ------> SAI1_SD_A
    PB6     ------> SAI1_SCK_A
    PB0     ------> SAI1_FS_A
    PG7     ------> SAI1_MCLK_A
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_SAI1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_SAI1;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* Peripheral DMA init*/

    nodeconfig.NodeType = DMA_GPDMA_LINEAR_NODE;
    nodeconfig.Init.Request = GPDMA1_REQUEST_SAI1_A;
    nodeconfig.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
    nodeconfig.Init.Direction = DMA_MEMORY_TO_PERIPH;
    nodeconfig.Init.SrcInc = DMA_SINC_INCREMENTED;
    nodeconfig.Init.DestInc = DMA_DINC_FIXED;
    nodeconfig.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_HALFWORD;
    nodeconfig.Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
    nodeconfig.Init.SrcBurstLength = 1;
    nodeconfig.Init.DestBurstLength = 1;
    nodeconfig.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0|DMA_DEST_ALLOCATED_PORT1;
    nodeconfig.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    nodeconfig.Init.Mode = DMA_NORMAL;
    nodeconfig.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;
    nodeconfig.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;
    nodeconfig.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;
    nodeconfig.SrcSecure                           = DMA_CHANNEL_SRC_SEC;
    nodeconfig.DestSecure                          = DMA_CHANNEL_DEST_SEC;
    if (HAL_DMAEx_List_BuildNode(&nodeconfig, &pNode_GPDMACH0) != HAL_OK)
    {
      Error_Handler();
    }

    if (HAL_DMAEx_List_InsertNode(&pQueueLinkList_GPDMACH0, NULL, &pNode_GPDMACH0) != HAL_OK)
    {
      Error_Handler();
    }

    if (HAL_DMAEx_List_SetCircularMode(&pQueueLinkList_GPDMACH0) != HAL_OK)
    {
      Error_Handler();
    }

    handle_GPDMA1_Channel0.Instance = GPDMA1_Channel0;
    handle_GPDMA1_Channel0.InitLinkedList.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
    handle_GPDMA1_Channel0.InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
    handle_GPDMA1_Channel0.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT0;
    handle_GPDMA1_Channel0.InitLinkedList.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    handle_GPDMA1_Channel0.InitLinkedList.LinkedListMode = DMA_LINKEDLIST_CIRCULAR;
    if (HAL_DMAEx_List_Init(&handle_GPDMA1_Channel0) != HAL_OK)
    {
      Error_Handler();
    }

    if (HAL_DMAEx_List_LinkQ(&handle_GPDMA1_Channel0, &pQueueLinkList_GPDMACH0) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hsai, hdmatx, handle_GPDMA1_Channel0);

    if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel0, DMA_CHANNEL_PRIV|DMA_CHANNEL_SEC) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(SAI1_A_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SAI1_A_IRQn);
    HAL_NVIC_SetPriority(GPDMA1_Channel0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel0_IRQn);
  }
}

/**
* @brief SAI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hsai: SAI handle pointer
* @retval None
*/
void HAL_SAI_MspDeInit(SAI_HandleTypeDef* hsai)
{
  if(hsai->Instance==SAI1_Block_A)
  {
    /* Peripheral clock disable */
    __HAL_RCC_SAI1_CLK_DISABLE();

    /**SAI1_A_Block_A GPIO Configuration
    PB7     ------> SAI1_SD_A
    PB6     ------> SAI1_SCK_A
    PB0     ------> SAI1_FS_A
    PG7     ------> SAI1_MCLK_A
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0|GPIO_PIN_6|GPIO_PIN_7);
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_7);

    /* SAI1_Block_A DMA DeInit */
    HAL_DMA_DeInit(hsai->hdmatx);

    /* SAI1_Block_A interrupt DeInit */
    HAL_NVIC_DisableIRQ(SAI1_A_IRQn);
    HAL_NVIC_DisableIRQ(GPDMA1_Channel0_IRQn);
  }
}

/* USER CODE END 1 */
