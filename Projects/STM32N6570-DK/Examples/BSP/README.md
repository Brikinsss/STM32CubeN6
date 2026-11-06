## <b>BSP Example Description</b>

This example provides a description of how to use the different BSP drivers.

This project is targeted to run on STM32N6xx device on Discovery STM32N6570-DK board from STMicroelectronics.

This project runs from internal RAM. The system clock is set at 600 MHz.

This project calls also SCB_EnableICache() and SCB_EnableDCache() functions in order to enable
the Layer 1 Core Instruction and Data Caches.

 In this example, the code is executed from SRAM memory.

- At the beginning of the main program the HAL_Init() function is called to set the SysTick.

The SystemClock_Config() function is used to configure the system clock for STM32N6xx Devices :
The CPU at 600MHz

Add clock settings for peripherals here.

This example shows how to use the different functionalities of components
available on the board by switching between all tests using the USER1 push-button.

- Push the User button to start first Test.

#### <b>LCD</b>
- This example shows how to use the different LCD features to display string with different fonts, 
  to display different shapes, to draw a bitmap and to test orientations.

- Add demos here

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.


### <b>Keywords</b>

BSP, Board support package, Button, LCD, LTDC, Display

### <b>Directory contents</b>

File | Description
 --- | ---
  BSP/Inc/main.h                       | Header for main.c module
  BSP/Inc/stm32n6xx_hal_conf.h         | HAL Configuration file
  BSP/Inc/stm32n6xx_it.h               | Interrupt handlers header file
  BSP/Inc/stm32n6570_discovery_conf.h  | BSP Configuration file
  BSP/Src/main.c                       | Main program
  BSP/Src/stm32n6xx_it.c               | Interrupt handlers
  BSP/Src/system_stm32n6xx.c           | STM32N6xx system source file
  BSP/Src/lcd.c                        | Test Lcd features

### <b>Hardware and Software environment</b>

  - This example runs on STM32N6xx devices.

  - This example has been tested with STMicroelectronics STM32N6570-DK
    board.

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

 Next, this program can be run in boot from flash mode. This is done by following the instructions below:

 - Resort to CubeProgrammer to add a header to the generated binary Project.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin Project.bin -nk -of 0x80000000 -t fsbl -o Project-trusted.bin -hv 2.3 -dump Project-trusted.bin*
   - The resulting binary is Project-trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Project-trusted.bin) in DK board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.



**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
