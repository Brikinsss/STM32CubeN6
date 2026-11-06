## <b>LPTIM_PulseCounter Example Description</b>

How to configure and use, through the LPTIM HAL API, the LPTIM peripheral 
to count pulses.

To reduce power consumption, MCU enters stop mode after starting counting.
Each time the counter reaches the maximum value (Period/Autoreload), an interruption
is generated, the MCU is woke up from stop mode and LED3 toggles the last state.
  
In this example Period value is set to 1000, so each time the counter counts
(1000 + 1) rising edges on LPTIM Input pin PE4, an interrupt is generated and LED3
toggles.

In this example the internal clock provided to the LPTIM1 is LSI (32 Khz),
so the external input is sampled with LSI clock. 

In order not to miss any event, the frequency of the changes on the external Input1 signal should never exceed the
frequency of the internal clock provided to the LPTIM1 (LSI for the present example).

#### <b>Notes</b>

 1. This example can not be used in DEBUG mode, this is due to the fact
    that the Cortex-M55 core is no longer clocked during low power mode
    so debugging features are disabled.

 2. Care must be taken when using HAL_Delay(), this function provides accurate
    delay (in milliseconds) based on variable incremented in SysTick ISR. This
    implies that if HAL_Delay() is called from a peripheral ISR process, then 
    the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
 3. This example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.


### <b>Keywords</b>

Timer, Low Power, Pulse Counter, Stop mode, Interrupt

### <b>Directory contents</b>

  - LPTIM/LPTIM_PulseCounter/FSBL/Inc/stm32n6xx_nucleo_conf.h  BSP configuration file
  - LPTIM/LPTIM_PulseCounter/FSBL/Inc/stm32n6xx_hal_conf.h     HAL configuration file
  - LPTIM/LPTIM_PulseCounter/FSBL/Inc/stm32n6xx_it.h           Interrupt handlers header file
  - LPTIM/LPTIM_PulseCounter/FSBL/Inc/main.h                   Header for main.c module  
  - LPTIM/LPTIM_PulseCounter/FSBL/Src/stm32n6xx_it.c           Interrupt handlers
  - LPTIM/LPTIM_PulseCounter/FSBL/Src/main.c                   Main program
  - LPTIM/LPTIM_PulseCounter/FSBL/Src/stm32n6xx_hal_msp.c      HAL MSP module
  - LPTIM/LPTIM_PulseCounter/FSBL/Src/system_stm32n6xx.c       STM32N6xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-N657X0-Q
    board and can be easily tailored to any other supported device
    and development board.

  - Generate pulses on PE4 (pin 21 in CN3 connector): connect a square waveform.

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
