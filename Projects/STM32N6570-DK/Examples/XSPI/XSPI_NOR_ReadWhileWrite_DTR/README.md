## <b>XSPI_NOR_ReadWhileWrite_DTR Example Description</b>

How to write and read data on XSPI NOR memory (bank1) when code is executed from bank0

This example demonstrates the Read While Write feature using Macronix NOR Flash
which supports simultaneous Read and Write operations (WRBI, WRCT, WRCF)

- Project structure:
  - FSBL Sub-project: After bootROM execution, the FSBL project executes in internal RAM, 
    ensures proper MPU, caches and clock setting then configures the external Flash in execution mode.
    When done, the program counter is set to the application entry point and the application in turn executes.
  - Appli Sub-Project: Firmware application executed from external FLash (Bank0).
- This project is targeted to run on STM32N657X0H3QU device on STM32N6570-DK board from STMicroelectronics.
  The HAL_Init() function is called at the beginning of the main program to reset all peripherals and initialize
  the systick used as a 1ms HAL timebase.
- This project runs from the external Flash memory. It is launched from a first boot stage and inherits
  from this boot project configuration (caches, MPU regions [regions 0, 1 and 2 here], system clock at
  600 MHz and external memory interface at the highest speed).

Program Execution from Bank0:

 1. Erase Memory (Bank1 area)
    - Enable Write Operation
    - Erasing Block(64KB) on bank1
    - Wait for end of block erase in bank1

 2. Write data on Bank1 using RWW feature
    - Enable Write Operation on NOR Flash (Global)
    - Write Buffer Initial command (WRBI)
    - Write Buffer Continue command (WRCT)
    - Write Buffer Confirm (WRCF)
    - Wait for end of programming in bank1

 3. Read and compare data previously written
    - Read data previously written on bank1
    - Compare results

STM32N6570-DK board's LEDs are used to monitor the example status:

- LED_GREEN is ON when the checked data is correct.
- LED_RED is ON when the checked data is correct.

Error_Handler() function is called (infinite loop) if an error is returned by the HAL API.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Memory, XSPI, Read, Write, RWW, XIP, NOR, DTR Mode, DMA

### <b>Sub-project FSBL directory contents</b>

File | Description
 --- | ---
XSPI/XSPI_NOR_ReadWhileWrite_DTR/FSBL/Inc/main.h                       |  Header for main.c module
XSPI/XSPI_NOR_ReadWhileWrite_DTR/FSBL/Inc/extmem_manager.h             |  Header for extmem_manager.c module
XSPI/XSPI_NOR_ReadWhileWrite_DTR/FSBL/Inc/stm32n6xx_hal_conf.h         |  HAL Configuration file
XSPI/XSPI_NOR_ReadWhileWrite_DTR/FSBL/Inc/stm32n6xx_it.h               |  Interrupt handlers header file
XSPI/XSPI_NOR_ReadWhileWrite_DTR/FSBL/Inc/stm32n6570_discovery_conf.h  |  BSP Configuration file
XSPI/XSPI_NOR_ReadWhileWrite_DTR/FSBL/Inc/stm32_extmem_conf.h          |  External memory manager Configuration file
XSPI/XSPI_NOR_ReadWhileWrite_DTR/FSBL/Src/main.c                       |  Main program
XSPI/XSPI_NOR_ReadWhileWrite_DTR/FSBL/Src/extmem_manager.c             |  code to initialize external memory
XSPI/XSPI_NOR_ReadWhileWrite_DTR/FSBL/Src/stm32n6xx_hal_msp.c          |  HAL MSP module
XSPI/XSPI_NOR_ReadWhileWrite_DTR/FSBL/Src/stm32n6xx_it.c               |  Interrupt handlers
XSPI/XSPI_NOR_ReadWhileWrite_DTR/FSBL/Src/system_stm32n6xx_fsbl.c      |  STM32N6xx system source file

### <b>Sub-project Appli directory contents</b>

File | Description
 --- | ---
XSPI/XSPI_NOR_ReadWhileWrite_DTR/Appli/Inc/main.h                      |  Header for main.c module
XSPI/XSPI_NOR_ReadWhileWrite_DTR/Appli/Inc/stm32n6xx_hal_conf.h        |  HAL Configuration file
XSPI/XSPI_NOR_ReadWhileWrite_DTR/Appli/Inc/stm32n6xx_it.h              |  Interrupt handlers header file
XSPI/XSPI_NOR_ReadWhileWrite_DTR/Appli/Inc/stm32n6570_discovery_conf.h |  BSP Configuration file
XSPI/XSPI_NOR_ReadWhileWrite_DTR/Appli/Inc/partition_stm32n657xx.h    |   Partition file
XSPI/XSPI_NOR_ReadWhileWrite_DTR/Appli/Src/main.c                      |  Main program
XSPI/XSPI_NOR_ReadWhileWrite_DTR/Appli/Src/stm32n6xx_hal_msp.c         |  HAL MSP module
XSPI/XSPI_NOR_ReadWhileWrite_DTR/Appli/Src/stm32n6xx_it.c              |  Interrupt handlers
XSPI/XSPI_NOR_ReadWhileWrite_DTR/Appli/Src/system_stm32n6xx_s.c        |  STM32N6xx system source file
XSPI/XSPI_NOR_ReadWhileWrite_DTR/Appli/Src/secure_nsc.c                |  STM32N6xx non-secure callable APIs file


### <b>Hardware and Software environment</b>

  - This template runs on STM32N6 devices.

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

 - Set the boot mode in development mode (BOOT1 switch position is 1-3, BOOT0 switch position doesn't matter).
 - Open your preferred toolchain
 - Select first the FSBL workspace
 - Rebuild all files from sub-project FSBL (if no modification is done on FSBL project, this step can be done only once)
 - Select the Appli workspace
 - Rebuild all files from sub-project Appli
 - Resort to CubeProgrammer to add a header to the generated Appli binary XSPI_NOR_ReadWhileWrite_DTR_Appli.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin XSPI_NOR_ReadWhileWrite_DTR_Appli.bin -nk -of 0x80000000 -t fsbl -o XSPI_NOR_ReadWhileWrite_DTR_Appli-trusted.bin -hv 2.3 -dump XSPI_NOR_ReadWhileWrite_DTR_Appli-trusted.bin*
   - The resulting binary is XSPI_NOR_ReadWhileWrite_DTR_Appli-trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the Appli binary and its header (XSPI_NOR_ReadWhileWrite_DTR_Appli-trusted.bin) in DK board external Flash at address 0x7010'0000.
 - Load the FSBL binary in internal RAM using the IDE
 - Run the example

 Next, this program can be run in boot from flash mode. This is done by following the instructions below:

 - Resort to CubeProgrammer to add a header to the generated binary XSPI_NOR_ReadWhileWrite_DTR_FSBL.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin XSPI_NOR_ReadWhileWrite_DTR_FSBL.bin -nk -of 0x80000000 -t fsbl -o XSPI_NOR_ReadWhileWrite_DTR_FSBL-trusted.bin -hv 2.3 -dump XSPI_NOR_ReadWhileWrite_DTR_FSBL-trusted.bin*
   - The resulting binary is XSPI_NOR_ReadWhileWrite_DTR_FSBL-trusted.bin.
 - In resorting again to CubeProgrammer, load the FSBL binary and its header (XSPI_NOR_ReadWhileWrite_DTR_FSBL-trusted.bin) in DK board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.



