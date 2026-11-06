## <b>I2C_TwoBoards_MultiMasterIT_Slave Example Description</b>

How to handle I2C data buffer communication between two boards, using an interrupt and two Masters and one Slave.

      - Board: NUCLEO-N657X0-Q's (embeds a STM32N6XX device)
      - SCL Master1 Pin: PE5 (Morpho CN15 pin 4)
      - SDA Master1 Pin: PE6 (Morpho CN15 pin 2)
      - SCL Master2 Pin: PE13 (Morpho CN15 pin 28)
      - SDA Master2 Pin: PE14 (Morpho CN15 pin 26)
      - SCL Slave   Pin: PE5 (Morpho CN15 pin 4)
      - SDA Slave   Pin: PE6 (Morpho CN15 pin 2)
      - Connect GND between each board.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the
Flash interface and the systick.

The system clock runs at 600 MHz and external memory interface at the highest speed.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function. This later is calling the HAL_I2C_MspInit()
function which core is implementing the configuration of the needed I2C resources according to the used hardware (CLOCK,
GPIO and NVIC). You may update this function to change I2C configuration.

In a first step after initialization of I2C Master1, Master2 and Timer.
Each time the timer generate an interrupt, the associated callback HAL_TIM_periodlapsedCallback() is called and a
Master2 starts the communication by sending aTxBufferMaster2 through HAL_I2C_Master_Transmit_IT() to I2C Slave which
receives aRxBuffer through HAL_I2C_Slave_Receive_IT().

Then when user press the User push-button on the Master Board, I2C Master1 starts the communication by sending
aTxBufferMaster1 through HAL_I2C_Master_Transmit_IT() to I2C Slave which receives aRxBuffer through
HAL_I2C_Slave_Receive_IT().

Each time the slave retrieve a buffer, associated variable are incremented.
User can easily add to live watch the variable nbMst1RxCplt and nbMst2RxCplt to catch the Multimaster buffer reception
on Slave side.

NUCLEO-N657X0-Q's LED can be used to monitor the transfer status:

 - LED1 toggle each time the timer period elapsed.
 - LED1 is slowly blinking (1 sec. period) in case of error in initialization or transmission process.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

I2C, Master, Slave, TIM, Multimaster, Communication, Interrupt, Transmission, Reception

### <b>Directory contents</b>

  - I2C/I2C_TwoBoards_MultiMasterIT_Slave/FSBL/Inc/stm32n6xx_nucleo_conf.h     BSP configuration file
  - I2C/I2C_TwoBoards_MultiMasterIT_Slave/FSBL/Inc/stm32n6xx_hal_conf.h        HAL configuration file
  - I2C/I2C_TwoBoards_MultiMasterIT_Slave/FSBL/Inc/stm32n6xx_it.h              PSSI interrupt handlers header file
  - I2C/I2C_TwoBoards_MultiMasterIT_Slave/FSBL/Inc/main.h                      Header for main.c module
  - I2C/I2C_TwoBoards_MultiMasterIT_Slave/FSBL/Src/stm32n6xx_it.c              PSSI interrupt handlers
  - I2C/I2C_TwoBoards_MultiMasterIT_Slave/FSBL/Src/main.c                      Main program
  - I2C/I2C_TwoBoards_MultiMasterIT_Slave/FSBL/Src/system_stm32n6xx.c          stm32n6xx system source file
  - I2C/I2C_TwoBoards_MultiMasterIT_Slave/FSBL/Src/stm32n6xx_hal_msp.c         HAL MSP file

### <b>Hardware and Software environment</b>

  - This example runs on STM32N6XX devices.

  - This example has been tested with NUCLEO-N657X0-Q board and can be easily tailored to any other supported device and
    development board.

  - NUCLEO-N657X0-Q Set-up
    - Connect Master board PE5 (Morpho CN15 pin 4) to Slave Board PE5 (Morpho CN15 pin 4)
    - Connect Master board PE6 (Morpho CN15 pin 2) to Slave Board PE6 (Morpho CN15 pin 2)
    - Connect Master board PE13 (Morpho CN15 pin 28) to Slave Board PE5 (Morpho CN15 pin 4)
    - Connect Master board PE14 (Morpho CN15 pin 26) to Slave Board PE6 (Morpho CN15 pin 2)
    - Connect Master board GND to Slave Board GND

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
