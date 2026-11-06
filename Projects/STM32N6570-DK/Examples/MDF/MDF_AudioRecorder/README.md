## <b>MDF_AudioRecorder Example Description</b>

This project demonstrates how to configure MDF to perform PCM record in DMA circular mode.

Digital microphone MP23DB01HPTR mounted on STM32N6570-DK board is used on this example.

On this example, MDF1 kernel clock is set to around 49.152MHz using PLL3.
MDF1 processing clock will be set to 24,576MHz and output clock frequency on MDF1_CCK0 will de set to 2.048MHz.
With a filter decimation of 32 and a reshape filter decimation ratio of 4,
we will obtain PCM samples at around 16KHz (2.048MHz/(32*4)).

PCM samples will be stored in loop (circular DMA) on PcmBuffer.

Recorded samples are played on headset using SAI and WM8904 audio codec.

If error occurs during previous steps, red led toggles.

- This project is targeted to run on STM32N657X0H3QU device on STM32N6570-DK board from STMicroelectronics.
- This project configures the maximum system clock frequency at 600 MHz.


#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.


### <b>Keywords</b>

Audio, MDF, DMA, PCM

### <b>Directory contents</b>

      - MDF/MDF_AudioRecorder/FSBL/Src/main.c                         Main program
      - MDF/MDF_AudioRecorder/FSBL/Src/stm32n6xx_hal_msp.c            HAL MSP module
      - MDF/MDF_AudioRecorder/FSBL/Src/stm32n6xx_it.c                 Interrupt handlers
      - MDF/MDF_AudioRecorder/FSBL/Src/system_stm32n6xx_fsbl.c        STM32N6xx system source file
      - MDF/MDF_AudioRecorder/FSBL/Inc/main.h                         Main program header file
      - MDF/MDF_AudioRecorder/FSBL/Inc/stm32n6xx_hal_conf.h           HAL configuration file
      - MDF/MDF_AudioRecorder/FSBL/Inc/stm32n6xx_it.h                 Interrupt handlers header file
      - MDF/MDF_AudioRecorder/FSBL/Inc/stm32n6570_discovery_conf.h    BSP configuration file


### <b>Hardware and Software environment</b>

  - This template runs on STM32N657X0H3QU devices.
  - This template has been tested with STMicroelectronics STM32N6570-DK (MB1939)
    board and can be easily tailored to any other supported device
    and development board.

  - **EWARM** : To monitor a variable in the live watch window, you must proceed as follow :
    - Start a debugging session.
    - Open the View > Images.
    - Double-click to deselect the second instance of project.out.

  - **MDK-ARM** : To monitor a variable in the live watch window, you must comment out SCB_EnableDCache() in main() function.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Plug headset on board.
 - Set the boot mode in development mode (BOOT1 switch position is 1-3, BOOT0 switch position doesn't matter).
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory. Code can be executed in this mode for debugging purposes.

 Next, this program can be run in boot from flash mode. This is done by following the instructions below:

 - Resort to CubeProgrammer to add a header to the generated binary Project.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin Project.bin -nk -of 0x80000000 -t fsbl -o Project-trusted.bin -hv 2.3 -dump Project-trusted.bin*
   - The resulting binary is Project-trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Project-trusted.bin) in DK board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.



**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
