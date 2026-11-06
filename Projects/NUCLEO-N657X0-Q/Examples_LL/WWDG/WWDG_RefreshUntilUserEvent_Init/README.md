## <b>WWDG_RefreshUntilUserEvent_Init Example Description</b>

Configuration of the WWDG to periodically update the counter and
generate an MCU WWDG reset when a user button is pressed. The peripheral initialization
uses the LL unitary service functions for optimization purposes (performance and size).

Example Configuration:

Configure the WWDG (Window, Prescaler & Counter) and enable it.
Refresh the WWDG downcounter in the main loop - LED1 is blinking fastly & continuously

Example Execution:

When USER push-button is pressed, the Downcounter automatic refresh mechanism is disable and thus, reset will occur.
After a reset when re-entering in the main, RCC WWDG Reset Flag will be checked and if we are back from a WWDG reset the LED1 will be switch ON.

Waiting a new USER push-button pressed to re-activate the WWDG

### <b>Keywords</b>

System, WWDG, Timeout, Refresh, Counter update, MCU Reset, Downcounter, Event, Window

### <b>Directory contents</b>

    - WWDG/WWDG_RefreshUntilUserEvent_Init/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
    - WWDG/WWDG_RefreshUntilUserEvent_Init/FSBL/Inc/main.h                  Header for main.c module
    - WWDG/WWDG_RefreshUntilUserEvent_Init/FSBL/Inc/stm32_assert.h          Template file to include assert_failed function
    - WWDG/WWDG_RefreshUntilUserEvent_Init/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
    - WWDG/WWDG_RefreshUntilUserEvent_Init/FSBL/Src/main.c                  Main program
    - WWDG/WWDG_RefreshUntilUserEvent_Init/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file

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
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Project-trusted.bin) in NUCLEO-N657X0-Q board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.
 - Run the example



**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
