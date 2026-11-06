## <b>RCC_ClockConfig Example Description</b>

Configuration of the system clock (SYSCLK) and modification of the clock settings in Run mode, using the RCC HAL API.

The system clock runs at 600 MHz and external memory interface at the highest speed.

In this example, after startup system bus clock is configured to the max frequency using the IC2 output as clock source,
the User push-button (connected to EXTI_Line13) will be used to change the clock source:

    - from IC2 output to HSE
    - from HSE to HSI
    - from HSI to IC2 output

Each time the User push-button is pressed EXTI_Line13 interrupt is generated and in the ISR
the system bus clock source is checked using __HAL_RCC_GET_SYSCLK_SOURCE() macro:

- If the IC2 output is selected as a source clock, the following steps will be followed to switch
   to HSE oscillator:

     a. Switch the system bus clock source to HSE
     b. Select HSE as system system bus clock source and configure bus clocks dividers

- If the HSE oscillator is selected as a source clock, the following steps will be followed to switch
   to HSI oscillator:

     a. Switch the system bus clock source to HSI
     b. Select HSI as system system bus clock source and configure bus clocks dividers

- If the HSI oscillator is selected as a source clock, the following steps will be followed to switch
   to IC2 output oscillator:

     a. Switch the system bus clock source to IC2 output
     b. Select IC2 output as system system bus clock source and configure bus clocks dividers

LED1 is toggled with a timing defined by the HAL_Delay() API.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

RCC, System, Clock Configuration, HSE, HSI, HSIUSB48, System clock, Oscillator

### <b>Directory contents</b>

  - RCC/RCC_ClockConfig/FSBL/Inc/stm32n6xx_nucleo_conf.h     BSP configuration file
  - RCC/RCC_ClockConfig/FSBL/Inc/stm32n6xx_hal_conf.h        HAL configuration file
  - RCC/RCC_ClockConfig/FSBL/Inc/stm32n6xx_it.h              Interrupt handlers header file
  - RCC/RCC_ClockConfig/FSBL/Inc/main.h                      Header for main.c module
  - RCC/RCC_ClockConfig/FSBL/Src/stm32n6xx_it.c              Interrupt handlers
  - RCC/RCC_ClockConfig/FSBL/Src/main.c                      Main program
  - RCC/RCC_ClockConfig/FSBL/Src/system_stm32n6xx.c          STM32N6xx system source file
  - RCC/RCC_ClockConfig/FSBL/Src/stm32n6xx_hal_msp.c         HAL MSP module

### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with a STM32N657X0HxQ embedded on an
    NUCLEO-N657X0-Q board and can be easily tailored to any other supported
    device and development board.

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
