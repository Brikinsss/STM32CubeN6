## <b>DMA_Trigger Example Description</b>

How to configure and use the DMA HAL API to perform DMA triggered transactions.

This project is targeted to run on STM32N657X0HxQ devices on NUCLEO-N657X0-Q board from STMicroelectronics.

This project runs from the internal RAM. It is launched by the bootROM after being copied from internal RAM to internal RAM.
Configuration (caches, MPU regions [region 0 and 1], system clock at 600 MHz and external memory interface at the highest speed).

The triggering DMA transfer is started by software.
When this transfer ends, its transfer complete signal will be used to trigger another DMA transfer in a different DMA channel.

The triggering DMA channel performs transactions from source buffer "aSRC_Buffer" to destination buffer
"aDST_TriggerChannel_Buffer" with block size equal to 4 bytes and generates a transfer complete signal when transaction ends.
The triggered channel performs transaction from source buffer "aDST_TriggerChannel_Buffer" to
destination buffer "aDST_TriggeredChannel_Buffer" with block size equal to 128 bytes.
At each trigger signal generation, the triggered channel performs a single data transaction (4 bytes).

NUCLEO-N657X0-Q board's LED can be used to monitor the transfers status:

 - LED1 toggle when no error detected.
 - LED2 is ON when any error occurred.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Memory, DMA, Trigger

### <b>Directory contents</b>

  - DMA/DMA_Trigger/FSBL/Src/main.c                  Main program
  - DMA/DMA_Trigger/FSBL/Src/system_stm32n6xx.c      STM32N6xx system clock configuration file
  - DMA/DMA_Trigger/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
  - DMA/DMA_Trigger/FSBL/Inc/main.h                  Main program header file
  - DMA/DMA_Trigger/FSBL/Inc/stm32n6xx_nucleo_conf.h BSP Configuration file
  - DMA/DMA_Trigger/FSBL/Inc/stm32n6xx_hal_conf.h    HAL Configuration file
  - DMA/DMA_Trigger/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with STMicroelectronics NUCLEO-N657X0-Q
    board and can be easily tailored to any other supported device
    and development board.

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
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Project-trusted.bin) in DK board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.
 
 

**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
