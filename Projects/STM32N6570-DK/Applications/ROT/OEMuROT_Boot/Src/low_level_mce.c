/**
  ******************************************************************************
  * @file    low_level_mce.c
  * @author  MCD Application Team
  * @brief   Low Level Interface module to use STM32 MCE Ip
  *
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
#include "low_level_rng.h"
#include "low_level_mce.h"
#include "boot_hal_flowcontrol.h"

/* Private defines -----------------------------------------------------------*/
#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

typedef struct
{
  MCE_RegionConfigTypeDef MCE_RegionConfig;
#ifdef FLOW_CONTROL
  uint32_t flow_step_enable;
  uint32_t flow_ctrl_enable;
  uint32_t flow_step_check;
  uint32_t flow_ctrl_check;
#endif
}MCE_RegionCfg_t;

/* Private variables ---------------------------------------------------------*/
static MCE_HandleTypeDef hmce_flash = {.Instance = MCE2};
#if (OEMUROT_LOAD_AND_RUN == LOAD_AND_RUN_EXT_RAM)
static MCE_HandleTypeDef hmce_ram = {.Instance = MCE1};
#endif /* OEMUROT_LOAD_AND_RUN == LOAD_AND_RUN_EXT_RAM */

static MCE_RegionCfg_t MCE_Flash_aRegion[] =
{
  /* Primary slots */
  {
    {
      .ContextID = MCE_CONTEXT1,
      .StartAddress = FLASH_PRIMARY_AREA_START_ADDRESS,
      .EndAddress = FLASH_PRIMARY_AREA_LIMIT_ADDRESS,
      .Mode = MCE_STREAM_CIPHER,
    },
#ifdef FLOW_CONTROL
    FLOW_STEP_MCE1_EN_R1,
    FLOW_CTRL_MCE1_EN_R1,
    FLOW_STEP_MCE1_CH_R1,
    FLOW_CTRL_MCE1_CH_R1,
#endif /* FLOW_CONTROL */
  },
#if defined(MCUBOOT_USE_HASH_REF)
  /* Hash ref area */
  {
    {
      .ContextID = MCE_CONTEXT1,
      .StartAddress = FLASH_AREA_HASH_REF_START,
      .EndAddress = FLASH_AREA_HASH_REF_LIMIT,
      .Mode = MCE_STREAM_CIPHER,
    },
#ifdef FLOW_CONTROL
    FLOW_STEP_MCE1_EN_R2,
    FLOW_CTRL_MCE1_EN_R2,
    FLOW_STEP_MCE1_CH_R2,
    FLOW_CTRL_MCE1_CH_R2,
#endif /* FLOW_CONTROL */
  },
#endif /* MCUBOOT_USE_HASH_REF */
#if !defined(MCUBOOT_OVERWRITE_ONLY)
  /* Scratch area */
  {
    {
      .ContextID = MCE_CONTEXT1,
      .StartAddress = FLASH_AREA_SCRATCH_START,
      .EndAddress = FLASH_AREA_SCRATCH_LIMIT,
      .Mode = MCE_STREAM_CIPHER,
    },
#ifdef FLOW_CONTROL
    FLOW_STEP_MCE1_EN_R3,
    FLOW_CTRL_MCE1_EN_R3,
    FLOW_STEP_MCE1_CH_R3,
    FLOW_CTRL_MCE1_CH_R3,
#endif /* FLOW_CONTROL */
  },
#endif /* not MCUBOOT_OVERWRITE_ONLY */
};

#if (OEMUROT_LOAD_AND_RUN == LOAD_AND_RUN_EXT_RAM)
static MCE_RegionCfg_t MCE_RAM_aRegion[] =
{
  /* RAM */
  {
    {
      .ContextID = MCE_CONTEXT2,
      .StartAddress = 0x90000000,
      .EndAddress = 0x91FFFFFF,
      .Mode = MCE_STREAM_CIPHER,
    },
#ifdef FLOW_CONTROL
    FLOW_STEP_MCE2_EN_R1,
    FLOW_CTRL_MCE2_EN_R1,
    FLOW_STEP_MCE2_CH_R1,
    FLOW_CTRL_MCE2_CH_R1,
#endif /* FLOW_CONTROL */
  }
};
#endif /* OEMUROT_LOAD_AND_RUN == LOAD_AND_RUN_EXT_RAM */

static MCE_AESConfigTypeDef MCE_Flash_AESConfig =
{
  .Nonce = {0xA3BB3AAB, 0x8A87A1A8},
  .Version = 0x0123,
  .pKey = NULL,
  .KeySize = MCE_AES_256,
  .Cipher_Mode = MCE_CONTEXT_STREAM_CIPHER
};

#if (OEMUROT_LOAD_AND_RUN == LOAD_AND_RUN_EXT_RAM)
static MCE_AESConfigTypeDef MCE_RAM_AESConfig =
{
  .Nonce = {0xFF0335AC, 0x751BA2F7},
  .Version = 0x1234,
  .pKey = NULL,
  .KeySize = MCE_AES_256,
  .Cipher_Mode = MCE_CONTEXT_STREAM_CIPHER
};
#endif /* OEMUROT_LOAD_AND_RUN == LOAD_AND_RUN_EXT_RAM */

/* Private function prototypes -----------------------------------------------*/
static void Generate_Keys(uint32_t *p_derived_key, uint32_t *p_random_key);
static int8_t MCE_CheckRegionConfig(MCE_HandleTypeDef *hmce, uint32_t RegionIndex,
                                       const MCE_RegionConfigTypeDef *pConfig);
static int8_t MCE_CheckRegionAESContext(MCE_HandleTypeDef *hmce, uint32_t ContextIndex, uint32_t RegionIndex);
static int8_t MCE_CheckAESContext(MCE_HandleTypeDef *hmce, const MCE_AESConfigTypeDef  *AESConfig,
                                           uint32_t ContextIndex);
static HAL_StatusTypeDef MCE_GetLockGlobalConfig(MCE_HandleTypeDef *hmce);
static HAL_StatusTypeDef MCE_GetLockAESContextConfig(MCE_HandleTypeDef *hmce, uint32_t ContextIndex);
static HAL_StatusTypeDef MCE_GetLockAESContextKey(MCE_HandleTypeDef *hmce, uint32_t ContextIndex);

/* Functions Definition ------------------------------------------------------*/
/**
  * @brief  Configure MCE configuration
  * @retval None
  */
void mce_init_cfg(void)
{
  uint32_t i = 0;

  /* Configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    uint32_t a_dkey[8U] = {0U}, a_rkey[8U] = {0U};

    /* Generate keys */
    Generate_Keys(a_dkey, a_rkey);

    /* Initialize MCE1 */
    if (HAL_MCE_Init(&hmce_flash) != HAL_OK)
    {
      Error_Handler();
    }

    /* Configure regions */
    for (i = 0; i < ARRAY_SIZE(MCE_Flash_aRegion); i++)
    {
      if (HAL_MCE_ConfigRegion(&hmce_flash, i, &MCE_Flash_aRegion[i].MCE_RegionConfig) != HAL_OK)
      {
        Error_Handler();
      }
      else
      {
        FLOW_CONTROL_STEP(uFlowProtectValue, MCE_Flash_aRegion[i].flow_step_enable,
                                             MCE_Flash_aRegion[i].flow_ctrl_enable);
      }
    }

    /* Set region context */
    if (HAL_MCE_SetRegionAESContext(&hmce_flash, MCE_CONTEXT1, MCE_REGION1) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE1_AES_CTX_EN,
                                           FLOW_CTRL_MCE1_AES_CTX_EN);
    }

    /* Configure AES context */
    MCE_Flash_AESConfig.pKey = a_dkey;
    if (HAL_MCE_ConfigAESContext(&hmce_flash, &MCE_Flash_AESConfig, MCE_CONTEXT1) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE1_AES_CFG_EN,
                                           FLOW_CTRL_MCE1_AES_CFG_EN);
    }

    /* Lock AES context key */
    if (HAL_MCE_LockAESContextKey(&hmce_flash, MCE_CONTEXT1) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE1_AES_KEY_LOCK,
                                           FLOW_CTRL_MCE1_AES_KEY_LOCK);
    }

    /* Enable AES context */
    if (HAL_MCE_EnableAESContext(&hmce_flash, MCE_CONTEXT1) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE1_AES_EN,
                                           FLOW_CTRL_MCE1_AES_EN);
    }

    /* Lock AES context configuration */
    if (HAL_MCE_LockAESContextConfig(&hmce_flash, MCE_CONTEXT1) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE1_AES_CFG_LOCK,
                                           FLOW_CTRL_MCE1_AES_CFG_LOCK);
    }
#if (OEMUROT_LOAD_AND_RUN == LOAD_AND_RUN_EXT_RAM)
    /* Initialize MCE2 */
    if (HAL_MCE_Init(&hmce_ram) != HAL_OK)
    {
      Error_Handler();
    }

    /* Configure regions */
    for (i = 0; i < ARRAY_SIZE(MCE_RAM_aRegion); i++)
    {
      if (HAL_MCE_ConfigRegion(&hmce_ram, i, &MCE_RAM_aRegion[i].MCE_RegionConfig) != HAL_OK)
      {
        Error_Handler();
      }
      else
      {
        FLOW_CONTROL_STEP(uFlowProtectValue, MCE_RAM_aRegion[i].flow_step_enable,
                                             MCE_RAM_aRegion[i].flow_ctrl_enable);
      }
    }

    if (HAL_MCE_SetRegionAESContext(&hmce_ram, MCE_CONTEXT2, MCE_REGION1) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE2_AES_CTX_EN,
                                           FLOW_CTRL_MCE2_AES_CTX_EN);
    }

    /* Configure AES context */
    MCE_RAM_AESConfig.pKey = a_rkey;
    if (HAL_MCE_ConfigAESContext(&hmce_ram, &MCE_RAM_AESConfig, MCE_CONTEXT2) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE2_AES_CFG_EN,
                                           FLOW_CTRL_MCE2_AES_CFG_EN);
    }

    /* Lock AES context key */
    if (HAL_MCE_LockAESContextKey(&hmce_ram, MCE_CONTEXT2) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE2_AES_KEY_LOCK,
                                           FLOW_CTRL_MCE2_AES_KEY_LOCK);
    }

    /* Enable AES context */
    if (HAL_MCE_EnableAESContext(&hmce_ram, MCE_CONTEXT2) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE2_AES_EN,
                                           FLOW_CTRL_MCE2_AES_EN);
    }

    /* Lock AES context configuration */
    if (HAL_MCE_LockAESContextConfig(&hmce_ram, MCE_CONTEXT2) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE2_AES_CFG_LOCK,
                                           FLOW_CTRL_MCE2_AES_CFG_LOCK);
    }

    /* Lock global configuration */
    HAL_MCE_LockGlobalConfig(&hmce_ram);

    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE2_AES_LOCK,
                                         FLOW_CTRL_MCE2_AES_LOCK);
#endif /* OEMUROT_LOAD_AND_RUN == LOAD_AND_RUN_EXT_RAM */
  }
  /* Verification stage */
  else
  {
    /* Check regions configuration */
    for (i = 0; i < ARRAY_SIZE(MCE_Flash_aRegion); i++)
    {
      if (MCE_CheckRegionConfig(&hmce_flash, i, &MCE_Flash_aRegion[i].MCE_RegionConfig) != 0)
      {
        Error_Handler();
      }
      else
      {
        FLOW_CONTROL_STEP(uFlowProtectValue, MCE_Flash_aRegion[i].flow_step_check,
                                             MCE_Flash_aRegion[i].flow_ctrl_check);
      }
    }

    /* Check region AES context */
    if (MCE_CheckRegionAESContext(&hmce_flash, MCE_CONTEXT1, MCE_REGION1) != 0)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE1_AES_CTX_CH,
                                           FLOW_CTRL_MCE1_AES_CTX_CH);
    }

    /* Check AES context */
    if (MCE_CheckAESContext(&hmce_flash, &MCE_Flash_AESConfig, MCE_CONTEXT1) != 0)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE1_AES_CFG_CH,
                                           FLOW_CTRL_MCE1_AES_CFG_CH);
    }

    /* Check AES context key lock state */
    if (MCE_GetLockAESContextKey(&hmce_flash, MCE_CONTEXT1) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE1_AES_KEY_LOCK_CH,
                                           FLOW_CTRL_MCE1_AES_KEY_LOCK_CH);
    }

    /* Check AES context configuration lock state */
    if (MCE_GetLockAESContextConfig(&hmce_flash, MCE_CONTEXT1) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE1_AES_CFG_LOCK_CH,
                                           FLOW_CTRL_MCE1_AES_CFG_LOCK_CH);
    }

#if (OEMUROT_LOAD_AND_RUN == LOAD_AND_RUN_EXT_RAM)
    /* Check regions configuration */
    for (i = 0; i < ARRAY_SIZE(MCE_RAM_aRegion); i++)
    {
      if (MCE_CheckRegionConfig(&hmce_ram, i, &MCE_RAM_aRegion[i].MCE_RegionConfig) != 0)
      {
        Error_Handler();
      }
      else
      {
        FLOW_CONTROL_STEP(uFlowProtectValue, MCE_RAM_aRegion[i].flow_step_check,
                                             MCE_RAM_aRegion[i].flow_ctrl_check);
      }
    }

    /* Check region AES context */
    if (MCE_CheckRegionAESContext(&hmce_ram, MCE_CONTEXT2, MCE_REGION1) != 0)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE2_AES_CTX_CH,
                                           FLOW_CTRL_MCE2_AES_CTX_CH);
    }

    /* Check AES context */
    if (MCE_CheckAESContext(&hmce_ram, &MCE_RAM_AESConfig, MCE_CONTEXT2) != 0)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE2_AES_CFG_CH,
                                           FLOW_CTRL_MCE2_AES_CFG_CH);
    }

    /* Check AES context key lock state */
    if (MCE_GetLockAESContextKey(&hmce_ram, MCE_CONTEXT2) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE2_AES_KEY_LOCK_CH,
                                           FLOW_CTRL_MCE2_AES_KEY_LOCK_CH);
    }

    /* Check AES context configuration lock state */
    if (MCE_GetLockAESContextConfig(&hmce_ram, MCE_CONTEXT2) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE2_AES_CFG_LOCK_CH,
                                           FLOW_CTRL_MCE2_AES_CFG_LOCK_CH);
    }

    /* Check MCE AES lock state */
    if (MCE_GetLockGlobalConfig(&hmce_ram) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE2_AES_LOCK_CH,
                                           FLOW_CTRL_MCE2_AES_LOCK_CH);
    }
#endif /* OEMUROT_LOAD_AND_RUN == LOAD_AND_RUN_EXT_RAM */
  }
}

/**
  * @brief  Lock external flash MCE configuration
  * @retval None
  */
void mce_lock_cfg(void)
{
  /* Configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* Lock global configuration */
    HAL_MCE_LockGlobalConfig(&hmce_flash);

    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE1_AES_LOCK,
                                         FLOW_CTRL_MCE1_AES_LOCK);
  }
  /* Verification stage */
  else
  {
    /* Check MCE AES lock state */
    if (MCE_GetLockGlobalConfig(&hmce_flash) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MCE1_AES_LOCK_CH,
                                           FLOW_CTRL_MCE1_AES_LOCK_CH);
    }
  }
}

/**
  * @brief  Check the status of the code primary region
  * @retval true if enabled, false otherwise.
  */
bool LL_MCE_IsCodePrimaryRegionEnabled(void)
{
  MCE_Region_TypeDef *p_region;
  __IO uint32_t address;

  address = (__IO uint32_t)((uint32_t)hmce_flash.Instance + 0x40U + (0x10U * MCE_REGION1));
  p_region = (MCE_Region_TypeDef *)address;

  if ((p_region->REGCR & MCE_REGCR_BREN) == MCE_REGCR_BREN)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
  * @brief  Disable the MCE protection of the code primary region
  * @retval HAL_OK if successful, HAL_ERROR otherwise.
  */
HAL_StatusTypeDef LL_MCE_DisableCodePrimaryRegion(void)
{
  return HAL_MCE_DisableRegion(&hmce_flash, MCE_REGION1);
}

/**
  * @brief  Enable the MCE protection of the code primary region
  * @retval HAL_OK if successful, HAL_ERROR otherwise.
  */
HAL_StatusTypeDef LL_MCE_EnableCodePrimaryRegion(void)
{
  return HAL_MCE_EnableRegion(&hmce_flash, MCE_REGION1);
}

/**
  * @brief  Check whether the buffer is in the code primary region
  * @param  pBuffer address of the buffer
  * @param  Length length of the buffer
  * @retval 1 if enabled, 0 otherwise.
  */
bool LL_MCE_IsBufferInCodePrimaryRegion(const uint8_t *pBuffer, size_t Length)
{
  if ((((uint32_t)pBuffer) >= MCE_Flash_aRegion[MCE_REGION1].MCE_RegionConfig.StartAddress)
      && (((uint32_t)pBuffer) <= MCE_Flash_aRegion[MCE_REGION1].MCE_RegionConfig.EndAddress))
  {
    return true;
  }
  if ((((uint32_t)(pBuffer + Length)) >= MCE_Flash_aRegion[MCE_REGION1].MCE_RegionConfig.StartAddress)
      && (((uint32_t)(pBuffer + Length)) <= MCE_Flash_aRegion[MCE_REGION1].MCE_RegionConfig.EndAddress))
  {
    return true;
  }
  return false;
}

/**
 * @brief Generate Derived and Random Keys
 *
 * This function generates 2 keys : derived key and a random key.
 *
 * @param[out] p_derived_key Pointer to the variable where the derived key will be stored.
 * @param[out] p_random_key  Pointer to the variable where the random key will be stored.
 */
static void Generate_Keys(uint32_t *p_derived_key, uint32_t *p_random_key)
{
  CRYP_HandleTypeDef hcryp = { 0U };
  uint32_t a_Magic[4U] = {0xA3B21441U, 0x9B345FFEU, 0xC03655FBU, 0x87AB0F67U};
  size_t magic_length = 0xFFFF;

  /* Set the SAES parameters */
  hcryp.Instance            = SAES;
  hcryp.Init.DataType       = CRYP_NO_SWAP;
  hcryp.Init.Algorithm      = CRYP_AES_ECB;
  hcryp.Init.KeySelect      = CRYP_KEYSEL_HW;
  hcryp.Init.KeyMode        = CRYP_KEYMODE_NORMAL;
  hcryp.Init.KeySize        = CRYP_KEYSIZE_256B;

  /* Enable RNG clock */
  __HAL_RCC_RNG_CLK_ENABLE();
  /* Enable SAES clock */
  __HAL_RCC_SAES_CLK_ENABLE();

  /* Initialize SAES */
  if (HAL_CRYP_Init(&hcryp) != HAL_OK)
  {
    Error_Handler();
  }

  /* Generate key by encrypting magic with DHUK */
  if (HAL_CRYP_Encrypt(&hcryp, (uint32_t *)a_Magic, 8, p_derived_key, 100) != HAL_OK)
  {
    Error_Handler();
  }

  /* Generate random magic */
  RNG_GetBytes((uint8_t *)a_Magic, sizeof(a_Magic), &magic_length);
  if (magic_length != sizeof(a_Magic))
  {
    Error_Handler();
  }

  /* Generate key by encrypting random magic with DHUK */
  if (HAL_CRYP_Encrypt(&hcryp, (uint32_t *)a_Magic, 8, p_random_key, 100) != HAL_OK)
  {
    Error_Handler();
  }

  /* Deinitialize SAES */
  if (HAL_CRYP_DeInit(&hcryp) != HAL_OK)
  {
    Error_Handler();
  }
}

static int8_t MCE_CheckRegionConfig(MCE_HandleTypeDef *hmce, uint32_t RegionIndex,
                                       const MCE_RegionConfigTypeDef *pConfig)
{
  MCE_Region_TypeDef *p_region;
  __IO uint32_t address;
  uint32_t regcr = 0xFFFFFFFF;

  address = (__IO uint32_t)((uint32_t)hmce->Instance + 0x40U + (0x10U * RegionIndex));
  p_region = (MCE_Region_TypeDef *)address;

  regcr = pConfig->Mode | MCE_REGCR_BREN;

  /* Check start and end addresses, mode and enable status */
  if (((p_region->REGCR & (MCE_REGCR_ENC | MCE_REGCR_BREN)) == regcr) &&
       (p_region->SADDR == (pConfig->StartAddress & 0x0FFFFFFF)) &&
       (p_region->EADDR == (pConfig->EndAddress & 0x0FFFFFFF)))
  {
    return 0;
  }

  return -1;
}

static int8_t MCE_CheckRegionAESContext(MCE_HandleTypeDef *hmce, uint32_t ContextIndex, uint32_t RegionIndex)
{
  MCE_Region_TypeDef *p_region;
  __IO uint32_t address;
  uint32_t regcr = 0xFFFFFFFF;

  address = (__IO uint32_t)((uint32_t)hmce->Instance + 0x40U + (0x10U * RegionIndex));
  p_region = (MCE_Region_TypeDef *)address;

  regcr = ContextIndex | MCE_REGCR_BREN;

  /* Check context ID and enable status */
  if ((p_region->REGCR & (MCE_REGCR_CTXID | MCE_REGCR_BREN)) == regcr)
  {
    return 0;
  }

  return -1;
}

static int8_t MCE_CheckAESContext(MCE_HandleTypeDef *hmce, const MCE_AESConfigTypeDef  *AESConfig,
                                           uint32_t ContextIndex)
{
  MCE_Context_TypeDef *p_context;
  __IO uint32_t address;
  uint32_t cccfgr = 0xFFFFFFFF;

  address = (__IO uint32_t)((uint32_t)hmce->Instance + 0x240UL + \
                            (0x30UL * ((ContextIndex - MCE_CONTEXT1) >> MCE_REGCR_CTXID_Pos)));
  p_context = (MCE_Context_TypeDef *)address;

  cccfgr = (uint32_t) AESConfig->Version << MCE_CCCFGR_VERSION_Pos;
  cccfgr |= MCE_CCCFGR_CCEN;
  cccfgr |= AESConfig->Cipher_Mode;

  /* Check AES context */
  if (((hmce->Instance->CR & MCE_CR_CIPHERSEL) == AESConfig->KeySize) &&
       (p_context->CCNR0 == AESConfig->Nonce[0]) &&
       (p_context->CCNR1 == AESConfig->Nonce[1]) &&
      ((p_context->CCCFGR & (MCE_CCCFGR_VERSION | MCE_CCCFGR_CCEN | MCE_CCCFGR_MODE)) == cccfgr))
  {
    return 0;
  }

  return -1;
}

static HAL_StatusTypeDef MCE_GetLockGlobalConfig(MCE_HandleTypeDef *hmce)
{
  /* Check the parameters */
  assert_param(IS_MCE_ALL_INSTANCE(hmce->Instance));

  /* Locked */
  if ((hmce->Instance->CR & MCE_CR_GLOCK) == MCE_CR_GLOCK)
  {
    return HAL_OK;
  }

  /* Unlocked */
  return HAL_ERROR;
}

static HAL_StatusTypeDef MCE_GetLockAESContextConfig(MCE_HandleTypeDef *hmce, uint32_t ContextIndex)
{
  MCE_Context_TypeDef *p_context;
  __IO uint32_t address;

  /* Check the parameters */
  assert_param(IS_MCE_ALL_INSTANCE(hmce->Instance));
  assert_param(IS_MCE_CONTEXT(hmce->Instance, ContextIndex));

  address = (__IO uint32_t)((uint32_t)hmce->Instance + 0x240UL + \
                            (0x30UL * ((ContextIndex - MCE_CONTEXT1) >> MCE_REGCR_CTXID_Pos)));
  p_context = (MCE_Context_TypeDef *)address;

  /* Locked */
  if ((p_context->CCCFGR & MCE_CCCFGR_CCLOCK) == MCE_CCCFGR_CCLOCK)
  {
    return HAL_OK;
  }

  /* Unlocked */
  return HAL_ERROR;
}

static HAL_StatusTypeDef MCE_GetLockAESContextKey(MCE_HandleTypeDef *hmce, uint32_t ContextIndex)
{
  MCE_Context_TypeDef *p_context;
  __IO uint32_t address;

  /* Check the parameters */
  assert_param(IS_MCE_ALL_INSTANCE(hmce->Instance));
  assert_param(IS_MCE_CONTEXT(hmce->Instance, ContextIndex));

  address = (__IO uint32_t)((uint32_t)hmce->Instance + 0x240UL + \
                            (0x30UL * ((ContextIndex - MCE_CONTEXT1) >> MCE_REGCR_CTXID_Pos)));
  p_context = (MCE_Context_TypeDef *)address;

  /* Locked */
  if ((p_context->CCCFGR & MCE_CCCFGR_KEYLOCK) == MCE_CCCFGR_KEYLOCK)
  {
    return HAL_OK;
  }

  /* Unlocked */
  return HAL_ERROR;
}
