##  <b>Nx_PTP_Client Application Description</b>

 This application provides an example of Azure RTOS NetX/NetXDuo stack usage. It shows how to develop a PTP Master and Client applications to synchronize using the NetX PTP addon : 

The main entry function tx_application_define() is then called by ThreadX during kernel start, at this stage, all NetX resources are created.

 + A <i> NX_PACKET_POOL </i> is allocated
 + A <i>NX_IP</i> instance using that pool is initialized
 + The <i>ARP</i>, <i>ICMP</i>, <i>UDP</i> and <i>TCP</i> protocols are enabled for the <i>NX_IP</i> instance
 + A <i>DHCP</i> client is created.

The application then creates 2 threads with the same priorities:

 + **NxAppThread** (priority 10, Preemption Threshold  10) : created with the <i>TX_AUTO_START</i> flag to start automatically.
 + **AppPTPThread** (priority 10, Preemption Threshold  10) : created with the <i>TX_DONT_START</i> flag to be started later.

The **NxAppThread** starts and performs the following actions:

  + Starts the DHCP client
  + Waits for the IP address resolution
  + Resumes the **AppPTPThread**

The **AppPTPThread**, once started:

  + Creates and starts a <i>PTP client</i>
  + if the **NX_PTP_ENABLE_MASTER** is set : start PTP Master 
  + Displays current system time prior to synchronization
  + Once a <i>master clock</i> is found, a message is displayed
  + Time is synchronized and should be the same as the PC's (there may be a 37 second offset, depending on PC client config)

##  <b>Expected success behavior</b>

 + The board IP address is printed on the HyperTerminal
 + An incorrect system is displayed each second, for example '1/01/1970 00:00:03.016048900'
 + After synchronization, the correct date/time is displayed '20/05/2024 14:49:07.293633659'

## <b>Error behaviors</b>

+ The data/time printed to the terminal remains incorrect.
+ In case of a bad configuration, a 'master timeout' message could be printed to the terminal.


## <b>Assumptions if any</b>

- The application uses the DHCP to acquire an IP address, thus a DHCP server should be reachable by the board in the LAN used to test the application.
- The application configure the Ethernet IP with a static predefined <i>MAC Address</i>, make sure to change it in case multiple boards are connected on the same LAN to avoid any potential network traffic issues.
- The <i>MAC Address</i> is defined in the `main.c`

```
void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH1;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE0;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x10;
  MACAddr[5] = 0x00;
```
### <b>Known limitations</b>
None

### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it, by updating the "TX_TIMER_TICKS_PER_SECOND" define in the "tx_user.h" file. The update should be reflected in "tx_initialize_low_level.S" file too.
 - ThreadX disable all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...)  but it should not in any way contain a system API call (HAL or BSP).
 - Using dynamic memory allocation requires to apply some changes to the linker file.
   ThreadX needs to pass a pointer to the first free memory location in RAM to the tx_application_define() function,
   using the "first_unused_memory" argument.
   This requires changes in the linker files to expose this memory location.
    + For EWARM add the following section into the .icf file:
     ```
     place in RAM_region    { last section FREE_MEM };
     ```
    + For MDK-ARM:
    ```
    Either define the RW_IRAM1 region in the ".sct" file
    or modify the line below in "tx_initialize_low_level.S to match the memory region being used
    LDR r1, =|Image$$RW_IRAM1$$ZI$$Limit|
    ```
    + For STM32CubeIDE add the following section into the .ld file:
    ```
    ._threadx_heap :
      {
         . = ALIGN(8);
         __RAM_segment_used_end__ = .;
         . = . + 64K;
         . = ALIGN(8);
       } >RAM_D1 AT> RAM_D1
    ```

### <b>NetX Duo usage hints</b>

- Depending on the application scenario, the total TX and RX descriptors may need to be increased by updating respectively  the "ETH_TX_DESC_CNT" and "ETH_RX_DESC_CNT" in the "stm32n6xx_hal_eth.h", to guarantee the application correct behaviour, but this will cost extra memory to allocate.
- The NetXDuo application needs to allocate the <b> <i> NX_PACKET </i> </b> pool in a dedicated section.
Below is an example of the section declaration for different IDEs.
   + For EWARM ".icf" file
   ```
   define symbol __ICFEDIT_region_NXDATA_start__ = 0x341F1000;
   define symbol __ICFEDIT_region_NXDATA_end__   = 0x341FFFFF;
   define region NXApp_region  = mem:[from __ICFEDIT_region_NXDATA_start__ to __ICFEDIT_region_NXDATA_end__];
   place in NXApp_region { section .NetXPoolSection};
   ```

   + For MDK-ARM add the following section into the .sct file:
   ```
    RW_NXDriverSection 0x341F1000 0xF000  {
      *(.NetXPoolSection)
    }
  ```
  
   + For STM32CubeIDE add the following section into the .ld file:
   ```
    .nx_data (NOLOAD):
    {
        . = ABSOLUTE(0x341F1000);
        *(.NetXPoolSection)

    } >RAM
   ```
  This section is then used in the <code> app_azure_rtos.c</code> file to force the <code>nx_byte_pool_buffer</code> allocation.

    ```
      /* USER CODE BEGIN NX_Pool_Buffer */

      #if defined ( __ICCARM__ ) /* IAR Compiler */
      #pragma location = ".NetXPoolSection"

      #else /* GNU and AC6 compilers */
      __attribute__((section(".NetXPoolSection")))

      #endif

      /* USER CODE END NX_Pool_Buffer */
      static UCHAR  nx_byte_pool_buffer[NX_APP_MEM_POOL_SIZE];
      static TX_BYTE_POOL nx_app_byte_pool;
    ```
For more details about the MPU configuration please refer to the [AN4838](https://www.st.com/resource/en/application_note/dm00272912-managing-memory-protection-unit-in-stm32-mcus-stmicroelectronics.pdf)

### <b>Keywords</b>

RTOS, Network, ThreadX, NetXDuo, PTP, UART

## <b>Hardware and Software environment</b>

  - This application runs on NUCLEO-N657X0-Q devices.
  - This application has been tested with STMicroelectronics NUCLEO-N657X0-Q boards and can be easily tailored to any other supported device and development board.

  - This application uses USART1 to display logs, the hyperterminal configuration is as follows:
      - BaudRate = 115200 baud
      - Word Length = 8 Bits
      - Stop Bit = 1
      - Parity = None
      - Flow control = None

## <b>How to use it ?</b>

The Nx_PTP_Client application supports four different PTP modes :

- PTP Client Ethernet 

- PTP Client UDP 

- PTP Master Ethernet

- PTP Master UDP

Each of which can be configured as defined below : 

### <b> PTP Client Ethernet </b>
In order to make the program work, you must do the following :

 1. Run the PTP service on the PC side, specifying gPTP configuration. For example using the <b> <i> ptp4l  </i> </b> tool with an
 interface name 'eno1':
```
  ptp4l -i eno1 -2mH --asCapable=true
```

 2. set the macro "NX_PTP_CLIENT_TRANSPORT_ETHERNET" to 1 and "NX_PTP_CLIENT_TRANSPORT_UDP" to 0 

 3. Rebuild all files and load your image into target memory. Code can be executed in this mode for debugging purposes.

 4. The board clock should be synchronized with the PTP Master

### <b> PTP Client UDP </b>
In order to make the program work, you must do the following :

 1. Run the PTP service on the PC side, specifying gPTP configuration. For example using the <b> <i> ptp4l </i> </b> tool with an
 interface name 'eno1':
 ```
  ptp4l -i eno1 -4mH --asCapable=true
```

 2. set the macro "NX_PTP_CLIENT_TRANSPORT_UDP" to 1 and "NX_PTP_CLIENT_TRANSPORT_ETHERNET" to 0 

 3. Rebuild all files and load your image into target memory. Code can be executed in this mode for debugging purposes.

 4. The board clock should be synchronized with the PTP Master

### <b> PTP Master Ethernet </b>
In order to make the program work, you must do the following :

 1. set the macro "NX_PTP_ENABLE_MASTER"

 2. set the macro "NX_PTP_CLIENT_TRANSPORT_ETHERNET" to 1 and "NX_PTP_CLIENT_TRANSPORT_UDP" to 0 

 3. Rebuild all files and load your image into target memory. Code can be executed in this mode for debugging purposes.

 4. run an ethernet PTP Client on the same LAN
 
 5. the client must synchronize its clock with the NUCLEO-N657X0-Q PTP Master

### <b> PTP Master UDP </b>
 
In order to make the program work, you must do the following :

 1. set the macro "NX_PTP_ENABLE_MASTER"

 2. set the macro "NX_PTP_CLIENT_TRANSPORT_UDP" to 1 and "NX_PTP_CLIENT_TRANSPORT_ETHERNET" to 0 

 3. Rebuild all files and load your image into target memory. Code can be executed in this mode for debugging purposes.

 4. run an UDP PTP Client on the same LAN
 
 5. the client must synchronize its clock with the NUCLEO-N657X0-Q PTP Master
 




**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
