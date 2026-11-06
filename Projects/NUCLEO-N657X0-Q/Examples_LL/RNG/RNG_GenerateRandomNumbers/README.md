## <b>RNG_GenerateRandomNumbers Example Description</b>

Configuration of the RNG to generate 32-bit long random numbers. The peripheral initialization uses LL unitary service
functions for optimization purposes (performance and size).

Example execution:
After startup from reset and system configuration, RNG configuration is performed.

User is then asked to press USER push-button (LED1 blinking fast).

On USER push-button press, several (8) Random 32bit numbers are generated

(DRDY flag is polled until 1, indicating a random number has been generated and could be retrieved from DR register).

Corresponding generated values are available and stored in a u32 array (aRandom32bit),
whose content could be displayed using debugger (Watch or LiveWatch features).

After successful Random numbers generation, LED1 is turned On.

In case of errors, LED1 is slowly blinking (1sec period).

### <b>Keywords</b>

Analog, RNG, Random, FIPS PUB 140-2, Analog Random number generator, Entropy, Period


### <b>Directory contents</b>

  - RNG/RNG_GenerateRandomNumbers/FSBL/Inc/stm32n6xx_it.h                Interrupt handlers header file
  - RNG/RNG_GenerateRandomNumbers/FSBL/Inc/main.h                        Header for main.c module
  - RNG/RNG_GenerateRandomNumbers/FSBL/Inc/stm32_assert.h                Template file to include assert_failed function
  - RNG/RNG_GenerateRandomNumbers/FSBL/Src/stm32n6xx_it.c                Interrupt handlers
  - RNG/RNG_GenerateRandomNumbers/FSBL/Src/main.c                        Main program
  - RNG/RNG_GenerateRandomNumbers/FSBL/Src/system_stm32n6xx.c            STM32N6xx system source file


### <b>Hardware and Software environment</b> 

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with NUCLEO-N657X0-Q board and can be
    easily tailored to any other supported device and development board.

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
