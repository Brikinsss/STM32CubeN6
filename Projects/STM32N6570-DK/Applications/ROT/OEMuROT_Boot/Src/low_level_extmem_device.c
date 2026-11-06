/**
  ******************************************************************************
  * @file    low_level_extmem_device.c
  * @author  MCD Application Team
  * @brief   This file contains device definition for low_level_extmem_device
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

#include "flash_layout.h"
#include "low_level_ext_flash.h"

XSPI_HandleTypeDef XSPI_HANDLE;

static struct ext_flash_range access_vect[] =
{
  { FLASH_AREA_BEGIN_OFFSET, FLASH_AREA_END_OFFSET - 1},
#if defined(MCUBOOT_USE_HASH_REF)
  { FLASH_AREA_HASH_REF_OFFSET, FLASH_AREA_HASH_REF_OFFSET + FLASH_AREA_HASH_REF_SIZE - 1},
#endif /* MCUBOOT_USE_HASH_REF */
#if !defined(MCUBOOT_OVERWRITE_ONLY)
  { FLASH_AREA_SCRATCH_OFFSET, FLASH_AREA_SCRATCH_OFFSET + FLASH_AREA_SCRATCH_SIZE - 1}
#endif /* not MCUBOOT_OVERWRITE_ONLY */
};

struct low_level_ext_flash_device EXT_FLASH0_DEV =
{
  .erase = { .nb = sizeof(access_vect) / sizeof(struct ext_flash_range), .range = access_vect},
  .write = { .nb = sizeof(access_vect) / sizeof(struct ext_flash_range), .range = access_vect},
};
