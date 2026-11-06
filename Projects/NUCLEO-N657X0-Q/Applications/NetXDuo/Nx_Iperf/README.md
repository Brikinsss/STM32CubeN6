
##  <b>Nx_Iperf Application Description</b>

This application provides an example of Azure RTOS NetXDuo stack usage. It shows performance when using different modes : TCP_server, UDP_server, TCP_client and UDP_client.
The main entry function tx_application_define() is then called by ThreadX during kernel start, at this stage, all NetXDuo resources are created.

 + A NX_PACKET_POOL **NxAppPool** is allocated
 + A NX_IP instance **NetXDuoEthIpInstance** using that pool is initialized
 + A NX_PACKET_POOL **WebServerPool** is allocated
 + The ARP, ICMP and protocols (TCP and UDP) are enabled for the NX_IP instance
 + A DHCP client is created.

The application creates 1 thread :

 + **NxAppThread** (priority 4, PreemtionThreashold 4) : created with the <i>TX_AUTO_START</i> flag to start automatically.

The **NxAppThread** starts and performs the following actions:

  + Starts the DHCP client
  + Waits for the IP address resolution
  + Resumes the *nx_iperf_entry*

The **nx_iperf_entry**, once started:

  + Creates a NetXDuo Iperf demo web page.

The application then creates 4 threads with the same priorities :

   + **thread_tcp_rx_iperf** (priority 1, PreemtionThreashold 1) : created with the <i>TX_AUTO_START</i> flag to start automatically.
   + **thread_tcp_tx_iperf** (priority 1, PreemtionThreashold 1) : created with the <i>TX_AUTO_START</i> flag to start automatically.
   + **thread_udp_rx_iperf** (priority 1, PreemtionThreashold 1) : created with the <i>TX_AUTO_START</i> flag to start automatically.
   + **thread_udp_tx_iperf** (priority 1, PreemtionThreashold 1) : created with the <i>TX_AUTO_START</i> flag to start automatically.

####  <b>Expected success behavior</b>

 + The board IP address is printed on the HyperTerminal
 + When the web HTTP server is successfully started, then the user can test the performance on the web browser after entering the url http://@IP.
 + To execute each Iperf test you must do the following steps and have the expected result in this link https://docs.microsoft.com/en-us/azure/rtos/netx-duo/netx-duo-iperf/chapter3 .

#### <b>Error behaviors</b>

+ The red LED is toggling to indicate any error that have occurred.

#### <b>Assumptions if any</b>

- The Application is using the DHCP to acquire IP address, thus a DHCP server should be reachable by the board in the LAN used to test the application.
- The application is configuring the Ethernet IP with a static predefined <i>MAC Address</i>, make sure to change it in case multiple boards are connected on the same LAN to avoid any potential network traffic issues.
- The _MAC Address_ is defined in the main.c`

```
void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  heth.Init.MACAddr[0] =   0x00;
  heth.Init.MACAddr[1] =   0x80;
  heth.Init.MACAddr[2] =   0xE0;
  heth.Init.MACAddr[3] =   0x01;
  heth.Init.MACAddr[4] =   0x11;
  heth.Init.MACAddr[5] =   0x01;
}
```
#### <b>Known limitations</b>
 - The packet pool is not optimized. It can be less than that by reducing NX_PACKET_POOL_SIZE in file "app_netxduo.h" and NX_APP_MEM_POOL_SIZE in file "app_azure_rtos_config.h". This update can decrease NetXDuo performance.

#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it, by updating the "TX_TIMER_TICKS_PER_SECOND" define in the "tx_user.h" file. The update should be reflected in "tx_initialize_low_level.S" file too.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
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
    either define the RW_IRAM1 region in the ".sct" file
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
       } >RAM AT> RAM
	```

       The simplest way to provide memory for ThreadX is to define a new section, see ._threadx_heap above.
       In the example above the ThreadX heap size is set to 64KBytes.
       The ._threadx_heap must be located between the .bss and the ._user_heap_stack sections in the linker script.
       Caution: Make sure that ThreadX does not need more than the provided heap memory (64KBytes in this example).
       Read more in STM32CubeIDE User Guide, chapter: "Linker script".

    + The "tx_initialize_low_level.S" should be also modified to enable the "USE_DYNAMIC_MEMORY_ALLOCATION" flag.

#### <b>NetX Duo usage hints</b>

- Depending on the application scenario, the total TX and RX descriptors may need to be increased by updating respectively  the "ETH_TX_DESC_CNT" and "ETH_RX_DESC_CNT" in the "stm32n6xx_hal_conf.h", to guarantee the application correct behaviour, but this will cost extra memory to allocate.
- The NetXDuo application needs to allocate the <b> <i> NX_PACKET </i> </b> pool in a dedicated section.
Below is an example of the section declaration for different IDEs.
   + For EWARM ".icf" file
   ```
   define symbol __ICFEDIT_region_NXDATA_start__ = 0x341E6000;
   define symbol __ICFEDIT_region_NXDATA_end__   = 0x341FFFFF;
   define region NXApp_region  = mem:[from __ICFEDIT_region_NXDATA_start__ to __ICFEDIT_region_NXDATA_end__];
   place in NXApp_region { section .NetXPoolSection};
   ```
   + For MDK-ARM
   ```
   RW_NXDriverSection 0x341E6000 0x1A000  {
   *(.NetXPoolSection)
   }
   ```
   + For STM32CubeIDE ".ld" file
   ```
   .nx_data (NOLOAD):
   {
   . = ABSOLUTE(0x341E6000);
   *(.NetXPoolSection)
   } >RAM AT> ROM
   ```

  This section is then used in the <code> app_azure_rtos.c</code> file to force the <code>nx_byte_pool_buffer</code> allocation.

```
/* USER CODE BEGIN NX_Pool_Buffer */

#if defined ( ICCARM ) / IAR Compiler /
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

RTOS, Network, ThreadX, NetXDuo, Iperf, UART

### <b>Hardware and Software environment</b>

  - This application runs on STM32N657X0H3QU devices.
  - This application has been tested with STMicroelectronics NUCLEO-N657X0-Q boards Revision MB1940-N657XOQ-C01 and can be easily tailored to any other supported device and development board.

  - This application uses USART1 to display logs, the hyperterminal configuration is as follows:
      - BaudRate = 115200 baud
      - Word Length = 8 Bits
      - Stop Bit = 1
      - Parity = None
      - Flow control = None

  - **EWARM** : To monitor a variable in the live watch window, you must proceed as follow :
    - Start a debugging session.
    - Open the View > Images.
    - Double-click to deselect the second instance of project.out.

  - **MDK-ARM** : To monitor a variable in the live watch window, you must comment out SCB_EnableDCache() in main() function.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Set the boot mode in development mode (BOOT1 switch position is 2-3, BOOT0 switch position doesn't matter).
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory. Code can be executed in this mode for debugging purposes.
 - Run the [iperf tool] by following steps described in this link https://docs.microsoft.com/en-us/azure/rtos/netx-duo/netx-duo-iperf/chapter3 .

Next, this program can be run in boot from flash mode. This can be done by following the instructions below:

 - Resort to CubeProgrammer to add a header to the generated binary Project.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin Nx_Iperf.bin -nk -of 0x80000000 -t fsbl -o Nx_Iperf_trusted.bin -hv 2.3 -dump Nx_Iperf_trusted.bin*
       - The resulting binary is Nx_Iperf_trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Nx_Iperf-trusted.bin) in Nucleo board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.



**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
