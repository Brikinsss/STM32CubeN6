## <b>BSEC_ShadowRegisters Example Description</b>

This project is targeted to run on STM32N6xx device on NUCLEO-N657X0 board from STMicroelectronics.

This project runs from the internal RAM and must be executed in booting from external Flash.

This example shows the use of shadow registers, scratch registers and reset counters in the BSEC peripheral.
It first reads a scratch registers. Its value should be 0.
Then, it reads the BSEC reset counter and saves the value in that same scratch register.
Shadow register operations are then shown :

  - Read a shadow register

  - Write a modified value to a shadow register

  - Lock writing and reloading the shadow register

  - Show that writing and reloading that register is not possible

The device then performs a system reset.
The scratch value is kept after a system reset, allowing the example to run its second part.
The previously written and locked shadow register is now read to verify that the value was not kept after reset.
The value is modified and written to demonstrate that the write lock was lifted by the system reset.

If the example runs as expected, the green LED blinks. Otherwise the red LED blinks.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.


### <b>Keywords</b>

BSEC, SRAM

### <b>Directory contents</b>

File | Description
 --- | ---
  BSEC/BSEC_ShadowRegisters/FSBL/Inc/main.h                   | Header for main.c module
  BSEC/BSEC_ShadowRegisters/FSBL/Inc/stm32n6xx_hal_conf.h     | HAL Configuration file
  BSEC/BSEC_ShadowRegisters/FSBL/Inc/stm32n6xx_it.h           | Interrupt handlers header file
  BSEC/BSEC_ShadowRegisters/FSBL/Inc/stm32n6xx_nucleo_conf.h  | BSP Configuration file
  BSEC/BSEC_ShadowRegisters/FSBL/Src/main.c                   | Main program
  BSEC/BSEC_ShadowRegisters/FSBL/Src/stm32n6xx_hal_msp.c      | HAL MSP module
  BSEC/BSEC_ShadowRegisters/FSBL/Src/stm32n6xx_it.c           | Interrupt handlers
  BSEC/BSEC_ShadowRegisters/FSBL/Src/system_stm32n6xx_fsbl.c  | STM32N6xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32N6xx devices.

  - This example has been tested with STMicroelectronics NUCLEO-N657X0
    board and can be easily tailored to any other supported device
    and development board.

  - **EWARM** : To monitor a variable in the live watch window, you must proceed as follow :
    - Start a debugging session.
    - Open the View > Images.
    - Double-click to deselect the second instance of project.out.

  - **MDK-ARM** : To monitor a variable in the live watch window, you must comment out SCB_EnableDCache() in main() function.

### <b>How to use it ?</b>

 In order to make the program work, you must do the following :

 - Set BOOT1 switch in 2-3 position (BOOT0 switch position doesn't matter)
 - Open your preferred toolchain
 - Rebuild all files
 - Resort to CubeProgrammer to add a header to the generated binary Project.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin Project.bin -nk -of 0x80000000 -t fsbl -o Project-trusted.bin -hv 2.3 -dump Project-trusted.bin*
   - The resulting binary is Project-trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Project-trusted.bin) in Nucleo board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.




**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
