/*
 * Copyright (c) 2017-2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __REGION_DEFS_H__
#define __REGION_DEFS_H__

#include "flash_layout.h"

#define BL2_HEAP_SIZE           0x0000800
#define BL2_MSP_STACK_SIZE      0x0001400

#define S_HEAP_SIZE             0x0001000
#define S_MSP_STACK_SIZE        0x0000800
#define S_PSP_STACK_SIZE        0x0000800

#define NS_HEAP_SIZE            0x0001000
#define NS_MSP_STACK_SIZE       0x0000C00
#define NS_PSP_STACK_SIZE       0x0000C00

#define _SRAM1_AXI_BASE_NS     (0x24000000)
#define _SRAM2_AXI_BASE_S      (0x34100000)   /* Base address of 1 MB system RAM 2 accessible over AXI */
#define _SRAM2_AXI_SIZE        (0x100000)     /* SRAM2_AXI = 1024 Kbytes */

#define BOOT_SHARED_DATA_SIZE        0
#define BOOT_SHARED_DATA_BASE        0

/*
 * Boot partition structure if MCUBoot is used:
 * 0x0_0000 Bootloader header
 * 0x0_0400 Image area
 * 0xz_zzzz Trailer
 */
/* IMAGE_CODE_SIZE is the space available for the software binary image.
 * It is less than the FLASH_PARTITION_SIZE because we reserve space
 * for the image header and trailer introduced by the bootloader.
 */

#define BL2_HEADER_SIZE                     (0x400) /*!< Appli image header size */
#define BL2_TRAILER_SIZE                    (0x2000)

/* External Flash base */
#define EXT_ROM_ALIAS(x)                    (EXT_FLASH_BASE_ADDRESS + (x))
#define EXT_RAM_ALIAS(x)                    (EXT_RAM_BASE_ADDRESS + (x))

#define S_RAM_ALIAS_BASE                    (_SRAM2_AXI_BASE_S)
#define NS_RAM_ALIAS_BASE                   (_SRAM1_AXI_BASE_NS)

/* Alias definitions for secure and non-secure areas*/
#define S_RAM_ALIAS(x)                      (S_RAM_ALIAS_BASE + (x))
#define NS_RAM_ALIAS(x)                     (NS_RAM_ALIAS_BASE + (x))

#define FLASH_APPLI_S_ADDRESS               (EXT_ROM_ALIAS(FLASH_AREA_0_OFFSET))
#define FLASH_DATA_S_ADDRESS                (EXT_ROM_ALIAS(FLASH_AREA_4_OFFSET))
#define FLASH_APPLI_NS_ADDRESS              (EXT_ROM_ALIAS(FLASH_AREA_1_OFFSET))
#define FLASH_DATA_NS_ADDRESS               (EXT_ROM_ALIAS(FLASH_AREA_5_OFFSET))

#define FLASH_PRIMARY_AREA_START_OFFSET     (FLASH_AREA_BEGIN_OFFSET)
#define FLASH_PRIMARY_AREA_LIMIT_OFFSET     (FLASH_AREA_BEGIN_OFFSET + FLASH_AREA_0_SIZE + \
                                             FLASH_AREA_4_SIZE + FLASH_AREA_1_SIZE + \
                                             FLASH_AREA_5_SIZE - 1)

#define FLASH_PRIMARY_AREA_START_ADDRESS    (EXT_ROM_ALIAS(FLASH_PRIMARY_AREA_START_OFFSET))
#define FLASH_PRIMARY_AREA_LIMIT_ADDRESS    (EXT_ROM_ALIAS(FLASH_PRIMARY_AREA_LIMIT_OFFSET))

/* BOOTROM */
#define BOOTROM_HEADER_SIZE                 (0x400)
#define BOOTROM_VERSION_OFFSET              (0x80)

/* BL2 regions */
#define BL2_OFFSET                          (0x80000)
#define BL2_CODE_OFFSET                     (BL2_OFFSET + BOOTROM_HEADER_SIZE)
#define BL2_CODE_START                      (S_RAM_ALIAS(BL2_CODE_OFFSET))
#define BL2_CODE_SIZE                       (FLASH_AREA_BL2_SIZE - BL2_JUMP_CODE_SIZE \
                                                                 - BOOTROM_HEADER_SIZE)
#define BL2_CODE_LIMIT                      (BL2_CODE_START + BL2_CODE_SIZE - 1)

#define BL2_JUMP_CODE_SIZE                  (0x2600)
#define BL2_JUMP_CODE_OFFSET                (BL2_CODE_OFFSET + BL2_CODE_SIZE)
#define BL2_JUMP_CODE_START                 (S_RAM_ALIAS(BL2_JUMP_CODE_OFFSET))
#define BL2_JUMP_CODE_LIMIT                 (BL2_JUMP_CODE_START + BL2_JUMP_CODE_SIZE - 1)

#define BL2_DATA_OFFSET                     (0xC0000)
#define BL2_DATA_START                      (S_RAM_ALIAS(BL2_DATA_OFFSET))
#define BL2_DATA_SIZE                       (0x20000)
#define BL2_DATA_LIMIT                      (BL2_DATA_START + BL2_DATA_SIZE - 1)

#if (OEMUROT_LOAD_AND_RUN == NO_LOAD_AND_RUN)
/* Secure regions */
#define S_CODE_START                        (EXT_ROM_ALIAS(FLASH_AREA_0_OFFSET))
#define S_DATA_START                        (S_RAM_ALIAS(0))
#define S_DATA2_START                       (EXT_ROM_ALIAS(FLASH_AREA_4_OFFSET))
/* Non-secure regions */
#define NS_CODE_START                       (EXT_ROM_ALIAS(FLASH_AREA_1_OFFSET))
#define NS_DATA_START                       (NS_RAM_ALIAS(0x64000))
#define NS_DATA2_START                      (EXT_ROM_ALIAS(FLASH_AREA_5_OFFSET))
#elif (OEMUROT_LOAD_AND_RUN == LOAD_AND_RUN_INT_RAM)
/* Secure regions */
#define S_CODE_START                        (S_RAM_ALIAS(0))
#define S_DATA_START                        (S_RAM_ALIAS(0xE0000))
#define S_DATA2_START                       (S_RAM_ALIAS(0xF0000))
/* Non-secure regions */
#define NS_CODE_START                       (NS_RAM_ALIAS(0x64000))
#define NS_DATA_START                       (NS_RAM_ALIAS(0xE4000))
#define NS_DATA2_START                      (NS_RAM_ALIAS(0xF0000))
#elif (OEMUROT_LOAD_AND_RUN == LOAD_AND_RUN_EXT_RAM)
/* Secure regions */
#define S_CODE_START                        (EXT_RAM_ALIAS(0))
#define S_DATA_START                        (EXT_RAM_ALIAS(0xFE0000))
#define S_DATA2_START                       (EXT_RAM_ALIAS(0xFF0000))
/* Non-secure regions */
#define NS_CODE_START                       (EXT_RAM_ALIAS(0x1000000))
#define NS_DATA_START                       (EXT_RAM_ALIAS(0x1FE0000))
#define NS_DATA2_START                      (EXT_RAM_ALIAS(0x1FF0000))
#else
#error "Unknown OEMUROT_LOAD_AND_RUN"
#endif /* OEMUROT_LOAD_AND_RUN == NO_LOAD_AND_RUN */

/* Secure regions */
#define S_CODE_SIZE                         (FLASH_S_PARTITION_SIZE - BL2_TRAILER_SIZE)
#define S_DATA_SIZE                         (0x10000)
#define S_DATA2_SIZE                        (FLASH_S_DATA_PARTITION_SIZE - BL2_TRAILER_SIZE)
/* Non-secure regions */
#define NS_CODE_SIZE                        (FLASH_NS_PARTITION_SIZE - BL2_TRAILER_SIZE)
#define NS_DATA_SIZE                        (0xC000)
#define NS_DATA2_SIZE                       (FLASH_NS_DATA_PARTITION_SIZE - BL2_TRAILER_SIZE)

/* Secure regions */
#define S_CODE_LIMIT                        (S_CODE_START + S_CODE_SIZE - 1)
#define S_DATA_LIMIT                        (S_DATA_START + S_DATA_SIZE - 1)
#define S_DATA2_LIMIT                       (S_DATA2_START + S_DATA2_SIZE - 1)
/* Non-secure regions */
#define NS_CODE_LIMIT                       (NS_CODE_START + NS_CODE_SIZE - 1)
#define NS_DATA_LIMIT                       (NS_DATA_START + NS_DATA_SIZE - 1)
#define NS_DATA2_LIMIT                      (NS_DATA2_START + NS_DATA2_SIZE - 1)

/* Additional Check to detect flash download slot overlap or overflow */
#define FLASH_AREA_END_OFFSET_MAX (EXT_FLASH_TOTAL_SIZE)
#if FLASH_AREA_END_OFFSET > FLASH_AREA_END_OFFSET_MAX
#error "Flash memory overflow"
#endif /* FLASH_AREA_END_OFFSET > FLASH_AREA_END_OFFSET_MAX */

#endif /* __REGION_DEFS_H__ */
