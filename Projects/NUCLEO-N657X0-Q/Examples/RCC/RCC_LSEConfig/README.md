
## <b>RCC_LSEConfig Example Description</b>

Enabling/disabling of the low-speed external(LSE) RC oscillator (about 32 KHz) at run time, using the RCC HAL API.

   - Board: NUCLEO-N657X0-Q (embeds a STM32N657X0H3Q device)
   - Microcontroller Clock Output MCO2 Pin: PC.09

In this example, after startup SYSCLK is configured to the max frequency using the PLL with
HSIDIV as clock source, the USER push-button (connected to External line 13)
is used to enable/disable LSE:

Each time the USER push-button is pressed, External line 13 interrupt is generated and the ISR
enables the LSE when it is off, disables it when it is on.

- when LSE is off, LED1 is toggling (every 300 ms)
- when LSE is on, LED3 is toggling (every 300 ms)

LSE clock waveform is available on the MCO2 pin PC.09 and can be captured
on an oscilloscope.


In case of configuration error, LED2 is On.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

System, RCC, PLL, PLLCLK, SYSCLK, LSE, Clock, Oscillator,

### <b>Directory contents</b>

  - RCC/RCC_LSEConfig/FSBL/Inc/stm32n6xx_nucleo_conf.h       BSP configuration file
  - RCC/RCC_LSEConfig/FSBL/Inc/stm32n6xx_hal_conf.h          HAL configuration file
  - RCC/RCC_LSEConfig/FSBL/Inc/stm32n6xx_it.h                Interrupt handlers header file
  - RCC/RCC_LSEConfig/FSBL/Inc/main.h                        Header for main.c module
  - RCC/RCC_LSEConfig/FSBL/Src/stm32n6xx_it.c                Interrupt handlers
  - RCC/RCC_LSEConfig/FSBL/Src/main.c                        Main program
  - RCC/RCC_LSEConfig/FSBL/Src/system_stm32n6xx.c            STM32N6xx system source file
  - RCC/RCC_LSEConfig/FSBL/Src/stm32n6xx_hal_msp.c           HAL MSP module

### <b>Hardware and Software environment</b>

  - This example runs on STM32N6XX devices.

  - This example has been tested with NUCLEO-N657X0-Q
    board and can be easily tailored to any other supported device
    and development board.

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
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Project-trusted.bin) in the board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.



**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
