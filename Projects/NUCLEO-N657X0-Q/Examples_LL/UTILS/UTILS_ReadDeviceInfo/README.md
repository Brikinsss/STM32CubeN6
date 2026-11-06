## <b>UTILS_ReadDeviceInfo Example Description</b>

This example reads the UID, Device ID and Revision ID and saves
them into a global information buffer.

Then UID, Device ID and Revision ID will be saved in following global variables:

- aShowDeviceID: Device identifier
- aShowRevisionID: Revision identifier (This field indicates the revision of the device.
  For example, it is read as 0x1000 for Revision 1.0)
- aShowCoordinate: X and Y coordinates on the wafer expressed in BCD format
- aShowWaferNumber: WAF_NUM[7:0], Wafer number (8-bit unsigned number)
- aShowLotNumber: LOT_NUM[55:0], Lot number (ASCII encoded)

### <b>Keywords</b>
System, Utils, Device info, UID, Device ID, Revision ID, Wafer, Lot number

### <b>Directory contents</b>

  - UTILS/UTILS_ReadDeviceInfo/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
  - UTILS/UTILS_ReadDeviceInfo/FSBL/Inc/main.h                  Header for main.c module
  - UTILS/UTILS_ReadDeviceInfo/FSBL/Inc/stm32_assert.h          Template file to include assert_failed function  
  - UTILS/UTILS_ReadDeviceInfo/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
  - UTILS/UTILS_ReadDeviceInfo/FSBL/Src/main.c                  Main program
  - UTILS/UTILS_ReadDeviceInfo/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file


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
