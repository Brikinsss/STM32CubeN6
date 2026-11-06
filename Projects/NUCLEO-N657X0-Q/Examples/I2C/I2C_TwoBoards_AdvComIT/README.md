## <b>I2C_TwoBoards_AdvComIT Example Description</b>

How to handle several I2C data buffer transmission/reception between
a master and a slave device, using an interrupt.

    Board: NUCLEO-N657X0-Q (embeds a STM32N657X0H3Q device)
    - SCL Pin: PE5 ( Morpho CN15 pin4)
    - SDA Pin: PE6 ( Morpho CN15 pin2)

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.

The system clock runs at 600 MHz and external memory interface at the highest speed.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change I2C configuration.

The I2C communication is then initiated.

The project is split in two parts: the Master Board and the Slave Board.

- Master Board:
  The HAL_I2C_Master_Receive_IT() and the HAL_I2C_Master_Transmit_IT() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Master mode using interrupt.

- Slave Board:
  The HAL_I2C_Slave_Receive_IT() and the HAL_I2C_Slave_Transmit_IT() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Slave mode using interrupt.

The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file:

- If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
- If the Slave board is used the "#define MASTER_BOARD" must be commented.

For this example two buffers are used :

- aTxBuffer buffer contains the data to be transmitted 
- aRxBuffer buffer is used to save the received data

Note that both buffers have same size

On Master board side:

 - Wait USER push-button to be pressed (used for only synchronization at startup)
 - Request write operation by sending specific command "MASTER_REQ_WRITE" to Slave
 - Send the number of data to be written
 - Transmit aTxBuffer buffer to slave
 - Request read operation by sending specific command "MASTER_REQ_READ" to Slave
 - Send the number of data to be read
 - Receive data from Slave in aRxBuffer
 - Check the correctness of data and Toggle LED1 when data is received correctly,
   otherwise LED2 is turned ON and communication is stopped (using infinite loop)

On Slave board side:

 - Receive request from Master
 - Receive the request operation from Master and depending on the operation type (write or read):
   - If Master requests write operation:
      - Receive number of data to be written by Master
      - Receive data from master in aRxBuffer
      - Check the correctness of data and Toggle LED1 when data is received correctly,
        otherwise LED2 is turned ON and communication is stopped (using infinite loop)
   - If Master request read operation:
      - Receive number of data to be written to Master
      - Transmit aTxBuffer buffer to master
   
These operations are repeated periodically and the start of communication is triggered 
by pushing the key button of the Master board.  

#### <b>Notes</b>

 1. In Master side, only Acknowledge failure error is handled. When this error
    occurs Master restart the current operation until Slave acknowledges it's
    address.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Connectivity, Communication, I2C, Interrupt, Master, Slave, Transmission, Reception, Fast mode,
Command, Acknowledge

### <b>Directory contents</b> 

  - I2C/I2C_TwoBoards_AdvComIT/FSBL/Inc/stm32n6xx_nucleo_conf.h     BSP configuration file
  - I2C/I2C_TwoBoards_AdvComIT/FSBL/Inc/stm32n6xx_hal_conf.h        HAL configuration file
  - I2C/I2C_TwoBoards_AdvComIT/FSBL/Inc/stm32n6xx_it.h              I2C interrupt handlers header file
  - I2C/I2C_TwoBoards_AdvComIT/FSBL/Inc/main.h                      Header for main.c module
  - I2C/I2C_TwoBoards_AdvComIT/FSBL/Src/stm32n6xx_it.c              I2C interrupt handlers
  - I2C/I2C_TwoBoards_AdvComIT/FSBL/Src/main.c                      Main program
  - I2C/I2C_TwoBoards_AdvComIT/FSBL/Src/system_stm32n6xx.c          STM32N6xx system source file
  - I2C/I2C_TwoBoards_AdvComIT/FSBL/Src/stm32n6xx_hal_msp.c         HAL MSP file

### <b>Hardware and Software environment</b>

  - This example runs on STM32N6XX devices.

  - This example has been tested with NUCLEO-N657X0-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-N657X0-Q Set-up

    - Connect I2C_SCL line of Master board (PE5,  Morpho CN15 pin4) to I2C_SCL line of Slave Board (PE5,  Morpho CN15 pin4).
    - Connect I2C_SDA line of Master board (PE6,  Morpho CN15 pin2) to I2C_SDA line of Slave Board (PE6,  Morpho CN15 pin2).
    - Connect GND of Master board to GND of Slave Board.

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
