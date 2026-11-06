## <b>I3C_Target_Direct_Command_DMA Example Description</b>

How to handle a Direct Command procedure between an I3C Controller and an I3C Target,
using Controller in DMA.

      - Board: NUCLEO-N657X0-Q's (embeds a STM32N657X0H3Q device)
      - SCL Pin: PH9 (Arduino SCL/D15 CN14 pin10)
      - SDA Pin: PC1 (Arduino SDA/D14 CN14 pin9)
      Connect GND between each board
      Use short wire as possible between the boards or twist an independent ground wire on each I3C lines
      mean one GND twist around SCL and one GND twist around SDA to help communication at 12.5Mhz.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.

The system clock runs at 600 MHz and external memory interface at the highest speed).

The I3C peripheral configuration is ensured by the HAL_I3C_Init() function.
This later is calling the HAL_I3C_MspInit()function which core is implementing
the configuration of the needed I3C resources according to the used hardware (CLOCK, GPIO and NVIC).
User may update this function to change I3C configuration.

The I3C communication is then initiated.
The project is split in two workspaces:
the Controller Board (I3C_Controller_Direct_Command_DMA) and the Target Board (I3C_Target_Direct_Command_DMA)

- Target Board
  The HAL_I3C_ActivateNotification() function allow the Target application to be inform when the Controller
  assign a Dynamic Address, Get or Set CCC command code.

  The HAL_I3C_Tgt_Config(), the HAL_I3C_SetConfigFifo(), and the HAL_I3C_ActivateNotification() functions
  allow respectively the configuration of the Target descriptor,
  the configuration of the internal hardware FIFOs and the enabling of Asynchronous catching event in Target mode using Interrupt.

  The Controller send data at 12.5Mhz during Push-pull phase.


In a first step the Target wait the assignation of a proper associated Dynamic Address from the Controller.

In a second step, until Common Command Code procedure from the Controller,
the Target wait in a no activity loop.
Then, at reception of the first Common Code procedure from the Controller,
the Target start the sending or receiving of the CCC additional data depending of Common Command Code.

This sending or receiving of additional data is totally manage by internal hardware state machine,
only end of CCC procedure is treated on software part through the callback HAL_I3C_NotifyCallback().

Each time a Set CCC command is confirmed treated through HAL_I3C_NotifyCallback(),
the associated data is compared with the expected value.

Each time a Get or Set CCC command is confirmed treated through HAL_I3C_NotifyCallback(), the LED1 is toggle.

NUCLEO-N657X0-Q's LEDs can be used to monitor the transfer status:
 - LED1 is toggle each time a Command Code process is treated on hardware side.
 - LED2 is toggle slowly when there is an error in Command Code process.  

#### <b>Notes</b>

  1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

  2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Connectivity, Communication, I3C, Interrupt, DMA, Controller, Target, Dynamic Address Assignment, Direct Command,
Transmission, Reception

### <b>Directory contents</b>

  - I3C/I3C_Target_Direct_Command_DMA/FSBL/Inc/stm32n6xx_nucleo_conf.h   BSP configuration file
  - I3C/I3C_Target_Direct_Command_DMA/FSBL/Inc/stm32n6xx_hal_conf.h      HAL configuration file
  - I3C/I3C_Target_Direct_Command_DMA/FSBL/Inc/stm32n6xx_it.h            I3C interrupt handlers header file
  - I3C/I3C_Target_Direct_Command_DMA/FSBL/Inc/main.h                    Header for main.c module
  - I3C/I3C_Target_Direct_Command_DMA/FSBL/Src/stm32n6xx_it.c            I3C interrupt handlers
  - I3C/I3C_Target_Direct_Command_DMA/FSBL/Src/main.c                    Main program
  - I3C/I3C_Target_Direct_Command_DMA/FSBL/Src/system_stm32n6xx.c        STM32N6xx system source file
  - I3C/I3C_Target_Direct_Command_DMA/FSBL/Src/stm32n6xx_hal_msp.c       HAL MSP file

### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0H3Q devices.

  - This example has been tested with NUCLEO-N657X0-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-N657X0-Q Set-up

    - Use short wire as possible between the boards or twist an independent ground wire on each I3C lines
      mean one GND twist around SCL and one GND twist around SDA to help communication at 12.5Mhz.
    - Connect I3C_SCL line of Controller board (PH9 Arduino SCL/D15 CN14 pin10) to I3C_SCL line of Target Board (PH9 Arduino SCL/D15 CN14 pin10).
    - Connect I3C_SDA line of Controller board (PC1 Arduino SDA/D14 CN14 pin9) to I3C_SDA line of Target Board (PC1 Arduino SDA/D14 CN14 pin9).
    - Connect GND of Controller board to GND of Target Board.

  - **EWARM** : To monitor a variable in the live watch window, you must proceed as follow :
    - Start a debugging session.
    - Open the View > Images.
    - Double-click to deselect the second instance of project.out.

  - **MDK-ARM** : To monitor a variable in the live watch window, you must comment out SCB_EnableDCache() in main() function.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:
 
 - Set the boot mode in development mode (BOOT1 switch position is 2-3, BOOT0 switch position doesn't matter).
 - Open your preferred toolchain
 - Rebuild all files and load your image into Controller memory (I3C_Controller_Direct_Command_DMA). Code can be executed in this mode for debugging purposes.
 - Then rebuild all files and load your image into Target memory (I3C_Target_Direct_Command_DMA).
  - Run the Controller in debug mode before run the Target in normal mode.
 This sequence will prevent a false startup phase on Target side
 as there is no high level on the bus, if the Target is started before the Controller.
 
 Next, this program can be run in boot from flash mode. This is done by following the instructions below:
 
 - Resort to CubeProgrammer to add a header to the generated binary Project.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin Project.bin -nk -of 0x80000000 -t fsbl -o Project-trusted.bin -hv 2.3 -dump Project-trusted.bin*
   - The resulting binary is Project-trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Project-trusted.bin) in the board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.


**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
