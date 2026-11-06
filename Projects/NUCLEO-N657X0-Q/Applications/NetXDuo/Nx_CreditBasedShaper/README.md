##  <b>Nx_CreditBasedShaper Application Description</b>

This application provides an example of Azure RTOS NetX/NetXDuo stack usage. It is used to demonstrate the effect of Credit-Based Shaper (IEEE 802.1-Qav) on Ethernet transmission performance.

The main entry function tx_application_define() is then called by ThreadX during kernel start, at this stage, all NetX resources are created.

 + A <i> NX_PACKET_POOL </i> is allocated
 + A <i>NX_IP</i> instance using that pool is initialized
 + The <i>ARP</i>, <i>ICMP</i>, <i>UDP</i> and <i>TCP</i> protocols are enabled for the <i>NX_IP</i> instance

The application creates an IP instance with a fixed address NX_APP_DEFAULT_IP_ADDRESS, which will transmit normal frames via Queue 0.
It also creates a secondary interface with a fixed address NX_APP_SECOND_IP_ADDRESS, which will transmit tagged frames (VLAN = 2) via Queue 1.

It launches an IPerf TX and starts sending UDP traffic on the secondary interface, and pinging on the primary interface.
All packets transmitted via Queue 1 are configured to support Credit-Based Shaping.

####  <b>Expected success behavior</b>

 + The board IP address is printed on the HyperTerminal
 + The CBS_IDLE_SLOPE value is printed on the HyperTerminal
 + TX UDP iperf test is launched
 + The board pings the RX side every second.

Iperf throughput seen on the Rx side should not exceed the value of CBS_IDLE_SLOPE.

 + Below are the expected messages on the HyperTerminal:
 ```
Nx_CreditBasedShaper application started..
STM32 IpAddress: 192.168.100.6
IDLE SLOPE = 70
Launching IPerf UDP TX Test
Pinged 192.168.100.3 successfully
```

#### <b>Error behaviors</b>

+ The red LED is turned ON


#### <b>Assumptions if any</b>

- The application is configuring the Ethernet IP with a static predefined <i>MAC Address</i>, make sure to change it in case multiple boards are connected on the same LAN to avoid any potential network traffic issues.
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
#### <b>Known limitations</b>
None

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

#### <b>NetX Duo usage hints</b>

- Depending on the application scenario, the total TX and RX descriptors may need to be increased by updating respectively  the "ETH_TX_DESC_CNT" and "ETH_RX_DESC_CNT" in the "stm32n6xx_hal_conf.h", to guarantee the application correct behaviour, but this will cost extra memory to allocate.
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

RTOS, Network, ThreadX, NetXDuo, TSN, CBS, PTP, UART

### <b>Hardware and Software environment</b>

  - This application runs on STM32N657xx devices.
  - This application has been tested with STMicroelectronics NUCLEO-N657X0-Q boards Revision MB1940-N657XOQ-C01 and can be easily tailored to any other supported device and development board.

  - This application uses USART3 to display logs, the hyperterminal configuration is as follows:
      - BaudRate = 115200 baud
      - Word Length = 8 Bits
      - Stop Bit = 1
      - Parity = None
      - Flow control = None

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Set the boot mode in development mode (BOOT1 switch position is 2-3, BOOT0 switch position doesn't matter).
 - Rebuild all files and load your image into target memory. Code can be executed in this mode for debugging purposes.
 - In remote PC you need to :
   + Create a new instance to handle tagged frames (vlan 2).
     Cmd Example : sudo ip link add link eno1 name eno1.2 type vlan id 2
   + Address and netmask configuration.
     Cmd Example : sudo ifconfig eno1.2 192.168.200.3 netmask 255.255.255.0 up
   + Run an Iperf UDP server to capture packets received from NX_APP_SECOND_IP_ADDRESS.
 - Launch the application

Next, this program can be run in boot from flash mode. This can be done by following the instructions below:

 - Resort to CubeProgrammer to add a header to the generated binary Project.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin Nx_CreditBasedShaper.bin -nk -of 0x80000000 -t fsbl -o Nx_CreditBasedShaper_trusted.bin -hv 2.3 -dump Nx_CreditBasedShaper_trusted.bin*
       - The resulting binary is Nx_CreditBasedShaper_trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Nx_CreditBasedShaper_trusted.bin) in Nucleo board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.







**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
