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
#include "nx_iperf.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern ctrl_info nx_iperf_ctrl_info;
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
extern void nx_iperf_udp_tx_cleanup(void);
extern VOID nx_iperf_entry(NX_PACKET_POOL *pool_ptr,
                           NX_IP *ip_ptr,
                           UCHAR *http_stack,
                           ULONG http_stack_size,
                           UCHAR *iperf_stack,
                           ULONG iperf_stack_size);
extern void nx_iperf_udp_tx_test(UCHAR *, ULONG);
extern UINT nx_driver_shaper_tas_entry(NX_SHAPER_DRIVER_PARAMETER *parameter);
extern UINT nx_driver_shaper_fpe_entry(NX_SHAPER_DRIVER_PARAMETER *parameter);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LINK_PRIORITY                          11
#define NX_APP_CABLE_CONNECTION_CHECK_PERIOD  (6 * NX_IP_PERIODIC_RATE)
/* USER CODE END PD */

#define NX_APP_PACKET_POOL_SIZE              ((DEFAULT_PAYLOAD_SIZE + sizeof(NX_PACKET)) * 20)

#define NX_APP_THREAD_STACK_SIZE             2 * 1024

#define Nx_IP_INSTANCE_THREAD_SIZE           2 * 1024

#define NX_APP_THREAD_PRIORITY               10

#define NX_APP_INSTANCE_PRIORITY             1

#ifndef NX_APP_INSTANCE_PRIORITY
#define NX_APP_INSTANCE_PRIORITY             NX_APP_THREAD_PRIORITY
#endif

#define CHANGE_FPE_STATE_TIMEOUT             5 * NX_IP_PERIODIC_RATE
#define NX_APP_DEFAULT_IP_ADDRESS            IP_ADDRESS(192, 168, 249, 6)
#define NX_APP_SECOND_IP_ADDRESS             IP_ADDRESS(192, 168, 250, 6)
#define NX_APP_DEFAULT_NET_MASK              IP_ADDRESS(255, 255, 255, 0)
/* Listener's IP addresses */
#define SERVER_IP_ADDRESS                    IP_ADDRESS(192, 168, 249, 7)
#define SERVER_SECOND_IP_ADDRESS             IP_ADDRESS(192, 168, 250, 7)

/* USER CODE BEGIN 1 */
/* vlan id = 2; vlan priority = 2 */
#define VLAN_TAG_1          (2 | (2<<13))

/* Iperf Configuration : */
#define PACKET_SIZE         1400;
#define SERVER_PORT         5001
#define VERSION_V4          4
#define CTRL_SIGN_7         7
#define TEST_TIME           (10 * NX_IP_PERIODIC_RATE)

#define HTTP_STACK_SIZE     4096
#define IPERF_STACK_SIZE    4096
/* Time Aware Shaper Timing parameters */
#define TAS_CYCLE_TIME                      1000000U
#define QUEUE_1_SLOT_TIME                   700000U
#define QUEUE_0_SLOT_TIME                   TAS_CYCLE_TIME - QUEUE_1_SLOT_TIME
#define QUEUE_0_SLOT_OFFSET                 QUEUE_1_SLOT_TIME
/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif
#endif /* __APP_NETXDUO_H__ */
