## <b>LPUART_WakeUpFromStop Example Description</b>

Configuration of an LPUART to wake up the MCU from Stop mode
when a given stimulus is received.

    Board:  NUCLEO-N657X0-Q (embeds a STM32N657X0H3Q device)
    Tx Pin: PE5 pin 2 of CN15 connector
    Rx Pin: PE6 pin 4 of CN15 connector
     _________________________                       _________________________
    |           ______________|                     |______________           |
    |          |LPUART        |                     |        LPUART|          |
    |          |              |                     |              |          |
    |          |           TX |_____________________| RX           |          |
    |          |              |                     |              |          |
    |          |              |                     |              |          |
    |          |              |                     |              |          |
    |          |           RX |_____________________| TX           |          |
    |          |              |                     |              |          |
    |          |______________|                     |______________|          |
    |                         |                     |                         |
    |                      GND|_____________________|GND                      |
    |                         |                     |                         |
    | STM32 Board 1           |                     | STM32 Board 2           |
    | (follows a sequence of  |                     | (sends stimuli to wake  |
    | RUN/STOP modes)         |                     |  up STM32 Board 1)      |
    |_________________________|                     |_________________________|


Board 1 enters STOP mode and is awoken by Board 2 which sends
the proper data to wake up the board 1 MCU.

When the proper wake-up event is recognized, the WUF interrupt is triggered
which wakes up board 1.

To confirm its wake up, board 1 sends a confirmation message to board 2 which
checks it is the expected message.

This cycle is repeated 4 times to verify 4 different events

1. wake-up by RXNE flag rising
2. wake-up by Start Bit detection
3. wake-up by 7-bit long address match
4. wake-up by 4-bit long address match

WARNING: as both boards do not behave the same way, "BOARD_IN_STOP_MODE"
compilation switch is defined in UART/LPUART_WakeUpFromStop/Src/main.c and must
be enabled at compilation time before loading the executable in board 1 (that
which is set in STOP mode).

The stimuli-transmitting board (board 2) needs to be loaded with an executable
software obtained with BOARD_IN_STOP_MODE disabled.

At the very beginning, board 1 LED1 is ON then is turned off.
This means that board 1 has entered STOP mode.

Parallelly, board 2 LED1 is quickly blinking (100 ms period). 

The user has just to wait for board 1 LED1 to be turned off then to press board 2
BUTTON_USER to send the first wake-up stimulus to board 1.

This starts the test sequence and no more action is required from the user
(a single BUTTON_USER press is needed to start the full 4 wake-up events
test sequence).

Board 1 LED1 behavior as described below allows to follow the
test progress:

ON ____          _______          _______          _______          ____
       |        |       |        |       |        |       |        |
OFF    |________|       |________|       |________|       |________|
      Enter    Exit    Enter    Exit    Enter    Exit    Enter    Exit
      Stop     Stop    Stop     Stop    Stop     Stop    Stop     Stop
      Mode     Mode    Mode     Mode    Mode     Mode    Mode     Mode
            (RXNE flag)      (Start Bit      (7-bit add.       (4-bit add.
                              Detection)        match)            match)


Simultaneously, board 2 LED1 behavior shown below allows to follow the
test progress on the other side:

      BLINKING
    ON ****________          _______          _______          _______          ____
                   |        |       |        |       |        |       |        |
    OFF            |________|       |________|       |________|       |________|
         USER     WkUp   2nd stim  WkUp   3rd stim  WkUp   4th stim  WkUp    LED ON
         button   conf     sent    conf     sent    conf     sent    conf    to mark
        pushed,   rcv'ed          rcv'ed           rcv'ed           rcv'ed   passed
      1st stimulus                                                            test
         sent
      (RXNE flag)      (Start Bit       (7-bit add.      (4-bit add.
                        Detection)         match)           match)


- If the test fails or if there is an initialization or transfer error, LED1
transmits a sequence of three dots, three dashes, three dots.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.

Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at the maximum frequency with HSI as source of main PLL.

Additionally the HSI is enabled to demonstrate LPUART wake-up capabilities.

Each time board 1 is awoken, the system clock (SYSCLK) is restored.


The LPUART is configured as follows:

    - BaudRate = 9600 baud
    - Word Length = 8 bits (8 data bits, no parity bit)
    - One Stop Bit
    - Parity none
    - Hardware flow control disabled (RTS and CTS signals)


### <b>Notes</b>

 1. This example can not be used in DEBUG mode due to the fact that the
    Cortex-M55 core is no longer clocked during low power mode and so
    debugging features are not available.

 2. Care must be taken when using HAL_Delay(), this function provides accurate
    delay (in milliseconds) based on variable incremented in SysTick ISR.
    This implies that if HAL_Delay() is called from a peripheral ISR process,
    then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be
    blocked. To change the SysTick interrupt priority you have to use
    HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set
    to 1 millisecond to have correct HAL operation.

### <b>Keywords</b>

Connectivity, LPUART, baud rate, RS-232, HyperTerminal, full-duplex,
Transmitter, Receiver, Asynchronous, Low Power, Wake Up

### <b>Directory contents</b>

  - UART/LPUART_WakeUpFromStop/FSBL/Inc/stm32n6xx_nucleo_conf.h     BSP configuration file
  - UART/LPUART_WakeUpFromStop/FSBL/Inc/stm32n6xx_hal_conf.h        HAL configuration file
  - UART/LPUART_WakeUpFromStop/FSBL/Inc/stm32n6xx_it.h              Interrupt handlers header file
  - UART/LPUART_WakeUpFromStop/FSBL/Inc/main.h                      Header for main.c module
  - UART/LPUART_WakeUpFromStop/FSBL/Src/stm32n6xx_it.c              Interrupt handlers
  - UART/LPUART_WakeUpFromStop/FSBL/Src/main.c                      Main program
  - UART/LPUART_WakeUpFromStop/FSBL/Src/stm32n6xx_hal_msp.c         HAL MSP module
  - UART/LPUART_WakeUpFromStop/FSBL/Src/system_stm32n6xx.c          STM32N6xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0H3Q devices.

  - This example has been tested with a couple of NUCLEO-N657X0-Q boards embedding a
    STM32N657X0H3Q device and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-N657X0-Q set-up

    - Connect a wire between 1st board PE5 pin 2 of CN15 connector (LPUART1 Tx)
      and 2nd board PE6 pin 4 of CN15 connector (LPUART1 Rx)
    - Connect a wire between 1st board PE6 pin 4 of CN15 connector (LPUART1 Rx)
      and 2nd board PE5 pin 2 of CN15 connector(LPUART1 Tx)

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
