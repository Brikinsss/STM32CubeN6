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

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
static TX_THREAD      NxAppThread;
static NX_PACKET_POOL NxAppPool;
static NX_IP          NetXDuoEthIpInstance;

/* USER CODE BEGIN PV */
static TX_THREAD AppLinkThread;

static ULONG IpAddress;
static ULONG NetMask;

static ULONG http_stack[HTTP_STACK_SIZE / sizeof(ULONG)];
static ULONG iperf_stack[IPERF_STACK_SIZE / sizeof(ULONG)];

static UINT interface_1;

static NX_SHAPER_CONTAINER shaper_container;

static NX_SHAPER cbs_shaper;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static VOID nx_app_thread_entry (ULONG thread_input);
static VOID App_Link_Thread_Entry(ULONG thread_input);
static VOID nx_app_cbs_config(VOID);
static VOID ip_to_string(ULONG ipv4, char* str);
/* USER CODE BEGIN PFP */

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
  printf("Nx_CreditBasedShaper application started..\r\n");
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
  ULONG actual_status,status;
  char ip_addr[IP_ADDRESS_STR_SIZE];
  CHAR ping_data[32];
  NX_PACKET *response_ptr = NULL;
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
    printf("Link is down, please check Ethernet cable \r\n");
    tx_thread_suspend(&NxAppThread);
  }

  PRINT_IP_ADDRESS(IpAddress);

  ret = nx_link_vlan_interface_create(&NetXDuoEthIpInstance, "NetX IP Interface 0:1", NX_APP_SECOND_IP_ADDRESS
                                         , NX_APP_DEFAULT_NET_MASK, VLAN_TAG_1, 0, &interface_1);

  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  nx_app_cbs_config();

  tx_thread_sleep(2 * NX_IP_PERIODIC_RATE);

  nx_iperf_entry(&NxAppPool, &NetXDuoEthIpInstance, (UCHAR *)http_stack, sizeof(http_stack), (UCHAR *)iperf_stack, sizeof(iperf_stack));

  /* Run test for 10 seconds*/
  nx_iperf_ctrl_info.TestTime = TEST_TIME;
  /* IPerf UDP server's address */
  nx_iperf_ctrl_info.ip = SERVER_SECOND_IP_ADDRESS;
  nx_iperf_ctrl_info.PacketSize = PACKET_SIZE;
  nx_iperf_ctrl_info.port = SERVER_PORT;
  nx_iperf_ctrl_info.Rate = 10;
  nx_iperf_ctrl_info.version = VERSION_V4;
  nx_iperf_ctrl_info.ctrl_sign = CTRL_SIGN_7;

  printf("Launching IPerf UDP TX test\r\n");
  nx_iperf_udp_tx_test((UCHAR *)iperf_stack,sizeof(iperf_stack));

  memset(ping_data,0x05,sizeof(ping_data));

  /* Form IP address string */
  ip_to_string(SERVER_IP_ADDRESS, ip_addr);

  while(1)
  {
    tx_thread_sleep(NX_IP_PERIODIC_RATE * 1);

    status = nx_icmp_ping(&NetXDuoEthIpInstance, SERVER_IP_ADDRESS,
                          ping_data, sizeof(ping_data),
                          &response_ptr, 1000);

    if(status)
    {
      printf("Failed to ping server on address %s\r\n", ip_addr);
    }else
    {
      nx_packet_release(response_ptr);
      printf("Pinged %s successfully\r\n", ip_addr);
    }
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
        printf("The network cable is connected.\r\n");

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
        printf("The network cable is not connected.\r\n");
        nx_ip_driver_direct_command(&NetXDuoEthIpInstance, NX_LINK_DISABLE,
                                    &actual_status);
      }
    }

    tx_thread_sleep(NX_APP_CABLE_CONNECTION_CHECK_PERIOD);
  }
}


/**
* @brief  Credit Based Shaper configuration function.
* @param none
* @retval none
*/
static VOID nx_app_cbs_config()
{
  NX_SHAPER_CBS_PARAMETER cbs_config;
  UINT ret = NX_SUCCESS;
  UCHAR pcp_list[8];
  UCHAR queue_id_list[8];
  UCHAR pcp;

  /* Init HW Queue Mapping and configure Credit Based Shaper */

  /* Create a CBS instance */
  ret = nx_shaper_create(&(NetXDuoEthIpInstance.nx_ip_interface[0]), &shaper_container, &cbs_shaper, NX_SHAPER_TYPE_CBS, nx_driver_shaper_cbs_entry);
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  /* Get HW Queue mapping */
  ret = nx_shaper_default_mapping_get(&(NetXDuoEthIpInstance.nx_ip_interface[0]), pcp_list, queue_id_list, 8);

  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  /* Set HW Queue mapping : VLAN ID 2 mapped to Queue 1 */
  ret = nx_shaper_mapping_set(&(NetXDuoEthIpInstance.nx_ip_interface[0]), pcp_list, queue_id_list, 8);
  
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }

  memset(&cbs_config, 0, sizeof(NX_SHAPER_CBS_PARAMETER));
  cbs_config.idle_slope = CBS_IDLE_SLOPE;
  cbs_config.hw_queue_id = 1;
  pcp = 2;

  printf(" IDLE SLOPE = %d \r\n", CBS_IDLE_SLOPE);

  /* Set The CBS Configuration */
  ret = nx_shaper_cbs_parameter_set(&(NetXDuoEthIpInstance.nx_ip_interface[0]), &cbs_config, pcp);
  if (ret != NX_SUCCESS)
  {
    Error_Handler();
  }
}

/**
  Utility function to convert IP address into a string, assuming LittleEndianness
*/
static VOID ip_to_string(ULONG ipv4, char* str)
{
  if(str == NULL)
  {
    return;
  }

  sprintf(str, "%lu.%lu.%lu.%lu", (ipv4 >> 24) & 0xff,
                                  (ipv4 >> 16) & 0xff,
                                  (ipv4 >> 8) & 0xff,
                                  (ipv4& 0xff));
}
/* USER CODE END 1 */