## <b>SPI_TwoBoards_FullDuplex_DMA_Slave_Init Example Description</b>

Data buffer transmission and receptionvia SPI using DMA mode. This example is
based on the STM32N6xx SPI LL API. The peripheral initialization uses
LL unitary service functions for optimization purposes (performance and size).

The communication is done with 2 boards through SPI.

This example shows how to configure GPIO and SPI peripherals
to use a Full-Duplex communication using DMA Transfer mode through the STM32N6xx SPI LL API.

This example is split in two projects, Master board and Slave board:

- Master Board
  SPI5 Peripheral is configured in Master mode.
  DMA1_Channel1 and DMA1_Channel2 configured to transfer Data via SPI peripheral
  GPIO associated to USER push-button is linked with EXTI.

- Slave Board
  SPI5 Peripheral is configured in Slave mode.
  DMA1_Channel1 and DMA1_Channel2 configured to transfer Data via SPI peripheral


Example execution:
On BOARD MASTER, LED1 is blinking Fast (200ms) and wait USER push-button action.
Press USER push-button on BOARD MASTER start a Full-Duplex communication through DMA.
On MASTER side, Clock will be generated on SCK line, Transmission(MOSI Line) and reception (MISO Line)
will be done at the same time.
SLAVE SPI will received  the Clock (SCK Line), so Transmission(MISO Line) and reception (MOSI Line) will be done also.

LED1 is On on both boards if data is well received.

In case of errors, LED1 is blinking Slowly (1s).

#### <b>Notes</b>

 1.You need to perform a reset on Master board, then perform it on Slave board
      to have the correct behaviour of this example.

### <b>Keywords</b>

Connectivity, SPI, Full-duplex, Interrupt, Transmission, Reception, Master, Slave, MISO, MOSI, DMA

### <b>Directory contents</b>

    - SPI/SPI_TwoBoards_FullDuplex_DMA_Slave_Init/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
    - SPI/SPI_TwoBoards_FullDuplex_DMA_Slave_Init/FSBL/Inc/main.h                  Header for main.c module
    - SPI/SPI_TwoBoards_FullDuplex_DMA_Slave_Init/FSBL/Inc/stm32_assert.h          Template file to include assert_failed function
    - SPI/SPI_TwoBoards_FullDuplex_DMA_Slave_Init/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
    - SPI/SPI_TwoBoards_FullDuplex_DMA_Slave_Init/FSBL/Src/main.c                  Main program
    - SPI/SPI_TwoBoards_FullDuplex_DMA_Slave_Init/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on NUCLEO-N657X0-Q devices.

  - This example has been tested with NUCLEO-N657X0-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-N657X0-Q Set-up
    - Connect Master board PE15 to Slave Board PE15 (CN15, pin 11)
    - Connect Master board PG1 to Slave Board PG1 (CN15, pin 13)
    - Connect Master board PG2 to Slave Board PG2 (CN15, pin 15)
    - Connect Master board GND to Slave Board GND

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
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Project-trusted.bin) in NUCLEO-N657X0-Q board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.
 - Run the example









**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
