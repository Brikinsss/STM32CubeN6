/**
******************************************************************************
* @file          st_monitor_trc.c
* @author        MCD Application Team
* @brief         Trace Alyser recording
*******************************************************************************
* @attention
*
* Copyright (c) 2019(-202) STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
********************************************************************************
*/

#include <trcRecorder.h>


void  st_monitor_trc_init(void )
{
  xTraceEnable(TRC_START);
}
