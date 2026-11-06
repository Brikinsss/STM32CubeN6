## <b>SPI_FullDuplex_CrcComIT_Master Example Description</b>

Data buffer transmission/reception with CRC between two boards via SPI using IT.

	Board: NUCLEO-N657X0-Q (embeds a STM32N657X0H3Q device)
	CLK Pin: PE.15 (Pin 11  on CN15 connector (Arduino D13 in 6 CN14))
	MISO Pin: PG.01 (Pin 13  on CN15 connector (Arduino D12 in 5 CN14))
	MOSI Pin: PG.02 (Pin 15  on CN15 connector (Arduino D11 in 4 CN14))

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.

The system clock runs at 600 MHz and external memory interface at the highest speed.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.

This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK,
GPIO, DMA and NVIC). You may update this function to change SPI configuration.

The SPI communication is then initiated.

The HAL_SPI_TransmitReceive_DMA() function allows the reception and the
transmission of a predefined data buffer at the same time (Full Duplex Mode).

If the Master board is used, the project SPI_FullDuplex_CrcComIT_Master must be used.

If the Slave board is used, the project SPI_FullDuplex_CrcComIT_Slave must be used.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the USER push-button, SPI Master starts the
communication by sending aTxBuffer and receiving aRxBuffer through
HAL_SPI_TransmitReceive_DMA(), at the same time SPI Slave transmits aTxBuffer
and receives aRxBuffer through HAL_SPI_TransmitReceive_DMA().

The callback functions (HAL_SPI_TxRxCpltCallback and HAL_SPI_ErrorCallbackand) update
the variable wTransferState used in the main function to check the transfer status.

Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to
check buffers correctness.

STM32 board's LEDs can be used to monitor the transfer status:

 - LED1 turns ON when the transmission process is complete.
 - LED1 turns ON when the reception process is complete.
 - LED2 turns ON when there is an error in transmission/reception process.

#### <b>Notes</b>

 1. You need to perform a reset on Slave board, then perform it on Master board
    to have the correct behaviour of this example.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Connectivity, SPI, CRC, Full-duplex, Interrupt, Transmission, Reception, Master, Slave, MISO, MOSI,

### <b>Directory contents</b>

  - SPI/SPI_FullDuplex_CrcComIT_Master/FSBL/Inc/stm32n6xx_nucleo_conf.h          BSP configuration file
  - SPI/SPI_FullDuplex_CrcComIT_Master/FSBL/Inc/stm32n6xx_hal_conf.h             HAL configuration file
  - SPI/SPI_FullDuplex_CrcComIT_Master/FSBL/Inc/stm32n6xx_it.h                   Interrupt handlers header file
  - SPI/SPI_FullDuplex_CrcComIT_Master/FSBL/Inc/main.h                           Header for main.c module
  - SPI/SPI_FullDuplex_CrcComIT_Master/FSBL/Src/stm32n6xx_it.c                   Interrupt handlers
  - SPI/SPI_FullDuplex_CrcComIT_Master/FSBL/Src/main.c                           Main program
  - SPI/SPI_FullDuplex_CrcComIT_Master/FSBL/Src/system_stm32n6xx.c               stm32n6xx system source file
  - SPI/SPI_FullDuplex_CrcComIT_Master/FSBL/Src/stm32n6xx_hal_msp.c              HAL MSP file

### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with NUCLEO-N657X0-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-N657X0-Q Set-up
    - Connect Master board PE.15 (Pin 11 on CN15 connector (Arduino D13 in 6 CN14)) to Slave Board PE.15 (Pin 11 on CN15 connector (Arduino D13 in 6 CN14))
    - Connect Master board PG.01 (Pin 13 on CN15 connector (Arduino D12 in 5 CN14)) to Slave Board PG.01 (Pin 13 on CN15 connector (Arduino D12 in 5 CN14))
    - Connect Master board PG.02 (Pin 15 on CN15 connector (Arduino D11 in 4 CN14)) to Slave Board PG.02 (Pin 15 on CN15 connector (Arduino D11 in 4 CN14))
    - Connect Master board GND  to Slave Board GND

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
