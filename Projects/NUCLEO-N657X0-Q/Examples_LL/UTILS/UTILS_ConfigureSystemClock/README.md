## <b>UTILS_ConfigureSystemClock Example Description</b>

Use of UTILS LL API to configure the system clock using PLL with HSI as source clock. 

The user application just needs to calculate the PLL parameters using STM32CubeMX and call the UTILS LL API.

The system clock runs at 600 MHz and external memory interface at the highest speed.

System starts with clock used after reset.

Then, a system clock switch is done to PLL with HSI as PLL clock source. 

User can easily set its own PLL parameters in changing global variable used to store them.

A LED1 toggle of 1sec provides this information that system is well configured to requested frequency.  

Anyway, user can connect on oscilloscope to MCO pin (PC.09) to check requested frequency:
 
 - SYSB frequency with frequency value around @302MHz.

### <b>Keywords</b>

Utils, system, Clock, HSI, PLL, flash latency, SYSCLK, frequency Oscilloscope



### <b>Directory contents</b>

    - UTILS/UTILS_ConfigureSystemClock/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
    - UTILS/UTILS_ConfigureSystemClock/FSBL/Inc/main.h                  Header for main.c module
    - UTILS/UTILS_ConfigureSystemClock/FSBL/Inc/stm32_assert.h          Template file to include assert_failed function
    - UTILS/UTILS_ConfigureSystemClock/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
    - UTILS/UTILS_ConfigureSystemClock/FSBL/Src/main.c                  Main program
    - UTILS/UTILS_ConfigureSystemClock/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.
    
  - This example has been tested with NUCLEO-N657X0-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-N657X0-Q Set-up
    - Connect the MCO pin to an oscilloscope to monitor the different waveforms:
      - PC.09:  pin 25 connector CN2

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
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Project-trusted.bin) in NUCLEO-N657X0-Q board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.
 - Run the example




**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
