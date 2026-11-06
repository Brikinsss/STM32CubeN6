## <b>UART_TwoBoards_ComPolling Example Description</b>

UART transmission (transmit/receive) in Polling mode 
between two boards.

    Board: NUCLEO-N657X0-Q (embeds a STM32N657X0HxQ device)
    Tx Pin: PE.05 (Pin 4 in CN15)
    Rx Pin: PE.06 (Pin 2 in CN15)

Two identical boards are connected :

 - Board 1: transmitting then receiving board
 - Board 2: receiving then transmitting board

The user presses the USER push-button on board 1.

Then, board 1 sends in polling mode a message to board 2 that sends it back to
board 1 in polling mode as well.

Finally, board 1 and 2 compare the received message to that sent.
If the messages are the same, the test passes.

WARNING: as both boards do not behave the same way, "TRANSMITTER_BOARD" compilation
switch is defined in /Src/main.c and must be enabled
at compilation time before loading the executable in the board that first transmits
then receives.

The receiving then transmitting board needs to be loaded with an executable
software obtained with TRANSMITTER_BOARD disabled. 

NUCLEO-N657X0-Q board LEDs are used to monitor the transfer status:
- While board 1 is waiting for the user to press the USER push-button, its LED1 is
  blinking rapidly (100 ms period).
- When the test passes, LED1 is turned on.
- If there is an initialization or transfer error, LED2 is turned on.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.

The system clock runs at 600 MHz and external memory interface at the highest speed.

The UART is configured as follows:

    - BaudRate = 9600 baud
    - Word Length = 8 bits (8 data bits, no parity bit)
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

**Note:** When the parity is enabled, the computed parity is inserted at the MSB
position of the transmitted data.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Connectivity, UART/USART, baud rate, RS-232, full-duplex, Polling, parity, stop bit,
Transmitter, Receiver, Asynchronous

### <b>Directory contents</b>

  - UART/UART_TwoBoards_ComPolling/Inc/stm32n6xx_nucleo_conf.h     BSP configuration file
  - UART/UART_TwoBoards_ComPolling/Inc/stm32n6xx_hal_conf.h    HAL configuration file
  - UART/UART_TwoBoards_ComPolling/Inc/stm32n6xx_it.h          interrupt handlers header file
  - UART/UART_TwoBoards_ComPolling/Inc/main.h                  Header for main.c module  
  - UART/UART_TwoBoards_ComPolling/Src/stm32n6xx_it.c          interrupt handlers
  - UART/UART_TwoBoards_ComPolling/Src/main.c                  Main program
  - UART/UART_TwoBoards_ComPolling/Src/stm32n6xx_hal_msp.c     HAL MSP module
  - UART/UART_TwoBoards_ComPolling/Src/system_stm32n6xx.c      STM32N6xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.    
  - This example has been tested with two NUCLEO-N657X0-Q boards embedding
    a STM32N657X0HxQ device and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-N657X0-Q set-up
    - Connect a wire between 1st board PE5 pin (Uart Tx) and 2nd board PE6 pin (Uart Rx)
    - Connect a wire between 1st board PE6 pin (Uart Rx) and 2nd board PE5 pin (Uart Tx)
    - Connect 1st board GND to 2nd Board GND

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
