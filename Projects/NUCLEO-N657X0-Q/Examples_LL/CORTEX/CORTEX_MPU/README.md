## <b>CORTEX_MPU example Description</b>

Presentation of the MPU features. This example configures MPU attributes of different
MPU regions then configures a memory area as privileged read-only, and attempts to
perform read and write operations in different modes.

If the access is permitted LED3 is toggling. If the access is not permitted,
a memory management fault is generated and LED3 is ON.

To generate an MPU memory fault exception due to an access right error, press
user button.

### <b>Notes</b>

  - Using STM32CubeIDE debugger, when hard fault is generated user shall press the debugger "Run" button to continue the execution

### <b>Keywords</b>

MPU, interrupt, MPU memory fault exception,

### <b>Directory contents</b>

    - CORTEX/CORTEX_MPU/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
    - CORTEX/CORTEX_MPU/FSBL/Inc/main.h                  Header for main.c module
    - CORTEX/CORTEX_MPU/FSBL/Inc/stm32_assert.h          Template file to include assert_failed function
    - CORTEX/CORTEX_MPU/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
    - CORTEX/CORTEX_MPU/FSBL/Src/main.c                  Main program
    - CORTEX/CORTEX_MPU/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with a STM32N657X0HxQ embedded on an
    NUCLEO-N657X0-Q board and can be easily tailored to any other supported
    device and development board.

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
