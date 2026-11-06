## <b>DMA_LinkedList Example Description</b>

This example describes how to use the DMA to perform a list of transfers. The transfer list is organized as linked-list,
each time the current transfer ends the DMA automatically reload the next transfer parameters, and starts it (without CPU intervention).

This project is targeted to run on STM32N657X0HxQ devices on NUCLEO-N657X0-Q board from STMicroelectronics.

At the beginning of the main program, the HAL_Init() function is called to reset
all the peripherals and initialize the systick used as 1ms HAL timebase.

The system clock runs at 600 MHz and external memory interface at the highest speed.

The Linked List is configured using the "DMA_LinkedListConfig" function. This function configures the DMA for a linked-list transfer which contains 3 nodes.
All nodes are filled using HAL function "HAL_DMAEx_LinkedList_BuildNode".

Nodes are connected to the linked-list using the HAL function "HAL_DMAEx_LinkedList_InsertNode_Tail".

After setting all nodes parameters using HAL_DMAEx_LinkedList_BuildNode()/HAL_DMAEx_LinkedList_InsertNode_Tail(),
start the transfer in interrupt mode using HAL_DMAEx_LinkedList_Start_IT() function.

Note that using the DMA, the transfer data length is always expressed in bytes whatever
the source and destination data size (byte, half word or word).

At the end, the main program waits for the end of DMA transmission or error interrupts: the DMA transmission ends
when all transfer nodes are served.

NUCLEO-N657X0-Q board's LED can be used to monitor the transfer status:

 - LED1 toggles when no error detected.

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

DMA, Data Transfer, Memory to Memory, channel, SRAM

### <b>Directory contents</b>

File | Description
 --- | ---
DMA/DMA_LinkedList/FSBL/Inc/stm32n6xx_nucleo_conf.h   | BSP configuration file
DMA/DMA_LinkedList/FSBL/Inc/stm32n6xx_hal_conf.h      | HAL configuration file
DMA/DMA_LinkedList/FSBL/Inc/stm32n6xx_it.h            | Header for stm32n6xx_it.c
DMA/DMA_LinkedList/FSBL/Inc/main.h                    | Header for main.c module
DMA/DMA_LinkedList/FSBL/Src/stm32n6xx_it.c            | Interrupt handlers
DMA/DMA_LinkedList/FSBL/Src/main.c                    | Main program
DMA/DMA_LinkedList/FSBL/Src/stm32n6xx_hal_msp.c       | HAL MSP module
DMA/DMA_LinkedList/FSBL/Src/system_stm32n6xx.c        | stm32n6xx system source file

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
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Project-trusted.bin) in the board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.


**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
