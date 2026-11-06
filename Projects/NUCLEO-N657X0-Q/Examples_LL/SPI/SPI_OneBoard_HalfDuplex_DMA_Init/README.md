## <b>SPI_OneBoard_HalfDuplex_DMA_Init Example Description</b>

Configuration of GPIO and SPI peripherals to transmit
bytes from an SPI Master device to an SPI Slave device in DMA mode. This example
is based on the STM32N6xx SPI LL API. The peripheral initialization uses the
LL initialization function to demonstrate LL init usage.

This example works with only one NUCLEO-N657X0-Q.

SPI1 Peripheral is configured in Master mode Half-Duplex Tx.
SPI5 Peripheral is configured in Slave mode Half-Duplex Rx.
GPIO associated to User push-button is linked with EXTI.

Example execution:

LED1 is blinking Fast (200ms) and wait USER push-button action.
Press USER push-button on BOARD start a Half-Duplex communication through DMA.
On MASTER side (SPI1), Clock will be generated on SCK line, Transmission done on MOSI Line.
On SLAVE side (SPI5) reception is done through the MISO Line.

LED1 is On if data is well received.

In case of errors, LED1 is blinking Slowly (1s).

### <b>Keywords</b>

Connectivity, SPI, Half-duplex, Transmission, Reception, Master, Slave, MISO, MOSI, DMA

### <b>Directory contents</b>

    - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
    - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/FSBL/Inc/main.h                  Header for main.c module
    - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/FSBL/Inc/stm32_assert.h          Template file to include assert_failed function
    - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
    - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/FSBL/Src/main.c                  Main program
    - SPI/SPI_OneBoard_HalfDuplex_DMA_Init/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on NUCLEO-N657X0-Q devices.

  - This example has been tested with NUCLEO-N657X0-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-N657X0-Q Set-up
    - Connect Master SCK  PC0 to Slave SCK  PE15
    - Connect Master MOSI PC3 to Slave MISO PG1

Relation with Board connector:

  - PC0 is connected to pin 17 of CN3 connector
  - PC3 is connected to pin 25 of CN3 connector
  - PE15 is connected to pin 11 of CN15 connector (Arduino D13 in 6 CN14)
  - PG1 is connected to pin 13 of CN15 connector (Arduino D12 in 5 CN14)

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
