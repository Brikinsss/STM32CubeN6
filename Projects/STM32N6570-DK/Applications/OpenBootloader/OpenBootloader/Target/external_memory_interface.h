/**
  ******************************************************************************
  * @file    external_memory_interface.h
  * @author  MCD Application Team
  * @brief   Header for external_memory_interface.c module
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

#ifndef EXTERNAL_MEMORY_INTERFACE_H
#define EXTERNAL_MEMORY_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
uint32_t OPENBL_ExtMem_Init(uint32_t Address);
uint8_t OPENBL_ExtMem_Read(uint32_t Address);
void OPENBL_ExtMem_Write(uint32_t Address, uint8_t *pData, uint32_t DataLength);
uint64_t OPENBL_ExtMem_Verify(uint32_t Address, uint32_t DataAddr, uint32_t DataLength, uint32_t CrcInit);
void OPENBL_ExtMem_JumpToAddress(uint32_t Address);
void OPENBL_ExtMem_MassErase(uint32_t Address);
void OPENBL_ExtMem_SectorErase(uint32_t StartAddress, uint32_t EndAddress);

#endif /* EXTERNAL_MEMORY_INTERFACE_H */
