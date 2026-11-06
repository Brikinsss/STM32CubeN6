## <b>Templates_Board Example Description</b>

This project provides a reference template for the STM32N6570-DK board based on the STM32Cube HAL API that can be used
to build any firmware application. Note that security is enabled by default on Cortex-M55.

- This project was created from STM32CubeMX using the "Start My project from ST board" feature.
- It is targeted to run on STM32N657X0H3QU device on STM32N6570-DK board from STMicroelectronics.
- This template shows how to use the BSP drivers for all the features embedded on the STM32N6570-DK 
board thanks to BSP resources initialization and demonstration codes.
- Once the clock is set, the green and the red LEDs (GPIO PO.01 and PG.10) toggle in an infinite loop with a 100-ms period.


#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.


### <b>Keywords</b>

Reference, Template

### <b>Directory contents</b>

      - Templates_Board/FSBL/Core/Src/main.c                         Main program
      - Templates_Board/FSBL/Core/Src/stm32n6xx_hal_msp.c            MSP file
      - Templates_Board/FSBL/Core/Src/stm32n6xx_it.c                 Interrupt handlers
      - Templates_Board/FSBL/Core/Src/system_stm32n6xx_fsbl.c        System file   
      - Templates_Board/FSBL/Core/Inc/main.h                         Main program header file
      - Templates_Board/FSBL/Core/Inc/stm32n6570_discovery_conf.h    BSP Configuration file
      - Templates_Board/FSBL/Core/Inc/stm32n6xx_hal_conf.h           HAL Configuration file
      - Templates_Board/FSBL/Core/Inc/stm32n6xx_it.h                 Interrupt handlers header file
      - Templates_Board/FSBL/Core/Inc/stm32_assert.h                 Assert header file


### <b>Hardware and Software environment</b>

  - This template runs on STM32N657X0H3QU devices.
  - This template has been tested with STMicroelectronics STM32N6570-DK (MB1939)
    board and can be easily tailored to any other supported device
    and development board.

  - **EWARM** : To monitor a variable in the live watch window, you must proceed as follow :
    - Start a debugging session.
    - Open the View > Images.
    - Double-click to deselect the second instance of project.out.

  - **MDK-ARM** : To monitor a variable in the live watch window, you must comment out SCB_EnableDCache() in main() function.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

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
