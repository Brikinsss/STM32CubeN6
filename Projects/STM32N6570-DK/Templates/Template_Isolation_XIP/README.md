## <b>Template Isolation XIP (eXecute In Place) Description</b>

This project provides a reference TrustZone Isolation XIP template based on the STM32Cube HAL API that can be used to build any firmware application made of a secure binary and of a non-secure binary to execute in external flash memory.

#### <b>Description</b>

This project is composed of four sub-projects:

- one for the First Stage BootLoader (FSBL)
- one for the secure application part (AppliSecure)
- one for the non-secure application part (AppliNonSecure).
- one for the external memory loader part (ExtMemLoader)

Please remember that on system with security enabled, the system always boots in secure and the secure application is responsible for launching the non-secure application.


- FLEXRAM, SRAM1 and part of external Flash are defined as fully secure
-       FLEXRAM (0x3400'0000 to 0x3406'3FFF) is defined as fully secure RAM area
-       SRAM1 (0x3406'4000 to 0x340F'FFFF) is defined as fully secure RAM area
-       part of external Flash (0x7010'0000 to 0x7017'FFFF) is defined as fully secure ROM area

- SRAM2 and part of external Flash are defined as fully non-secure
-       SRAM2 (0x2410'0000 to 0x241F'FFFF) is defined as non-secure RAM area
-       part of external Flash (0x7018'0000 to 0x701F'FFFF) is defined as non-secure ROM area

\
After bootROM execution, the FSBL project executes in internal RAM, ensures proper MPU and caches setting and configures the maximum system clock frequency at 600 MHz. 

Next it configures the external Flash in execution mode. When done, the program counter is set to the secure application entry point in external Flash and the secure application in turn executes.

The secure application configures the RISAF peripheral to set FLEXRAM, SRAM1, SRAM2 and external Flash secure or non-secure attributes.

Finally, the system jumps in non-secure code in external Flash and the green LED (GPIO PO.01) toggles in an infinite loop with a 200-ms period.

The ExtMemLoader is a subproject used to generate a binary library capable of downloading an application to external memory. This binary is called a "Loader" which can be used by the IDE or CubeProgrammer. This project is based on the use of two middelwares: ExtMemManager and ExtMemLoader, it doesn't  have a main function but CubeMX generates an extmemloader_init function responsible for initializing the system.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Reference, Template, FSBL, XIP, TrustZone, Isolation, Loader

### <b>Sub-project FSBL directory contents</b>

      - Templates/Template_Isolation_XIP/FSBL/Inc/main.h                       Header for main.c module
      - Templates/Template_Isolation_XIP/FSBL/Inc/extmem.h                     Header for extmem.c module
      - Templates/Template_Isolation_XIP/FSBL/Inc/partition_stm32n657xx.h      Partition file
      - Templates/Template_Isolation_XIP/FSBL/Inc/stm32n6xx_hal_conf.h         HAL Configuration file
      - Templates/Template_Isolation_XIP/FSBL/Inc/stm32n6xx_it.h               Interrupt handlers header file
      - Templates/Template_Isolation_XIP/FSBL/Inc/stm32_extmem_conf.h          External memory manager Configuration file
      - Templates/Template_Isolation_XIP/FSBL/Src/main.c                       Main program
      - Templates/Template_Isolation_XIP/FSBL/Src/extmem.c                     Code to initialize external memory
      - Templates/Template_Isolation_XIP/FSBL/Src/stm32n6xx_hal_msp.c          HAL MSP module
      - Templates/Template_Isolation_XIP/FSBL/Src/stm32n6xx_it.c               Interrupt handlers
      - Templates/Template_Isolation_XIP/FSBL/Src/system_stm32n6xx_fsbl.c      STM32N6xx system source file

### <b>Sub-project AppliSecure directory contents</b>

      - Templates/Template_Isolation_XIP/AppliSecure/Inc/main.h                      Header for main.c module
      - Templates/Template_Isolation_XIP/AppliSecure/Inc/stm32n6xx_hal_conf.h        HAL Configuration file
      - Templates/Template_Isolation_XIP/AppliSecure/Inc/stm32n6xx_it.h              Interrupt handlers header file
      - Templates/Template_Isolation_XIP/AppliSecure/Inc/partition_stm32n657xx.h     Partition file
      - Templates/Template_Isolation_XIP/AppliSecure/Src/main.c                      Main program
      - Templates/Template_Isolation_XIP/AppliSecure/Src/secure_nsc.c                Secure Non-Secure callable functions implementation file
      - Templates/Template_Isolation_XIP/AppliSecure/Src/stm32n6xx_it.c              Interrupt handlers
      - Templates/Template_Isolation_XIP/AppliSecure/Src/system_stm32n6xx.c          STM32N6xx system source file

### <b>Sub-project AppliNonSecure directory contents</b>

      - Templates/Template_Isolation_XIP/AppliNonSecure/Inc/main.h                      Header for main.c module
      - Templates/Template_Isolation_XIP/AppliNonSecure/Inc/stm32n6xx_hal_conf.h        HAL Configuration file
      - Templates/Template_Isolation_XIP/AppliNonSecure/Inc/stm32n6xx_it.h              Interrupt handlers header file
      - Templates/Template_Isolation_XIP/AppliNonSecure/Inc/stm32n6570_discovery_conf.h BSP Configuration file
      - Templates/Template_Isolation_XIP/AppliNonSecure/Src/main.c                      Main program
      - Templates/Template_Isolation_XIP/AppliNonSecure/Src/stm32n6xx_it.c              Interrupt handlers
      - Templates/Template_Isolation_XIP/AppliNonSecure/Src/system_stm32n6xx_ns.c       STM32N6xx system source file

### <b>Sub-project ExtMemLoader directory contents</b>

      - Templates/Template_Isolation_XIP/ExtMemLoader/Inc/extmem_manager.h             Header for extmem_manager.c module
      - Templates/Template_Isolation_XIP/ExtMemLoader/Inc/extmemloader_init.h          Header for system initialization
      - Templates/Template_Isolation_XIP/ExtMemLoader/Inc/stm32_extmem_conf.h          External memory manager Configuration file
      - Templates/Template_Isolation_XIP/ExtMemLoader/Inc/stm32_extmemloader_conf.h    Header with externam memory device information
      - Templates/Template_Isolation_XIP/ExtMemLoader/Inc/stm32n6xx_hal_conf.h         HAL Configuration file
      - Templates/Template_Isolation_XIP/ExtMemLoader/Src/extmem_manager.c             code to initialize external memory
      - Templates/Template_Isolation_XIP/ExtMemLoader/Src/extmemloader_init.c          code to perform system initialization
      - Templates/Template_Isolation_XIP/ExtMemLoader/Src/stm32n6xx_hal_msp.c          HAL MSP module
      - Templates/Template_Isolation_XIP/ExtMemLoader/Src/system_stm32n6xx_fsbl.c      STM32N6xx system source file


### <b>Hardware and Software environment</b>

  - This template runs on STM32N6 devices.

  - This template has been tested with STMicroelectronics STM32N6570-DK (MB1939)
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

The following section describes how to use the Template Isolation XIP. The ExtMemLoader sub-project is NOT considered.

In order to make the program work, you must do the following :

 - Set the boot mode in development mode (BOOT1 switch position is 1-3, BOOT0 switch position doesn't matter).
 - Open your preferred toolchain
 - Select first the FSBL workspace
 - Rebuild all files from sub-project FSBL (if no modification is done on FSBL project, this step can be done only once)
 - Select the AppliSecure workspace
 - Rebuild all files from sub-project AppliSecure
 - Select the AppliNonSecure workspace
 - Rebuild all files from sub-project AppliNonSecure
 - Resort to CubeProgrammer to add a header to the generated AppliSecure binary Project_s.bin with the following command (depending on your IDE, the binary name can change)
   - *STM32_SigningTool_CLI.exe -bin Project_s.bin -nk -of 0x80000000 -t fsbl -o Project_s-trusted.bin -hv 2.3 -dump Project_s-trusted.bin*
   - The resulting binary is Project_s-trusted.bin.
 - Do the same with AppliNonSecure
   - *STM32_SigningTool_CLI.exe -bin Project_ns.bin -nk -of 0x80000000 -t fsbl -o Project_ns-trusted.bin -hv 2.3 -dump Project_ns-trusted.bin*
   - The resulting binary is Project_ns-trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the secure application binary and its header (Project_s-trusted.bin) in DK board external Flash at address 0x7010'0000 and the non-secure application binary and its header (Project_ns-trusted.bin) at address 0x7018'0000.

\
 To run the template with boot in development mode,

 - Load the FSBL binary in internal RAM using the IDE
 - Run the example

\
To run the template in boot from flash mode,

 - Resort to CubeProgrammer to add a header to the generated binary FSBL.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin FSBL.bin -nk -of 0x80000000 -t fsbl -o FSBL-trusted.bin -hv 2.3 -dump FSBL-trusted.bin*
   - The resulting binary is FSBL-trusted.bin. 
 - With CubeProgrammer, load the FSBL binary and its header (FSBL-trusted.bin) in DK board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from Flash mode.



**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
