## <b>JPEG_DecodingFromOSPI_DMA Example Description</b>

This project demonstrates how to decode a JPEG image using using the JPEG HW decoder in DMA mode.
After bootROM execution, the FSBL project executes in internal RAM, ensures proper MPU, caches and clock setting then configures the external Flash in execution mode. When done, the program counter is set to the application entry point and the application in turn executes.

- This project is targeted to run on STM32N657X0H3QU device on STM32N6570-DK board from STMicroelectronics.  
- The FSBL project configures the maximum system clock frequency at 600 MHz.
- Once the application runs, the green LED (GPIO PO.01) turns ON.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

FSBL, XIP, JPEG, Graphics, Basics, DMA

### <b>Sub-project FSBL directory contents</b>

      - JPEG/JPEG_DecodingFromOSPI_DMA/FSBL/Inc/main.h                       Header for main.c module
      - JPEG/JPEG_DecodingFromOSPI_DMA/FSBL/Inc/extmem.h                     Header for extmem.c module
      - JPEG/JPEG_DecodingFromOSPI_DMA/FSBL/Inc/partition_stm32n657xx.h      Partition file
      - JPEG/JPEG_DecodingFromOSPI_DMA/FSBL/Inc/stm32n6xx_hal_conf.h         HAL Configuration file
      - JPEG/JPEG_DecodingFromOSPI_DMA/FSBL/Inc/stm32n6xx_it.h               Interrupt handlers header file
      - JPEG/JPEG_DecodingFromOSPI_DMA/FSBL/Inc/stm32_extmem_conf.h          External memory manager Configuration file
      - JPEG/JPEG_DecodingFromOSPI_DMA/FSBL/Src/main.c                       Main program
      - JPEG/JPEG_DecodingFromOSPI_DMA/FSBL/Src/extmem.c                     Code to initialize external memory
      - JPEG/JPEG_DecodingFromOSPI_DMA/FSBL/Src/stm32n6xx_hal_msp.c          HAL MSP module
      - JPEG/JPEG_DecodingFromOSPI_DMA/FSBL/Src/stm32n6xx_it.c               Interrupt handlers
      - JPEG/JPEG_DecodingFromOSPI_DMA/FSBL/Src/system_stm32n6xx_fsbl.c      STM32N6xx system source file

### <b>Sub-project Appli directory contents</b>


      - JPEG/JPEG_DecodingFromOSPI_DMA/Appli/Inc/main.h                      Header for main.c module  
      - JPEG/JPEG_EncodingFromOSPI_DMA/Appli/Inc/stm32n6xx_hal_conf.h        HAL Configuration file
      - JPEG/JPEG_EncodingFromOSPI_DMA/Appli/Inc/stm32n6xx_it.h              Interrupt handlers header file
      - JPEG/JPEG_EncodingFromOSPI_DMA/Appli/Inc/partition_stm32n657xx.h     Partition file
      - JPEG/JPEG_EncodingFromOSPI_DMA/Appli/Inc/stm32n6570_discovery_conf.h BSP Configuration file 
      - JPEG/JPEG_DecodingFromOSPI_DMA/Appli/Inc/image_320_240_jpg.h         JPEG Image 320x240 to be decoded
      - JPEG/JPEG_DecodingFromOSPI_DMA/Appli/Inc/decode_dma.h                Header for encode_dma.c module
      - JPEG/JPEG_EncodingFromOSPI_DMA/Appli/Src/main.c                      Main program
      - JPEG/JPEG_EncodingFromOSPI_DMA/Appli/Src/stm32n6xx_hal_msp.c         HAL MSP module
      - JPEG/JPEG_EncodingFromOSPI_DMA/Appli/Src/stm32n6xx_it.c              Interrupt handlers
      - JPEG/JPEG_EncodingFromOSPI_DMA/Appli/Src/system_stm32n6xx.c          STM32N6xx system source file
      - JPEG/JPEG_DecodingFromOSPI_DMA/Appli/Src/decode_dma.c                JPEG Decoding (from memory) with DMA module      

### <b>Hardware and Software environment</b>

  - This example runs on STM32N6 devices.

  - This example has been tested with STMicroelectronics STM32N6570-DK (MB1939)
    board and can be easily tailored to any other supported device
    and development board.

  - The following OTP fuses are set in this example:

    - VDDIO3_HSLV=1     I/O XSPIM_P2 High speed option enabled

**WARNING**

When OTP fuses are set, they can not be reset.

If the OTP fuseVDDIO3_HSLV is not already set, run the example once with boot mode set in development mode and NO_OTP_FUSE compilation switch disabled
(otherwise, it isn't possible to benefit from the maximum transfer rate to access the external flash).

For the following runs, select NO_OTP_FUSE option by default using the preprocessor definition in the IDE.


  - **EWARM** : To monitor a variable in the live watch window, you must proceed as follow :
    - Start a debugging session.
    - Open the View > Images.
    - Double-click to deselect the second instance of project.out.

  - **MDK-ARM** : To monitor a variable in the live watch window, you must comment out SCB_EnableDCache() in main() function.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Set the boot mode in development mode (BOOT1 switch position is 1-3, BOOT0 switch position doesn't matter).
 - Open your preferred toolchain
 - Select first the FSBL workspace
 - Rebuild all files from sub-project FSBL (if no modification is done on FSBL project, this step can be done only once)
 - Select the Appli workspace
 - Rebuild all files from sub-project Appli
 - Resort to CubeProgrammer to add a header to the generated Appli binary Project.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin Project.bin -nk -of 0x80000000 -t fsbl -o Project-trusted.bin -hv 2.3 -dump Project-trusted.bin*
   - The resulting binary is Project-trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the Appli binary and its header (Project-trusted.bin) in DK board external Flash at address 0x7010'0000.
 - Load the FSBL binary in internal RAM using the IDE
 - Run the example

 Next, this program can be run in boot from flash mode. This is done by following the instructions below:

 - Resort to CubeProgrammer to add a header to the generated binary FSBL.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin FSBL.bin -nk -of 0x80000000 -t fsbl -o FSBL-trusted.bin -hv 2.3 -dump FSBL-trusted.bin*
   - The resulting binary is FSBL-trusted.bin. 
 - In resorting again to CubeProgrammer, load the FSBL binary and its header (FSBL-trusted.bin) in DK board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.






**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
