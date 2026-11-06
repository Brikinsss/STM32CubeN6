## <b>USART_Communication_Rx_IT_Continuous_VCP_Init Example Description</b>

This example shows how to configure GPIO and USART peripheral for continuously receiving characters
from HyperTerminal (PC) in Asynchronous mode using Interrupt mode. Peripheral initialization is
done using LL unitary services functions for optimization purpose (performance and size).

USART Peripheral is configured in asynchronous mode (115200 bauds, 8 data bit, 1 start bit, 1 stop bit, no parity).

No HW flow control is used.

GPIO associated to User push-button is linked with EXTI.

Example execution:

After startup from reset and system configuration, LED1 is blinking quickly.

On press on push button, USART RX Not Empty interrupt is enabled.

When character is received on USART Rx line, a RXNE interrupt occurs.

Received data is read from USART RDR register and stored in user buffer.

A double buffer is available for received data, allowing continuous reception.

User could process data received in Buffer A, while buffer B is used for reception.

When buffer B is full, it could be handled by user, while buffer A becomes active buffer for next reception, and so on.

Each time a reception buffer is full, user data process callback is called.

Data processing consists in echoing data buffer content on PC Com port, and making LED1 toggle.

In case of errors, LED1 is blinking.

Virtual Com port feature of STLINK could be used for UART communication between board and PC.

### <b>Keywords</b>

Connectivity, UART/USART, Asynchronous, RS-232, baud rate, Interrupt, HyperTerminal, Continuous Reception, VCP,
Receiver

### <b>Directory contents</b>

  - USART/USART_Communication_Rx_IT_Continuous_VCP_Init/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
  - USART/USART_Communication_Rx_IT_Continuous_VCP_Init/FSBL/Inc/main.h                  Header for main.c module
  - USART/USART_Communication_Rx_IT_Continuous_VCP_Init/FSBL/Inc/stm32_assert.h          Template file to include assert_failed function
  - USART/USART_Communication_Rx_IT_Continuous_VCP_Init/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
  - USART/USART_Communication_Rx_IT_Continuous_VCP_Init/FSBL/Src/main.c                  Main program
  - USART/USART_Communication_Rx_IT_Continuous_VCP_Init/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with NUCLEO-N657X0-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-N657X0-Q Set-up
    Please ensure that USART communication between the target MCU and ST-LINK MCU is properly enabled
	on HW board in order to support Virtual Com Port (Default HW SB configuration allows use of VCP)

  - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration
    (115200 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control).

  - Launch the program. Enter characters on PC communication SW side.

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
