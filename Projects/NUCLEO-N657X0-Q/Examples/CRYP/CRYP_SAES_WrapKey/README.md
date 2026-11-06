## <b>CRYP_SAES_WrapKey Example Description</b>
  
How to use the Secure AES co-processor (SAES) peripheral to wrap application keys
using hardware secret key DHUK then use it to encrypt in polling mode.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.

The SystemClock_Config() function is used to configure the system clock for STM32N657X0HxQ Devices :
The CPU at 600 MHz.

The Secure AES instance is configured in wrap mode and hardware secret key DHUK is selected.

The goal here to not share application key with users.

First, application key is encrypted (the result is stored in Encryptedkey buffer).

Then, user decrypt Encryptedkey, now application key is loaded in SAES_KEYRx registers
(without knowing him), immediately usable by users for any SAES operation.

Finally, ECB encryption/decryption is performed using key already in SAES_KEYRx registers
and results are compared with expected ones.

The Secure AES mode requires:

1. Application keys (AESKey256) to be wrapped using DHUK(derived hardware unique key).

2. Plain text (Plaintext) which will be encrypted.

3. Cipher text (CiphertextAESECB256)  which is the encryption result of the Plaint text.

In this example, LED1 (led BLUE) is turned on when the operation finish with success .

and LED2 (led RED) is turned on when the operation generate an error.

#### <b>Notes</b>
1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

System, Security, SAES, AES,  TrustZone disabled

### <b>Directory contents</b>

  - CRYP/CRYP_SAES_WrapKey/FSBL/Src/main.c                         Main program
  - CRYP/CRYP_SAES_WrapKey/FSBL/Src/system_stm32n6xxc              STM32N6xx system clock configuration file
  - CRYP/CRYP_SAES_WrapKey/FSBL/Src/stm32n6xx_it.c                 Interrupt handlers
  - CRYP/CRYP_SAES_WrapKey/FSBL/Src/stm32n6xx_hal_msp.c            HAL MSP module
  - CRYP/CRYP_SAES_WrapKey/FSBL/Inc/main.h                         Main program header file
  - CRYP/CRYP_SAES_WrapKey/FSBL/Inc/stm32n6xx_nucleo_conf.h        BSP Configuration file
  - CRYP/CRYP_SAES_WrapKey/FSBL/Inc/stm32n6xx_hal_conf.h           HAL Configuration file
  - CRYP/CRYP_SAES_WrapKey/FSBL/Inc/stm32n6xx_it.h                 Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with STMicroelectronics NUCLEO-N657X0-Q
    board and can be easily tailored to any other supported device
    and development board.

  - **EWARM** : To monitor a variable in the live watch window, you must proceed as follow :
    - Start a debugging session.
    - Open the View > Images.
    - Double-click to deselect the second instance of project.out. 

  - **MDK-ARM** : it is not possible to monitor a variable in the live watch window    

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
