/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_netxduo.h
  * @author  MCD Application Team
  * @brief   NetXDuo applicative header file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_NETXDUO_H__
#define __APP_NETXDUO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
#include "nx_stm32_eth_driver.h"

/* USER CODE BEGIN Includes */
#include "main.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */
/* The DEFAULT_PAYLOAD_SIZE should match with RxBuffLen configured via MX_ETH_Init */
#ifndef DEFAULT_PAYLOAD_SIZE
#define DEFAULT_PAYLOAD_SIZE      1536
#endif

#ifndef DEFAULT_ARP_CACHE_SIZE
#define DEFAULT_ARP_CACHE_SIZE    1024
#endif

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define PRINT_IP_ADDRESS(addr)             do { \
                                                printf("STM32 %s: %lu.%lu.%lu.%lu \n", #addr, \
                                                (addr >> 24) & 0xff, \
                                                (addr >> 16) & 0xff, \
                                                (addr >> 8) & 0xff, \
                                                addr& 0xff);\
                                           }while(0)
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
UINT MX_NetXDuo_Init(VOID *memory_ptr);

/* USER CODE BEGIN EFP */
extern UINT   nx_driver_shaper_tas_entry(NX_SHAPER_DRIVER_PARAMETER *parameter);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_QUEUE_SIZE                        5
/* USER CODE END PD */

#define NX_APP_THREAD_TIME_SLICE             1

#define NX_APP_PACKET_POOL_SIZE              ((DEFAULT_PAYLOAD_SIZE + sizeof(NX_PACKET)) * 30)

#define NX_APP_THREAD_STACK_SIZE             2 * 1024

#define Nx_IP_INSTANCE_THREAD_SIZE           2 * 1024

#define NX_APP_THREAD_PRIORITY               3

#define NX_APP_INSTANCE_PRIORITY             1

#ifndef NX_APP_INSTANCE_PRIORITY
#define NX_APP_INSTANCE_PRIORITY             NX_APP_THREAD_PRIORITY
#endif

#define NX_APP_DEFAULT_IP_ADDRESS            IP_ADDRESS(192, 168, 249, 6)
#define NX_APP_SECOND_IP_ADDRESS             IP_ADDRESS(192, 168, 250, 6)
#define NX_APP_DEFAULT_NET_MASK              IP_ADDRESS(255, 255, 255, 0)

#define SERVER_IP_ADDRESS                    IP_ADDRESS(192, 168, 249, 5)
#define SERVER_IP_SECOND_ADDRESS             IP_ADDRESS(192, 168, 250, 5)

/* vlan id = 2; vlan priority = 2 */
#define VLAN_TAG_1                           (2 | (2<<13))

#define UDP_PORT_1                           5001
#define UDP_PORT_2                           5002

#define ETH_QUEUE_0                          0U
#define ETH_QUEUE_1                          1U

#define DEFAULT_MESSAGE_Q0                  "QUEUE 0"
#define DEFAULT_MESSAGE_Q1                  "QUEUE 1"

/* Time Aware Shaper Timing parameters */
#define TAS_CYCLE_TIME                      1000000U  /* 1000000U = 1 ms */
#define QUEUE_1_SLOT_TIME                   700000U
#define QUEUE_0_SLOT_TIME                   TAS_CYCLE_TIME - QUEUE_1_SLOT_TIME
#define QUEUE_0_SLOT_OFFSET                 QUEUE_1_SLOT_TIME

#ifdef __cplusplus
}
#endif
#endif /* __APP_NETXDUO_H__ */
