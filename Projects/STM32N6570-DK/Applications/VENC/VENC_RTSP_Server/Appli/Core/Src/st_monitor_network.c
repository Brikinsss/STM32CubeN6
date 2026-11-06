/**
  ******************************************************************************
  * @file           : st_monitor_network.c
  * @brief          : Implement bitrate monitoring
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019(-2025) STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32n6xx_hal.h"
#include "st_monitor_network.h"
#include "tx_api.h"
#include "app_netxduo.h"

extern NX_IP IpInstance;

void monitor_network(void)
{
  ULONG ip_total_packets_sent;
  ULONG ip_total_bytes_sent;
  ULONG ip_total_packets_received;
  ULONG ip_total_bytes_received;
  ULONG ip_invalid_packets;
  ULONG ip_receive_packets_dropped;
  ULONG ip_receive_checksum_errors;
  ULONG ip_send_packets_dropped;
  ULONG ip_total_fragments_sent;
  ULONG ip_total_fragments_received;
  
  nx_ip_info_get(&IpInstance,
                 &ip_total_packets_sent,
                 &ip_total_bytes_sent,
                 &ip_total_packets_received,
                 &ip_total_bytes_received,
                 &ip_invalid_packets,
                 &ip_receive_packets_dropped,
                 &ip_receive_checksum_errors,
                 &ip_send_packets_dropped,
                 &ip_total_fragments_sent,
                 &ip_total_fragments_received);
                 
  //printf("NetX/packet dropped : Rx = %lu/%lu - Tx = %lu/%lu\n", ip_receive_packets_dropped, ip_total_packets_received, ip_send_packets_dropped, ip_total_packets_sent);
  
}