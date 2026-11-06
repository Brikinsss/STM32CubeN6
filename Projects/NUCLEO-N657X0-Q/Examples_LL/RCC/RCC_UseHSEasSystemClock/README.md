
## <b>RCC_UseHSEasSystemClock Example Description</b>

Use of the RCC LL API to start the HSE and use it as system clock.

After startup, the SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 400 Mhz and the CPU clock at 600 MHz, using PLL1 with HSI as source.

Then, HSE is started and we wait for ready state. System clock is set to HSE thanks to IT triggered by HSE ready.

- LED3 (green) is toggling at 1Hz to indicate that HSE is ready
- LED2 (red) is toggling at 1Hz if an error occurred

### <b>Keywords</b>

System, RCC, PLL, PLLCLK, SYSCLK, HSE, Clock, Oscillator, HSI


### <b>Directory contents</b>

File | Description
 --- | ---
  RCC/FSBL/RCC_UseHSEasSystemClock/Inc/main.h                     | Header for main.c module
  RCC/FSBL/RCC_UseHSEasSystemClock/Inc/stm32_assert.h             | Template file to include assert_failed function
  RCC/FSBL/RCC_UseHSEasSystemClock/Inc/stm32n6xx_it.h             | Interrupt handlers header file
  RCC/FSBL/RCC_UseHSEasSystemClock/Src/main.c                     | Main program
  RCC/FSBL/RCC_UseHSEasSystemClock/Src/stm32n6xx_it.c             | Interrupt handlers
  RCC/FSBL/RCC_UseHSEasSystemClock/Src/system_stm32n6xx_fsbl.c    | STM32N6xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32N6xx devices.

  - This example has been tested with NUCLEO-N657X0-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-N657X0-Q Set-up
     - Connect MCO1 pin (pin 1 of U7 component) to an oscilloscope to monitor the system clock.

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
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Project-trusted.bin) in NUCLEO-N657X0-Q board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.
 - Run the example



**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
