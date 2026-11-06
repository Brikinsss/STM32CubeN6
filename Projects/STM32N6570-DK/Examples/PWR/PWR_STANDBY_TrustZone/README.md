## <b>PWR_STANDBY_TrustZone Example Description</b>

How to enter the Standby mode with a joint secure/non-secure application and wake-up from this mode by using the RTC wakeup timer.

#### <b>Description</b>

This project is composed of two sub-projects:

- one for the First Stage BootLoader (FSBL), from the workspace 'PWR_STANDBY_TrustZone_FSBL'
- one for the non-secure application part (PWR_STANDBY_TrustZone_AppliNonSecure).



\
Please remember that on system with security enabled, the system always boots in secure and the secure application is responsible for launching the non-secure application.


- FLEXRAM, SRAM1 and part of external Flash are defined as fully secure
-       FLEXRAM (0x3400'0000 to 0x3406'3FFF) is defined as fully secure RAM area
-       SRAM1 (0x3406'4000 to 0x340F'FFFF) is defined as fully secure RAM area
-       part of external Flash (0x7010'0000 to 0x7017'FFFF) is defined as fully secure ROM area

- SRAM2 and part of external Flash are defined as fully non-secure
-       SRAM2 (0x2410'0000 to 0x241F'FFFF) is defined as non-secure RAM area
-       part of external Flash (0x7018'0000 to 0x701F'FFFF) is defined as non-secure ROM area

\
After power on and bootROM execution,  

 - FSBL executes in AXISRAM1 (0x3400'0000) in the retention memory, the HAL_Init() function is called to reset all the peripherals and initialize the systick used as 1ms HAL timebase.
 - The SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 400 Mhz and the CPU clock at 600 MHz.
 - The example project calls also SCB_EnableICache() and SCB_EnableDCache() functions in order to enable the Layer 1 Core Instruction and Data Caches. This is provided as template implementation that the user may integrate in his application in order to enhance the performance.
 - The FSBL configures the necessary security features for effective resource isolation (RIF, SAU etc..), sets the external memory in execution mode then jumps into the Non-Secure application in external Flash.
 - The non-secure application executes, the green LED blinks at 1Hz for 5 seconds to signify initial run.
 - The retention and RTC wake-up configuration is done.
 - The device enters standby mode.
 - The device is woken up after 5 seconds by an RTC interrupt.
 - The FSBL in retention area (0x3400'0000) executes, re-initializes the external memory, then jumps into the non-secure application.
 - The non-secure application executes, the green LED blinks at 5Hz for 5 seconds to signify exit from standby mode.
 - The device enters standby mode again.
 - The device is again woken up after 5 seconds by an RTC interrupt and the last steps described above infinitely unroll.



#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Reference, Template, FSBL, XIP, TrustZone, Isolation

### <b>Sub-project FSBL directory contents</b>

      - PWR/PWR_STANDBY_TrustZone/FSBL/Inc/main.h                       Header for main.c module
      - PWR/PWR_STANDBY_TrustZone/FSBL/Inc/extmem.h                     Header for extmem.c module
      - PWR/PWR_STANDBY_TrustZone/FSBL/Inc/partition_stm32n657xx.h      Partition file
      - PWR/PWR_STANDBY_TrustZone/FSBL/Inc/stm32n6xx_hal_conf.h         HAL Configuration file
      - PWR/PWR_STANDBY_TrustZone/FSBL/Inc/stm32n6xx_it.h               Interrupt handlers header file
      - PWR/PWR_STANDBY_TrustZone/FSBL/Inc/stm32_extmem_conf.h          External memory manager Configuration file
      - PWR/PWR_STANDBY_TrustZone/FSBL/Src/main.c                       Main program
      - PWR/PWR_STANDBY_TrustZone/FSBL/Src/extmem.c                     Code to initialize external memory
      - PWR/PWR_STANDBY_TrustZone/FSBL/Src/stm32n6xx_hal_msp.c          HAL MSP module
      - PWR/PWR_STANDBY_TrustZone/FSBL/Src/stm32n6xx_it.c               Interrupt handlers
      - PWR/PWR_STANDBY_TrustZone/FSBL/Src/system_stm32n6xx_fsbl.c      STM32N6xx system source file


 ### <b>Sub-project AppliNonSecure directory contents</b>

      - PWR/PWR_STANDBY_RTC_TrustZone/AppliNonSecure/Inc/main.h                      Header for main.c module
      - PWR/PWR_STANDBY_RTC_TrustZone/AppliNonSecure/Inc/stm32n6xx_hal_conf.h        HAL Configuration file
      - PWR/PWR_STANDBY_RTC_TrustZone/AppliNonSecure/Inc/stm32n6xx_it.h              Interrupt handlers header file
      - PWR/PWR_STANDBY_RTC_TrustZone/AppliNonSecure/Inc/stm32n6570_discovery_conf.h BSP Configuration file
      - PWR/PWR_STANDBY_RTC_TrustZone/AppliNonSecure/Src/main.c                      Main program
      - PWR/PWR_STANDBY_RTC_TrustZone/AppliNonSecure/Src/stm32n6xx_hal_msp.c         HAL MSP module
      - PWR/PWR_STANDBY_RTC_TrustZone/AppliNonSecure/Src/stm32n6xx_it.c              Interrupt handlers
      - PWR/PWR_STANDBY_RTC_TrustZone/AppliNonSecure/Src/system_stm32n6xx_ns.c       STM32N6xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32N6 devices.

  - This example has been tested with STMicroelectronics STM32N6570-DK (MB1939)
    board and can be easily tailored to any other supported device
    and development board.

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

  - **MDK-ARM** : To monitor a variable in the live watch window, you must comment out SCB_EnableDCache() in main() function.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Set the boot mode in development mode (BOOT1 switch position is 1-3, BOOT0 switch position doesn't matter)
 - Open your preferred toolchain
 - Select the FSBL workspace
 - Rebuild all files from sub-project FSBL
 - Select the AppliNonSecure workspace
 - Rebuild all files from sub-project AppliNonSecure
 - Resort to CubeProgrammer to add a header to the generated AppliNonSecure binary AppliNonSecure.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin AppliNonSecure.bin -nk -of 0x80000000 -t fsbl -o AppliNonSecure-trusted.bin -hv 2.3 -dump AppliNonSecure-trusted.bin*
   - The resulting binary is AppliNonSecure-trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the non-secure application binary and its header (AppliNonSecure-trusted.bin) at address 0x7018'0000.

\
 To run the template with boot in development mode,

 - Load the FSBL binary in internal RAM using the IDE
 - Run the example

\
This example is meant to run in development mode only.








**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
