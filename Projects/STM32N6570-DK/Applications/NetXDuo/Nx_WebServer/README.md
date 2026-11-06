
## <b>Nx_WebServer Application Description</b>

This application provides an example of Azure RTOS NetX Duo stack usage on STM32N6570-DK board, it shows how to develop Web HTTP server based application.
The application is designed to load files and dyncamic web pages stored in SD card using a Web HTTP server, the code provides all required features to build a compliant Web HTTP Server.
The main entry function tx_application_define() is called by ThreadX during kernel start, at this stage, all NetX and FileX resources are created.

 + A NX_PACKET_POOL **NxAppPool** is allocated
 + A NX_IP instance **NetXDuoEthIpInstance** using that pool is initialized
 + A NX_PACKET_POOL **WebServerPool** is allocated
 + A NX_WEB_HTTP_SERVER **HTTPServer** instance using that pool is initialized
 + The ARP, ICMP and protocols (TCP and UDP) are enabled for the NX_IP instance
 + A DHCP client is created.

The application then creates 2 threads with different priorities:

 + **NxAppThread** (priority 10, PreemtionThreashold 10) : created with the TX_AUTO_START flag to start automatically.
 + **AppServerThread** (priority 5, PreemtionThreashold 5) : created with the TX_DONT_START flag to be started later.
 + **LedThread** (priority 15, PreemtionThreashold 15) : created with the TX_DONT_START flag to be started later.

The **NxAppThread** starts and performs the following actions:

  + Starts the DHCP client
  + Waits for the IP address resolution
  + Resumes the **AppServerThread**

The **AppServerThread**, once started:

  + Fx_media_open.
  + Starts HTTP server.
  + Each command coming from client (browser) is treated on the callback webserver_request_notify_callback.

The **LedThread**, once resumed from the dashboard:

  + Green LED is toggling & message is printed on HyperTerminal.

#### <b>Expected success behavior</b>

 + The board IP address "IP@" is printed on the HyperTerminal
 + Home page is well displayed on the browser after entering the url: http://IP@/about.html
 + Dashboard is well displayed on the browser after entering the url: http://IP@/dashboard.html
 + On dashboard, You can command green LED and watch some infos about threadx and netxduo.

#### <b>Error behaviors</b>

If the WEB HTTP server is not successfully started, an error message is printed on UART, the green LED stays OFF and the red LED toggles every 1 second.
In case of other errors, the Web HTTP server does not operate as designed (Files stored in the SD card are not loaded in the web browser).

#### <b>Assumptions if any</b>

- The uSD card must be plugged before starting the application.
- The board must be in a DHCP Ethernet network.
- The application is configuring the Ethernet IP with a static predefined _MAC Address_, make sure to change it in case multiple boards are connected on the same LAN to avoid any potential network traffic issues.
- The _MAC Address_ is defined in the `main.c`

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
}
```

#### <b>Known limitations</b>

Hotplug is not implemented for this example, that is, the SD card is expected to be inserted before application running.

#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it, by updating the "TX_TIMER_TICKS_PER_SECOND" define in the "tx_user.h" file. The update should be reflected in "tx_initialize_low_level.S" file too.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...) but it should not in any way contain a system API call (HAL or BSP).
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

#### <b>FileX/LevelX usage hints</b>

- When calling the fx_media_format() API, it is highly recommended to understand all the parameters used by the API to correctly generate a valid filesystem.
- FileX is using data buffers, passed as arguments to fx_media_open(), fx_media_read() and fx_media_write() API it is recommended that these buffers are multiple of sector size and "32 bytes" aligned to avoid cache maintenance issues.

#### <b>NetX Duo usage hints</b>

- Depending on the application scenario, the total TX and RX descriptors may need to be increased by updating respectively the "ETH_TX_DESC_CNT" and "ETH_RX_DESC_CNT" in the "stm32n6xx_hal_conf.h", to guarantee the application correct behaviour, but this will cost extra memory to allocate.
- The NetXDuo application needs to allocate the <b> <i> NX_PACKET </i> </b> pool in a dedicated section.
Below is an example of the section declaration for different IDEs.
   + For EWARM ".icf" file
   ```
   define symbol __ICFEDIT_region_NXDATA_start__ = 0x341D4200;
   define symbol __ICFEDIT_region_NXDATA_end__   = 0x341F01FF;
   define region NXApp_region  = mem:[from __ICFEDIT_region_NXDATA_start__ to __ICFEDIT_region_NXDATA_end__];
   place in NXApp_region { section .NetXPoolSection};
   ```
   + For MDK-ARM
   ```
   RW_NXServerPoolSection 0x341D4200 0x1C000 {
   *(.NxServerPoolSection)
   }

   RW_NXDriverSection 0x341F0200 0xFE00 {
   *(.NetXPoolSection)
   }
   ```
   + For STM32CubeIDE ".ld" file
   ```
   .nx_data (NOLOAD):
   {
      . = ABSOLUTE(0x341D4200);
      *(.NxServerPoolSection)

      . = ABSOLUTE(0x341F0200);
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

RTOS, ThreadX, Network, NetxDuo, Web HTTP Server, FileX, File ,SDMMC, UART

### <b>Hardware and Software environment</b>

  - This application runs on STM32N657X0H3QU devices.
  - This application has been tested with STMicroelectronics STM32N6570-DK boards Revision: MB1939-N6570-C01
    and can be easily tailored to any other supported device and development board.
  - This application uses USART1 to display logs, the hyperterminal configuration is as follows:
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - Stop Bit = 1
    - Parity = None
    - Flow control = None

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

  - Set the boot mode in development mode (BOOT1 switch position is 1-3, BOOT0 switch position doesn't matter).
  - Open your preferred toolchain
  - Rebuild all files and load your image into target memory. Code can be executed in this mode for debugging purposes.
  - Run the application

 Next, this program can be run in boot from flash mode. This is done by following the instructions below:

 - Resort to CubeProgrammer to add a header to the generated binary Nx_WebServer.bin with the following command
      - *STM32_SigningTool_CLI.exe -bin Nx_WebServer.bin -nk -of 0x80000000 -t fsbl -o Nx_WebServer-trusted.bin -hv 2.3 -dump Nx_WebServer-trusted.bin*
   - The resulting binary is Nx_WebServer-trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Nx_WebServer-trusted.bin) in DK board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.


**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
