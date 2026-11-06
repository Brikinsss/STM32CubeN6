## <b>HASH_HMAC_SHA2_384 example Description</b>

This example provides a short description of how to use the HASH peripheral to 
hash data using long key and SHA_384 Algorithm.

For this example, DMA is not used for data transfer, the CPU is using HASH peripheral in
polling mode.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.

The system clock runs at 600 MHz and external memory interface at the highest speed.

The key used is long type (2088 bits = 261 bytes).

The HMAC SHA-384 message digest result is a 384-bit long (48 bytes) data

The expected HASH digests (Msg261_Key261_expectedHmacSHA384) are already computed using an online
HASH tool. Those values are compared to those computed by the HASH (Sha384Hmacoutput).

In case there is a mismatch the red LED is turned ON (LED2).

In case the SHA384 digest is computed correctly the green LED (LED1) is turned ON.

###### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

System, Security, HASH, HMAC, SHA384, digest, key

### <b>Directory contents</b>

      - HASH/HASH_HMAC_SHA2_384/FSBL/Inc/stm32n6xx_nucleo_conf.h    BSP configuration file
      - HASH/HASH_HMAC_SHA2_384/FSBL/Inc/stm32n6xx_hal_conf.h       HAL configuration file
      - HASH/HASH_HMAC_SHA2_384/FSBL/Inc/stm32n6xx_it.h             Interrupt handlers header file
      - HASH/HASH_HMAC_SHA2_384/FSBL/Inc/main.h                     Header for main.c module  
      - HASH/HASH_HMAC_SHA2_384/FSBL/Src/stm32n6xx_it.c             Interrupt handlers
      - HASH/HASH_HMAC_SHA2_384/FSBL/Src/main.c                     Main program
      - HASH/HASH_HMAC_SHA2_384/FSBL/Src/stm32n6xx_hal_msp.c        HAL MSP module
      - HASH/HASH_HMAC_SHA2_384/FSBL/Src/system_stm32n6xx.c         STM32N6xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with a STM32N657X0HxQ embedded on an
    NUCLEO-N657X0-Q board and can be easily tailored to any other supported
    device and development board.

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
