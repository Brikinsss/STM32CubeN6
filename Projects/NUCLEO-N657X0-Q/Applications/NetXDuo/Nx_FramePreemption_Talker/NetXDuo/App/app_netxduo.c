/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file    app_netxduo.c
* @author  MCD Application Team
* @brief   NetXDuo applicative file
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "nxd_ptp_client.h"
#include "nx_shaper.h"
#include "app_netxduo.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* if FPE_ENABLED flag is defined, FPE feature is enabled, otherwise, FPE feature is disabled */
#define FPE_ENABLED
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
static TX_THREAD      NxAppThread;
static TX_THREAD AppLinkThread;
static NX_PACKET_POOL NxAppPool;
static NX_IP          NetXDuoEthIpInstance;
/* USER CODE BEGIN PV */
static ULONG IpAddress;
static ULONG NetMask;

static ULONG http_stack[HTTP_STACK_SIZE / sizeof(ULONG)];
static ULONG iperf_stack[IPERF_STACK_SIZE / sizeof(ULONG)];

static UINT interface_1;

static NX_SHAPER_CONTAINER shaper_container;
static NX_SHAPER_TAS_CONFIG tas_config;
static NX_SHAPER tas_shaper;
#ifdef FPE_ENABLED
static NX_SHAPER_FP_PARAMETER fp_config;
static NX_SHAPER fp_shaper;
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static VOID nx_app_thread_entry (ULONG thread_input);
static UINT shaper_init(NX_INTERFACE *interface_ptr);
static VOID App_Link_Thread_Entry(ULONG thread_input);
/* USER CODE END PFP */

/**
* @brief  Application NetXDuo Initialization.
* @param memory_ptr: memory pointer
* @retval int
*/
UINT MX_NetXDuo_Init(VOID *memory_ptr)
{
  UINT ret = NX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  CHAR *pointer;

  /* USER CODE BEGIN MX_NetXDuo_MEM_POOL */

  /* USER CODE END MX_NetXDuo_MEM_POOL */

  /* USER CODE BEGIN 0 */
  printf("Nx_FramePreemption_Talker application started..\n");
  /* USER CODE END 0 */

  /* Initialize the NetXDuo system. */
  nx_system_initialize();

  /* Allocate the memory for packet_pool.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_APP_PACKET_POOL_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the Packet pool to be used for packet allocation,
  * If extra NX_PACKET are to be used the NX_APP_PACKET_POOL_SIZE should be increased
  */
  ret = nx_packet_pool_create(&NxAppPool, "NetXDuo App Pool", DEFAULT_PAYLOAD_SIZE, pointer, NX_APP_PACKET_POOL_SIZE);

  if (ret != NX_SUCCESS)
  {
    return NX_POOL_ERROR;
  }

  /* Allocate the memory for Ip_Instance */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, Nx_IP_INSTANCE_THREAD_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the main NX_IP instance */
  ret = nx_ip_create(&NetXDuoEthIpInstance, "NetX Ip instance", NX_APP_DEFAULT_IP_ADDRESS, NX_APP_DEFAULT_NET_MASK, &NxAppPool, nx_stm32_eth_driver,
                     pointer, Nx_IP_INSTANCE_THREAD_SIZE, NX_APP_INSTANCE_PRIORITY);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_SUCCESSFUL;
  }

  /* Allocate the memory for ARP */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, DEFAULT_ARP_CACHE_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Enable the ARP protocol and provide the ARP cache size for the IP instance */

  /* USER CODE BEGIN ARP_Protocol_Initialization */

  /* USER CODE END ARP_Protocol_Initialization */

  ret = nx_arp_enable(&NetXDuoEthIpInstance, (VOID *)pointer, DEFAULT_ARP_CACHE_SIZE);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_SUCCESSFUL;
  }

  /* Enable the ICMP */

  /* USER CODE BEGIN ICMP_Protocol_Initialization */

  /* USER CODE END ICMP_Protocol_Initialization */

  ret = nx_icmp_enable(&NetXDuoEthIpInstance);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_SUCCESSFUL;
  }

  /* Enable TCP Protocol */

  /* USER CODE BEGIN TCP_Protocol_Initialization */
  /* Allocate the memory for TCP server thread   */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_APP_THREAD_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* USER CODE END TCP_Protocol_Initialization */

  ret = nx_tcp_enable(&NetXDuoEthIpInstance);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_SUCCESSFUL;
  }

  /* USER CODE BEGIN UDP_Protocol_Initialization */

  /* USER CODE END UDP_Protocol_Initialization */

  ret = nx_udp_enable(&NetXDuoEthIpInstance);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_SUCCESSFUL;
  }

  /* Allocate the memory for main thread   */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_APP_THREAD_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the main thread */
  ret = tx_thread_create(&NxAppThread, "NetXDuo App thread", nx_app_thread_entry , 0, pointer, NX_APP_THREAD_STACK_SIZE,
                         NX_APP_THREAD_PRIORITY, NX_APP_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

  if (ret != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* USER CODE BEGIN MX_NetXDuo_Init */
  /* Allocate the memory for Link thread   */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,NX_APP_THREAD_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* create the Link thread */
  ret = tx_thread_create(&AppLinkThread, "App Link Thread", App_Link_Thread_Entry, 0, pointer, NX_APP_THREAD_STACK_SIZE,
                         LINK_PRIORITY, LINK_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

  if (ret != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }
  /* USER CODE END MX_NetXDuo_Init */

  return ret;
}

/**
* @brief  Main thread entry.
* @param thread_input: ULONG user argument used by the thread entry
* @retval none
*/
static VOID nx_app_thread_entry (ULONG thread_input)
{
  /* USER CODE BEGIN Nx_App_Thread_Entry 0 */
  ULONG actual_status;
  /* USER CODE END Nx_App_Thread_Entry 0 */

  UINT ret = NX_SUCCESS;

  /* USER CODE BEGIN Nx_App_Thread_Entry 1 */

  /* USER CODE END Nx_App_Thread_Entry 1 */

  /* USER CODE BEGIN Nx_App_Thread_Entry 2 */
  ret = nx_ip_address_get(&NetXDuoEthIpInstance, &IpAddress, &NetMask);

  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  ret = nx_ip_interface_status_check(&NetXDuoEthIpInstance, 0, NX_IP_LINK_ENABLED, &actual_status, 100);

  if (ret != NX_SUCCESS)
  {
    printf("Link is down, please check Ethernet cable \n");
    tx_thread_suspend(&NxAppThread);
  }

  PRINT_IP_ADDRESS(IpAddress);

  /* Create a secondary interface */
  ret = nx_link_vlan_interface_create(&NetXDuoEthIpInstance, "NetX IP Interface 0:1", NX_APP_SECOND_IP_ADDRESS,
                                      NX_APP_DEFAULT_NET_MASK, VLAN_TAG_1, 0, &interface_1);

  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

#ifdef FPE_ENABLED
  printf("FPE feature is enabled \n");
#else
  printf("FPE feature is disabled \n");
#endif

  /* FPE & TAS configuration */
  ret = shaper_init(&(NetXDuoEthIpInstance.nx_ip_interface[0]));

  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  /* 2 sec delay before start sending */
   tx_thread_sleep(2 * NX_IP_PERIODIC_RATE);

  nx_iperf_entry(&NxAppPool, &NetXDuoEthIpInstance, (UCHAR *)http_stack, sizeof(http_stack), (UCHAR *)iperf_stack, sizeof(iperf_stack));

  while (1)
  {
    nx_iperf_udp_tx_cleanup();
    nx_iperf_ctrl_info.TestTime = TEST_TIME; /* Run test for 10 seconds*/
    nx_iperf_ctrl_info.ip = SERVER_IP_ADDRESS;
    nx_iperf_ctrl_info.PacketSize = PACKET_SIZE;
    nx_iperf_ctrl_info.port = SERVER_PORT;
    nx_iperf_ctrl_info.Rate = 10;
    nx_iperf_ctrl_info.version = VERSION_V4;
    nx_iperf_ctrl_info.ctrl_sign = CTRL_SIGN_7;

    printf("IPerf UDP TX Started .. \r\n");
    nx_iperf_udp_tx_test((UCHAR *)iperf_stack,sizeof(iperf_stack));

    tx_thread_sleep(nx_iperf_ctrl_info.TestTime + 300);
  }
  /* USER CODE END Nx_App_Thread_Entry 2 */
}
/* USER CODE BEGIN 1 */
/**
* @brief  Link thread entry
* @param thread_input: ULONG thread parameter
* @retval none
*/
static VOID App_Link_Thread_Entry(ULONG thread_input)
{
  ULONG actual_status;
  UINT linkdown = 0, status;

  while(1)
  {
    /* Send request to check if the Ethernet cable is connected. */
    status = nx_ip_interface_status_check(&NetXDuoEthIpInstance, 0, NX_IP_LINK_ENABLED,
                                          &actual_status, 10);

    if(status == NX_SUCCESS)
    {
      if(linkdown == 1)
      {
        linkdown = 0;

        /* The network cable is connected. */
        printf("The network cable is connected.\n");

        /* Send request to enable PHY Link. */
        nx_ip_driver_direct_command(&NetXDuoEthIpInstance, NX_LINK_ENABLE,
                                    &actual_status);
      }
      tx_thread_resume(&NxAppThread);
    }
    else
    {
      if(0 == linkdown)
      {
        linkdown = 1;
        /* The network cable is not connected. */
        printf("The network cable is not connected.\n");
        nx_ip_driver_direct_command(&NetXDuoEthIpInstance, NX_LINK_DISABLE,
                                    &actual_status);
      }
    }

    tx_thread_sleep(NX_APP_CABLE_CONNECTION_CHECK_PERIOD);
  }
}

/**
  * @brief  Initialize the Ethernet Shaper.
  * @param  interface_ptr: pointer to a NX_INTERFACE structure that contains
  *         the configuration information for ETHERNET Shaper
  * @retval status
  */
UINT shaper_init(NX_INTERFACE *interface_ptr)
{
  UINT ret;
  UCHAR pcp_list[8];
  UCHAR queue_id_list[8];

  ret = nx_shaper_create(interface_ptr, &shaper_container, &tas_shaper, NX_SHAPER_TYPE_TAS, nx_driver_shaper_tas_entry);
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

#ifdef FPE_ENABLED
    ret = nx_shaper_create(interface_ptr, &shaper_container, &fp_shaper, NX_SHAPER_TYPE_FP, nx_driver_shaper_fpe_entry);
    if (ret != NX_SUCCESS)
    {
      Error_Handler();
    }
#endif

  ret = nx_shaper_default_mapping_get(interface_ptr, pcp_list, queue_id_list, 8);
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  ret = nx_shaper_mapping_set(interface_ptr, pcp_list, queue_id_list, 8);
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

#ifdef FPE_ENABLED
    memset(&fp_config, 0, sizeof(NX_SHAPER_FP_PARAMETER));

    fp_config.verification_enable = 1;
    fp_config.express_guardband_enable = NX_TRUE;
    fp_config.ha = 0;
    fp_config.ra = 0;
    fp_config.express_queue_bitmap = (1 << 2);

    ret = nx_shaper_fp_parameter_set(interface_ptr, &fp_config);
    if (ret != NX_SUCCESS)
    {
      Error_Handler();
    }
#endif

  tas_config.base_time = 0;
  tas_config.auto_fill_status = NX_SHAPER_TAS_IDLE_CYCLE_AUTO_FILL_DISABLED;
  tas_config.cycle_time = TAS_CYCLE_TIME; /* 1ms */
  tas_config.traffic_count = 2;

  tas_config.traffic[0].pcp = 2;
  tas_config.traffic[0].time_offset = 0;
  tas_config.traffic[0].duration = QUEUE_1_SLOT_TIME;
  tas_config.traffic[0].traffic_control = NX_SHAPER_TRAFFIC_OPEN;

  tas_config.traffic[1].pcp = 0;
  tas_config.traffic[1].time_offset = QUEUE_0_SLOT_OFFSET;
  tas_config.traffic[1].duration = QUEUE_0_SLOT_TIME;
  tas_config.traffic[1].traffic_control = NX_SHAPER_TRAFFIC_OPEN;

  ret = nx_shaper_tas_parameter_set(interface_ptr, &tas_config);

  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  return ret;
}
/* USER CODE END 1 */