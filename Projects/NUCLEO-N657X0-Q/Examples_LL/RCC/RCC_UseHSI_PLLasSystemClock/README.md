## <b>RCC_UseHSI_PLLasSystemClock Example Description</b>

Modification of the PLL parameters in run time.

In this example, the toggling frequency of the green LED1 depends on the system clock
frequency and, each time the USER push-button is pressed, the PLL switches between two configurations.
This make the LED1 blinking speed to highlight the system clock frequency changes.

In this example, after start-up, CPU clock is configured to the max frequency using the PLL with
HSI as clock source.

Then, when pressing USER push-button, an automatic switch is done between PLL with HSI as clock source (SYSBUS
set to 600MHz) and PLL with HSI as clock source (SYSBUS set to 453.33MHz).

LED1 will toggle differently between the 2 configurations (quick toggle with SYSBUS configuration at 600MHz).

In case of issues, LED1 will toggle every 1 second.

**Note:** "uwFrequency" variable can be added in LiveWatch to monitor the CPU clock frequency.

### <b>Keywords</b>

System, RCC, PLL, PLLCLK, SYSBUS, HSE, Clock, Oscillator, HSI

### <b>Directory contents</b>

    - RCC/RCC_UseHSI_PLLasSystemClock/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
    - RCC/RCC_UseHSI_PLLasSystemClock/FSBL/Inc/main.h                  Header for main.c module
    - RCC/RCC_UseHSI_PLLasSystemClock/FSBL/Inc/stm32_assert.h          Template file to include assert_failed function
    - RCC/RCC_UseHSI_PLLasSystemClock/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
    - RCC/RCC_UseHSI_PLLasSystemClock/FSBL/Src/main.c                  Main program
    - RCC/RCC_UseHSI_PLLasSystemClock/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file


### <b>Hardware and Software environment</b> 

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with NUCLEO-N657X0-Q board and can be
    easily tailored to any other supported device and development board.

  - **EWARM** : To monitor a variable in the live watch window, you must proceed as follow :
    - Start a debugging session.
    - Open the View > Images.
    - Double-click to deselect the second instance of project.out.

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
