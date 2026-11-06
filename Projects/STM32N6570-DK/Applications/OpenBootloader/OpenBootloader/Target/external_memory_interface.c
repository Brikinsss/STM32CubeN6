/**
  ******************************************************************************
  * @file    external_memory_interface.c
  * @author  MCD Application Team
  * @brief   Contains External memory access functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "openbl_mem.h"
#include "app_openbootloader.h"
#include "common_interface.h"
#include "external_memory_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define INIT_INDEX                                0x00U
#define WRITE_INDEX                               0x01U
#define SECTOR_ERASE_INDEX                        0x02U
#define MASS_ERASE_INDEX                          0x03U
#define VERIFY_INDEX                              0x05U
#define READ_INDEX                                0x06U

/* Address where flash loader functions pointers are placed in RAM */
#define FLASH_LOADER_FUNCTIONS_POINTERS           0x38000000U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Base address of external flash loader functions, can change if external flash loader project is modified */
static uint32_t *FlashLoaderFunctionsAdd = (uint32_t *)FLASH_LOADER_FUNCTIONS_POINTERS;

/* Private function prototypes -----------------------------------------------*/
static uint32_t OPENBL_ExtMem_IsAddressInRam(uint32_t);
static uint32_t (*ExtLoader_Init)(void);
static uint32_t (*ExtLoader_Write)(uint32_t Address, uint32_t DataLength, uint8_t *pData);
static uint32_t (*ExtLoader_Erase)(uint32_t StartAddress, uint32_t EndAddress);
static uint32_t (*ExtLoader_MassErase)(void);
static uint64_t (*ExtLoader_Verify)(uint32_t Address, uint32_t DataAddr, uint32_t DataLength, uint32_t CrcInit);
static void (*ExtLoader_Read)(uint32_t Address, uint32_t DataLength, uint8_t *pData);

/* Exported variables --------------------------------------------------------*/
/* In the future the items of this structure will be automatically getting from the external loader*/
OPENBL_MemoryTypeDef EXTERNAL_MEMORY_Descriptor =
{
  EXT_MEMORY_START_ADDRESS,
  EXT_MEMORY_END_ADDRESS,
  EXT_MEMORY_SIZE,
  EXTERNAL_MEMORY_AREA,
  OPENBL_ExtMem_Init,
  OPENBL_ExtMem_Read,
  OPENBL_ExtMem_Write,
  OPENBL_ExtMem_JumpToAddress,
  OPENBL_ExtMem_MassErase,
  OPENBL_ExtMem_SectorErase,
  OPENBL_ExtMem_Verify
};

/* Exported functions --------------------------------------------------------*/
/**
  * @brief  This function is used for initializing external memory.
  * @param  Address Address where external flash loader functions pointers are placed.
  * @retval Returns 1 if success else returns 0.
  */
uint32_t OPENBL_ExtMem_Init(uint32_t Address)
{
  uint32_t status = 0U;

  UNUSED(Address);

  /* Functions pointers initialization */
  ExtLoader_Init      = (uint32_t (*)(void))(*(FlashLoaderFunctionsAdd + INIT_INDEX));
  ExtLoader_Write     = (uint32_t (*)(uint32_t, uint32_t, uint8_t *))(*(FlashLoaderFunctionsAdd + WRITE_INDEX));
  ExtLoader_Erase     = (uint32_t (*)(uint32_t, uint32_t))(*(FlashLoaderFunctionsAdd + SECTOR_ERASE_INDEX));
  ExtLoader_MassErase = (uint32_t (*)(void))(*(FlashLoaderFunctionsAdd + MASS_ERASE_INDEX));
  ExtLoader_Verify    = (uint64_t (*)(uint32_t, uint32_t, uint32_t, uint32_t))(*(FlashLoaderFunctionsAdd + VERIFY_INDEX));
  ExtLoader_Read      = (void (*)(uint32_t, uint32_t, uint8_t *))(*(FlashLoaderFunctionsAdd + READ_INDEX));

  /* Make sure that the Init function points to RAM */
  if (OPENBL_ExtMem_IsAddressInRam((uint32_t)ExtLoader_Init))
  {
    status = ExtLoader_Init();
  }

  return status;
}

/**
  * @brief  This function is used to read data from a given address.
  * @param  Address The address to be read.
  * @retval Returns the read value.
  */
uint8_t OPENBL_ExtMem_Read(uint32_t Address)
{
  static uint8_t data[1];

  if (ExtLoader_Read != NULL)
  {
    /* Read one byte of data from external memory */
    if (OPENBL_ExtMem_IsAddressInRam((uint32_t)ExtLoader_Read))
    {
      ExtLoader_Read(Address, 1U, data);
    }

    /* Return the read data */
    return (*data);
  }
  else
  {
    /* Return the read data */
    return (*(uint8_t *)(Address));
  }
}

/**
  * @brief  This function is used to write data in external memory.
  * @param  Address The address where that data will be written.
  * @param  Data The data to be written.
  * @param  DataLength The length of the data to be written.
  * @retval None.
  */
void OPENBL_ExtMem_Write(uint32_t Address, uint8_t *pData, uint32_t DataLength)
{
  if (OPENBL_ExtMem_IsAddressInRam((uint32_t)ExtLoader_Write))
  {
    ExtLoader_Write(Address, DataLength, pData);
  }
}

/**
  * @brief  Verify flash memory with RAM buffer and calculates checksum value of
  * the programmed memory
  * @param  Address The Flash address
  * @param  DataAddr The RAM buffer address
  * @param  DataLength The Size (in WORD)
  * @param  CrcInit The Initial CRC value
  * @retval R0: Operation failed (address of failure)
  *         R1: Checksum value
  */
uint64_t OPENBL_ExtMem_Verify(uint32_t Address, uint32_t DataAddr, uint32_t DataLength, uint32_t CrcInit)
{
  if (OPENBL_ExtMem_IsAddressInRam((uint32_t)ExtLoader_Verify))
  {
    return (ExtLoader_Verify(Address, DataAddr, DataLength, CrcInit));
  }

  return 0U;
}

/**
  * @brief  This function is used to jump to a given address.
  * @param  Address The address where the function will jump.
  * @retval None.
  */
void OPENBL_ExtMem_JumpToAddress(uint32_t Address)
{
  Function_Pointer jump_to_address;

  /* Deinitialize all HW resources used by the Bootloader to their reset values */
  OpenBootloader_DeInit();

  /* Enable IRQ */
  Common_EnableIrq();

  jump_to_address = (Function_Pointer)(*(__IO uint32_t *)(Address + 4U));

  /* Initialize user application's stack pointer */
  Common_SetMsp(*(__IO uint32_t *) Address);

  jump_to_address();
}

/**
  * @brief  This function is used to erase the chip.
  * @param  None.
  * @retval None.
  */
void OPENBL_ExtMem_MassErase(uint32_t Address)
{
  int status;

  if (OPENBL_ExtMem_IsAddressInRam((uint32_t)ExtLoader_MassErase))
  {
    status = ExtLoader_MassErase();

    UNUSED(status);
  }
}

/**
  * @brief  This function is used to erase the specified external memory sector.
  * @param  StartAddress Address of the first sector to be erased.
  * @param  EndAddress Address of the last sector to be erased.
  * @retval None.
  */
void OPENBL_ExtMem_SectorErase(uint32_t StartAddress, uint32_t EndAddress)
{
  int status;

  if (OPENBL_ExtMem_IsAddressInRam((uint32_t)ExtLoader_Erase))
  {
    status = ExtLoader_Erase(StartAddress, EndAddress);

    UNUSED(status);
  }
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Check if the function address is located in RAM.
  * @param  address the address to be check.
  * @retval Returns true if the function address is in RAM, false otherwise.
  */
static uint32_t OPENBL_ExtMem_IsAddressInRam(uint32_t Address)
{
  return ((Address > FLASH_LOADER_START_ADDRESS) && (Address < FLASH_LOADER_END_ADDRESS));
}
