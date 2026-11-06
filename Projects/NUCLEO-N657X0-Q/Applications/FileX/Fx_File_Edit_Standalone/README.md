
## <b>Fx_File_Edit_Standalone Application Description</b>

This application provides an example of FileX stack usage on NUCLEO-N657X0-Q board, running in standalone mode (without ThreadX). It demonstrates how to create a Fat File system on the internal SRAM memory using FileX API.

The application is designed to execute file operations on the SRAM-Disk device, it provides all required software code for properly managing it.

The application's main calls the MX_FileX_Init() function in order to Initialize FileX, format the SRAM memory as FAT and open the sram_disk driver. The resulting file system is a FAT32 compatible, with 512 bytes per sector.

Upon successful opening of the created SRAM-Disk media, FileX continues with creating a file called "STM32.TXT" into the root directory by calling MX_FileX_Process(VOID *arg) function, then writes into it some predefined data. The file is re-opened in read only mode and content is checked.

As stated earlier, the present application runs in standalone mode without ThreadX, for this reason, the standalone variant of FileX is used, plus the following flags need to be set in fx_user.h:

-  #define FX_SINGLE_THREAD
-  #define FX_STANDALONE_ENABLE

#### <b>Expected success behavior</b>

Successful operation is marked by a toggling green led light.

Also, information regarding executing operation on the SRAM-Disk is printed to the serial port.

#### <b>Error behaviors</b>

On failure, the red led starts toggling while the green and blue leds are switched off.

#### <b>Assumptions if any</b>

None

#### <b>Known limitations</b>

None

### <b>Notes</b>

None

#### <b>FileX/LevelX usage hints</b>

- When calling the fx_media_format() API, it is highly recommended to understand all the parameters used by the API to correctly generate a valid filesystem.

### <b>Keywords</b>

FileX, File System, FAT32, SRAM

### <b>Hardware and Software environment</b>

  - This application runs on STM32N657X0H3QU devices.
  - This application has been tested with STMicroelectronics NUCLEO-N657X0-Q boards Revision MB1940-N657XOQ-C01 and can be easily tailored to any other supported device and development board.

  - This application uses USART1 to display logs, the hyperterminal configuration is as follows:

    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - Stop Bit = 1
    - Parity = None
    - Flow control = None

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

Next, this program can be run in boot from flash mode. This can be done by following the instructions below:

 - Resort to CubeProgrammer to add a header to the generated binary Fx_File_Edit_Standalone.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin Fx_File_Edit_Standalone.bin -nk -of 0x80000000 -t fsbl -o Fx_File_Edit_Standalone-trusted.bin -hv 2.3 -dump Fx_File_Edit_Standalone-trusted.bin*
   - The resulting binary is Fx_File_Edit_Standalone-trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Fx_File_Edit_Standalone-trusted.bin) in Nucleo board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.


**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
