## <b>I2C_TwoBoards_MasterTx_SlaveRx_Init Example Description</b>

How to transmit data bytes from an I2C master device using polling mode
to an I2C slave device using interrupt mode. The peripheral is initialized
with LL unitary service functions to optimize for performance and size.

This example guides you through the different configuration steps by mean of LL API
to configure GPIO and I2C peripherals using two NUCLEO-N657X0-Q.

       Boards: NUCLEO-N657X0-Q (embeds a STM32N657X0H3Q device)
       - SCL Pin: PE.5 (CN15, pin 4)
       - SDA Pin: PE.6 (CN15, pin 2)

The project is split in two parts the Master Board and the Slave Board.

- Master Board: 
  I2C1 Peripheral is configured in Master mode (Clock 200Mhz).
  And GPIO associated to USER push-button is linked with EXTI.

- Slave Board: 
  I2C1 Peripheral is configured in Slave mode (Clock 200Mhz, Own address 7-bit enabled).

The user can choose between Master and Slave through "#define SLAVE_BOARD"
in the "main.h" file:

- Comment "#define SLAVE_BOARD" to select Master board.
- Uncomment "#define SLAVE_BOARD" to select Slave board.

The user can disable internal pull-up by opening ioc file.

For that, user can follow the procedure :

1- Double click on the I2C_TwoBoards_MasterTx_SlaveRx_Init.ioc file
2- When CUBEMX tool is opened, select System Core category
3- Then in the configuration of GPIO/I2C1, change Pull-up to No pull-up and no pull-down for the both pins
4- Last step, generate new code thanks to button "GENERATE CODE"
The example is updated with no pull on each pin used for I2C communication

A first program launch, BOARD SLAVE waiting Address Match code through Handle_I2C_Slave() routine.
LED1 blinks quickly on BOARD MASTER to wait for USER push-button press.

Example execution:

Press the USER push-button on BOARD MASTER to initiate a write request by Master through Handle_I2C_Master() routine.
This action will generate an I2C start condition with the Slave address and a write bit condition.
When address Slave match code is received on I2C1 of BOARD SLAVE, an ADDR event occurs.
Handle_I2C_Slave() routine is then checking Address Match Code and direction Write.
This will allow Slave to enter in receiver mode and then acknowledge Master to send the bytes.
When acknowledge is received on I2C1 (Master), a TXIS event occurs.
This will allow Master to transmit a byte to the Slave.
Each time a byte is received on I2C1 (Slave), an RXNE event occurs until a STOP condition.
And so each time the Slave acknowledge the byte received a TXIS event occurs on Master side.
Master auto-generate a Stop condition when size of data to transmit is achieved.

The STOP condition generate a STOP event and initiate the end of reception on Slave side.
Handle_I2C_Slave() and Handle_I2C_Master() routine are then clearing the STOP flag in both side.

LED1 is On :

- Slave side if data are well received.
- Master side if transfer sequence is completed.

In case of errors, LED1 is blinking slowly (1s).

### <b>Keywords</b>

Connectivity, Communication, I2C, Master Rx, Slave Tx, Transmission, Reception, Fast mode, SDA, SCL


#### <b>Notes</b>

 1. If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),
    it is recommended to configure the latters as Write Through.
    This is ensured by configuring the memory attributes at MPU level in order to ensure cache coherence on SRAM1 and SRAM2.
    Please, refer to Template project for a typical MPU configuration.

 2. If external memory is shared between several processors, it is recommended to configure it as Write Back (bufferable), shareable and cacheable.
    The memory base address and size must be properly updated.
    The user needs to manage the cache coherence at application level.

For more details about the MPU configuration and use, please refer to AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”

### <b>Directory contents</b> 

    - I2C/I2C_TwoBoards_MasterTx_SlaveRx_Init/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
    - I2C/I2C_TwoBoards_MasterTx_SlaveRx_Init/FSBL/Inc/main.h                  Header for main.c module
    - I2C/I2C_TwoBoards_MasterTx_SlaveRx_Init/FSBL/Inc/stm32_assert.h          Template file to include assert_failed function
    - I2C/I2C_TwoBoards_MasterTx_SlaveRx_Init/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
    - I2C/I2C_TwoBoards_MasterTx_SlaveRx_Init/FSBL/Src/main.c                  Main program
    - I2C/I2C_TwoBoards_MasterTx_SlaveRx_Init/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with STM32N657X0HxQ board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-N657X0-Q Set-up
    - Connect GPIOs connected to Board Slave I2C1 SCL/SDA (PE.5 and PE.6)
    to respectively Board Master SCL and SDA pins of I2C1 (PE.6 and PE.5).
      - I2C1_SCL  PE.5 (CN15, pin 4) : connected to I2C1_SCL PE.6 (CN15, pin 2)
      - I2C1_SDA  PE.6 (CN15, pin 2) : connected to I2C1_SDA PE.5 (CN15, pin 4)
    - Connect Master board GND to Slave Board GND

  - Launch the program.
  - Press USER push-button to initiate a write request by Master
      then Slave receive bytes.

  - **EWARM** : To monitor a variable in the live watch window, you must proceed as follow :
    - Start a debugging session.
    - Open the View > Images.
    - Double-click to deselect the second instance of project.out.

  - **MDK-ARM** : To monitor a variable in the live watch window, you must comment out SCB_EnableDCache() in main() function.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Set the boot mode in development mode (BOOT1 switch position is 2-3, BOOT0 switch position doesn't matter).
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory (The user can choose between Master
   and Slave target through "#define SLAVE_BOARD" in the "main.h" file):
      - Comment "#define SLAVE_BOARD" and load the project in Master Board
      - Uncomment "#define SLAVE_BOARD" and load the project in Slave Board
   Code can be executed in this mode for debugging purposes.
        
 Next, this program can be run in boot from flash mode. This is done by following the instructions below:
 
 - Resort to CubeProgrammer to add a header to the generated binary Project.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin Project.bin -nk -of 0x80000000 -t fsbl -o Project-trusted.bin -hv 2.3 -dump Project-trusted.bin*
   - The resulting binary is Project-trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Project-trusted.bin) in the board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.



**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
