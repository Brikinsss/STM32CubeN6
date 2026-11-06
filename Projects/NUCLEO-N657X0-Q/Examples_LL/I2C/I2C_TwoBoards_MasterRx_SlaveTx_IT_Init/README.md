## <b>I2C_TwoBoards_MasterRx_SlaveTx_IT_Init Example Description</b>

How to handle the reception of one data byte from an I2C slave device
by an I2C master device. Both devices operate in interrupt mode. The peripheral
is initialized with LL unitary service functions to optimize for performance
and size.

This example guides you through the different configuration steps by mean of LL API
to configure GPIO and I2C peripherals using two NUCLEO-N657X0-Q.

       Boards: NUCLEO-N657X0-Q (embeds a STM32N657X0H3Q device)
       - SCL Pin: PE.5 (CN15, pin 4)
       - SDA Pin: PE.6 (CN15, pin 2)

The project is split in two parts the Master Board and the Slave Board.

- Master Board: 
  I2C1 Peripheral is configured in Master mode with EXTI (Fast Mode).
  And GPIO associated to USER push-button is linked with EXTI.

- Slave Board: 
  I2C1 Peripheral is configured in Slave mode with EXTI (Fast Mode, Own address 7-bit enabled).

The user can choose between Master and Slave through "#define SLAVE_BOARD"
in the "main.h" file:

- Comment "#define SLAVE_BOARD" to select Master board.
- Uncomment "#define SLAVE_BOARD" to select Slave board.

The user can disable internal pull-up by opening ioc file.

For that, user can follow the procedure :

1- Double click on the I2C_TwoBoards_MasterRx_SlaveTx_IT_Init.ioc file
2- When CUBEMX tool is opened, select System Core category
3- Then in the configuration of GPIO/I2C1, change Pull-up to No pull-up and no pull-down for the both pins
4- Last step, generate new code thanks to button "GENERATE CODE"
The example is updated with no pull on each pin used for I2C communication

LED1 blinks quickly on BOARD MASTER to wait for USER push-button press.

Example execution:

Press the USER push-button on BOARD MASTER to initiate a read request by Master.
This action will generate an I2C start condition with the Slave address and a read bit condition.
When address Slave match code is received on I2C1 of BOARD SLAVE, an ADDR interrupt occurs.
I2C1 Slave IRQ Handler routine is then checking Address Match Code and direction Read.
This will allow Slave to enter in transmitter mode and then send a byte when TXIS interrupt occurs.
When byte is received on I2C1 of BOARD MASTER, an RXNE interrupt occurs.
When RXDR register is read, Master auto-generate a NACK and STOP condition
to inform the Slave that the transfer is finished.
The NACK condition generate a NACK interrupt in Slave side treated in the I2C1 Slave IRQ handler routine by a clear of NACK flag.
The STOP condition generate a STOP interrupt in both side (Slave and Master). Both I2C1 IRQ handler routine are then
clearing the STOP flag in both side.

LED1 is On :

- Slave side if transfer sequence is completed.
- Master side if data is well received.

In case of errors, LED1 is blinking slowly (1s).

### <b>Keywords</b>

Connectivity, Communication, I2C, Interrupt, Master Rx, Slave Tx, Transmission, Reception, Fast mode, SDA, SCL


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

    - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT_Init/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
    - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT_Init/FSBL/Inc/main.h                  Header for main.c module
    - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT_Init/FSBL/Inc/stm32_assert.h          Template file to include assert_failed function
    - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT_Init/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
    - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT_Init/FSBL/Src/main.c                  Main program
    - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT_Init/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with STM32N657X0HxQ board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-N657X0-Q Set-up
    - Connect GPIOs connected to Board Slave I2C1 SCL/SDA (PE.5 and PE.6)
    to respectively Board Master SCL and SDA pins of I2C1 (PE.5 and PE.6).
      - I2C1_SCL  PE.5 (CN15, pin 4) : connected to I2C1_SCL PE.5 (CN15, pin 4)
      - I2C1_SDA  PE.6 (CN15, pin 2) : connected to I2C1_SDA PE.6 (CN15, pin 2)
    - Connect Master board GND to Slave Board GND

  - Launch the program.
  - Press USER push-button to initiate a read request by Master
      then Slave send a byte.

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
