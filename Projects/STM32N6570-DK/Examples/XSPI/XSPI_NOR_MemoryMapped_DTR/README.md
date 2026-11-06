## <b>XSPI_NOR_MemoryMapped_DTR Example Description</b>

How to use a XSPI NOR memory in memory-mapped mode.

This example describes how to erase a part of an XSPI NOR memory, write data in indirect mode 
and access to XSPI NOR memory in memory-mapped mode to check the data in an infinite loop.
The memory is configured in octal DTR mode.

- This project is targeted to run on STM32N657X0H3QU device on STM32N6570-DK board from STMicroelectronics.  
- The FSBL project configures the maximum system clock frequency at 600 MHz.  
LED_GREEN toggles each time a new comparison is good.  
LED_RED is ON as soon as a comparison error occurs.  
LED_RED toggles as soon as an error is returned by HAL API.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Memory, XSPI, Read, Write, Memory Mapped, NOR, DTR Mode

### <b>Directory contents</b>

  - XSPI/XSPI_NOR_MemoryMapped_DTR/FSBL/Inc/stm32n6xx_hal_conf.h    HAL configuration file
  - XSPI/XSPI_NOR_MemoryMapped_DTR/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
  - XSPI/XSPI_NOR_MemoryMapped_DTR/FSBL/Inc/main.h                  Header for main.c module
  - XSPI/XSPI_NOR_MemoryMapped_DTR/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
  - XSPI/XSPI_NOR_MemoryMapped_DTR/FSBL/Src/main.c                  Main program
  - XSPI/XSPI_NOR_MemoryMapped_DTR/FSBL/Src/stm32n6xx_hal_msp.c     HAL MSP module
  - XSPI/XSPI_NOR_MemoryMapped_DTR/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file

### <b>Hardware and Software environment</b>

  - This example has been tested with STMicroelectronics STM32N6570-DK (MB1939)
    board and can be easily tailored to any other supported device
    and development board.

  - On STM32N6570-DK board, the BOOT1 mechanical slide switch must be set to SW1.


  - **EWARM** : To monitor a variable in the live watch window, you must proceed as follow :
    - Start a debugging session.
    - Open the View > Images.
    - Double-click to deselect the second instance of project.out.

  - **MDK-ARM** : To monitor a variable in the live watch window, you must comment out SCB_EnableDCache() in main() function.

### <b>How to use it ?</b>

 - Set the boot mode in development mode (BOOT1 switch position is 1-3, BOOT0 switch position doesn't matter).
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory. Code can be executed in this mode for debugging purposes.

 Next, this program can be run in boot from flash mode. This is done by following the instructions below:
 
 - Resort to CubeProgrammer to add a header to the generated binary Project.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin Project.bin -nk -of 0x80000000 -t fsbl -o Project-trusted.bin -hv 2.3 -dump Project-trusted.bin*
   - The resulting binary is Project-trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Project-trusted.bin) in DK board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.
 


**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
