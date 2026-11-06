## <b>Tx_SecureLEDToggle_TrustZone Application Description</b>

This application provides an example of Azure RTOS ThreadX stack usage, it shows how to develop an application using the ThreadX when the TrustZone feature is enabled.

The application is running ThreadX thread in non-secure context that is invoking a secure function to toggle a led each 1 sec.

The main entry function tx_application_define() is called by ThreadX during kernel start, at the non-secure stage, the application creates 1 thread :
  - MainThread (Prio : 5; Preemption Threshold : 5)

Once started, the <b>MainThread</b> toggle the LED_GREEN before sleeping for 1s.

This project is composed of three sub-projects:

- one for the First Stage BootLoader (FSBL)
- one for the secure application part (Tx_SecureLEDToggle_TrustZone_S)
- one for the non-secure application part (Tx_SecureLEDToggle_TrustZone_NS).

Please remember that on system with security enabled, the system always boots in secure and the secure application is responsible for launching the non-secure application.
- the SystemInit() function in secure application sets up the SAU/IDAU, FPU and secure/non-secure interrupts allocations defined in partition_stm32n657xx.h file.

This project shows how to switch between secure and non-secure applications thanks to the system isolation performed to split the internal SRAM memories into two halves:

- FLEXRAM and SRAM1 are defined as fully secure
-       FLEXRAM (0x3400'0000 to 0x3406'3FFF) is defined as fully secure ROM area
-       SRAM1 (0x3406'4000 to 0x340F'FFFF) is defined as fully secure RAM area

- SRAM2 is defined as fully non-secure
-       (0x2410'0000 to 0x2417'FFFF) is defined as non-secure ROM area
-       (0x2418'0000 to 0x241F'FFFF) is defined as non-secure RAM area

\
After bootROM execution, the FSBL project executes in internal RAM, ensures proper MPU and caches setting and configures the maximum system clock frequency at 600 MHz.

Next, it copies the secure application binary then the non-secure application binary from external Flash to internal RAM. When done, the program counter is set to the secure application entry point and the secure application in turn executes.

The secure application configures the RISAF peripheral to set FLEXRAM, SRAM1 and SRAM2 secure or non-secure attributes.

Finally, the system jumps in non-secure code and the blue LED (GPIO PG.10) toggles in an infinite loop with a 100-ms period.

####  <b>Expected success behavior</b>

LED_GREEN toggles every 1s

#### <b>Error behaviors</b>

LED_RED toggles every 1 second if any error occurs.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

#### <b>Assumptions if any</b>
None

#### <b>Known limitations</b>
None

#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it in the "tx_user.h", the "TX_TIMER_TICKS_PER_SECOND" define,but this should be reflected in "tx_initialize_low_level.S" file too.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, that is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all applications ThreadX related resources (threads, semaphores, memory pools...)  but it should not in any way contain a system API call (HAL or BSP).
 - Using dynamic memory allocation requires to apply some changes to the linker file.
   ThreadX needs to pass a pointer to the first free memory location in RAM to the tx_application_define() function,
   using the "first_unused_memory" argument.
   This require changes in the linker files to expose this memory location.
    + For EWARM add the following section into the .icf file:
     ```
	 place in RAM_region    { last section FREE_MEM };
	 ```
    + For MDK-ARM:
	```
    either define the RW_IRAM1 region in the ".sct" file
    or modify the line below in "tx_low_level_initilize.S to match the memory region being used
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

       The simplest way to provide memory for ThreadX is to define a new section, see ._threadx_heap above.
       In the example above the ThreadX heap size is set to 64KBytes.
       The ._threadx_heap must be located between the .bss and the ._user_heap_stack sections in the linker script.
       Caution: Make sure that ThreadX does not need more than the provided heap memory (64KBytes in this example).
       Read more in STM32CubeIDE User Guide, chapter: "Linker script".

    + The "tx_initialize_low_level.S" should be also modified to enable the "USE_DYNAMIC_MEMORY_ALLOCATION" flag.

### <b>Keywords</b>

RTOS, ThreadX, Threading, TrustZone, LRUN, FSBL, Isolation, Secure, Non-secure.


### <b>Hardware and Software environment</b>

  - This Application runs on STM32N657X0H3QU devices.
  - This template has been tested with STMicroelectronics NUCLEO-N657X0 boards revision MB1940-N657X0-C01 board and can be easily tailored to any other supported device and development board.

  - The following OTP fuses are set in this template:

    - VDDIO3_HSLV=1     I/O XSPIM_P2 High speed option enabled

**WARNING**

When OTP fuses are set, they can not be reset.

If the OTP fuseVDDIO3_HSLV is not already set, run the template once with boot mode set in development mode and NO_OTP_FUSE compilation switch disabled
(otherwise, it isn't possible to benefit from the maximum transfer rate to access the external flash).

For the following runs, select NO_OTP_FUSE option by default using the preprocessor definition in the IDE.

  - **EWARM** : To monitor a variable in the live watch window, you must proceed as follow :
    - Start a debugging session.
    - Open the View > Images.
    - Double-click to deselect the second instance of project.out.

###  <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Set the boot mode in development mode (BOOT1 switch position is 2-3, BOOT0 switch position doesn't matter).
 - Open your preferred toolchain
 - Select first the FSBL workspace
 - Rebuild all files from sub-project FSBL (if no modification is done on FSBL project, this step can be done only once)
 - Select the Project_s workspace
 - Rebuild all files from sub-project Project_s
 - Select the Project_ns workspace
 - Rebuild all files from sub-project Project_ns
 - Resort to CubeProgrammer to add a header to the generated App_Secure binary Project.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin Tx_SecureLEDToggle_TrustZone_S.bin -nk -of 0x80000000 -t fsbl -o Tx_SecureLEDToggle_TrustZone_S-trusted.bin -hv 2.3 -dump Tx_SecureLEDToggle_TrustZone_S-trusted.bin*
       - The resulting binary is Project-trusted.bin.
 - Do the same with App_NonSecure
    - *STM32_SigningTool_CLI.exe -bin Tx_SecureLEDToggle_TrustZone_NS.bin -nk -of 0x80000000 -t fsbl -o Tx_SecureLEDToggle_TrustZone_NS-trusted.bin -hv 2.3 -dump Tx_SecureLEDToggle_TrustZone_NS-trusted.bin*
       - The resulting binary is Project_ns-trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the secure application binary and its header (Tx_SecureLEDToggle_TrustZone_S-trusted.bin) in Nucleo board external Flash at address 0x7010'0000 and the non-secure application binary and its header (Tx_SecureLEDToggle_TrustZone_NS-trusted.bin) at address 0x7018'0000.

\
 To run the template with boot in development mode,

 - Load the FSBL binary in internal RAM using the IDE
 - Run the example

\
To run the template in boot from flash mode,

 - Resort to CubeProgrammer to add a header to the generated binary FSBL.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin FSBL.bin -nk -of 0x80000000 -t fsbl -o FSBL-trusted.bin -hv 2.3 -dump FSBL-trusted.bin*
       - The resulting binary is FSBL-trusted.bin.
 - With CubeProgrammer, load the FSBL binary and its header (FSBL-trusted.bin) in Nucleo board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from Flash mode.

**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
