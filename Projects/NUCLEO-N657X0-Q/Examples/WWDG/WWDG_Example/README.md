## <b>WWDG_Example Example Description</b>

Configuration of the HAL API to periodically update the WWDG counter and simulate a software fault that
generates an MCU WWDG reset when a predefined time period has elapsed.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.

Then the SystemClock_Config() function is used to configure the system
clock to run at 600 MHz.

The WWDG peripheral configuration is ensured by the HAL_WWDG_Init() function.
This later is calling the HAL_WWDG_MspInit()function which core is implementing
the configuration of the needed WWDG resources according to the used hardware (CLOCK
and NVIC). You may update this function to change WWDG configuration.

The WWDG timeout is set, through counter value, to 335.488 ms.
The refresh window is set in order to make user wait 125.376 ms after a wadchdog refresh,
before writing again counter. Hence the WWDG counter is refreshed each (125.376 + 1) ms in the
main program infinite loop to prevent a WWDG reset.

**LED1** is toggling at same frequency, indicating that the program is running.


An EXTI Line is connected to a GPIO pin, and configured to generate an interrupt
on the falling edge of the signal.

The EXTI Line is used to simulate a software failure: once the EXTI Line event
occurs by pressing the USER push-button (PC.13), the corresponding interrupt is served.

In the ISR, a write to invalid address generates a Hardfault exception containing
an infinite loop and preventing to return to main program (the WWDG counter is
not refreshed).
As a result, when the WWDG counter falls to 0x3F, WWDG reset occurs.

If the WWDG reset is generated, after the system resumes from reset, **LED1** is turned ON for 4 seconds.

If the EXTI Line event does not occur, the WWDG counter is indefinitely refreshed
in the main program infinite loop, and there is no WWDG reset.

**LED2** is turned ON and remains ON if any error occurs.

#### <b>Notes</b>

 1. This example must be tested in standalone mode (not in debug).

 2. Care must be taken when using HAL_Delay(), this function provides accurate
    delay (in milliseconds) based on variable incremented in SysTick ISR. This
    implies that if HAL_Delay() is called from a peripheral ISR process, then
    the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.


### <b>Keywords</b>

System, WWDG, EXTI, update counter, MCU Reset, Timeout, Software fault

### <b>Directory contents</b>

  - WWDG/WWDG_Example/Inc/stm32n6xx_nucleo_conf.h BSP configuration file
  - WWDG/WWDG_Example/Inc/stm32n6xx_hal_conf.h    HAL configuration file
  - WWDG/WWDG_Example/Inc/stm32n6xx_it.h          Interrupt handlers header file
  - WWDG/WWDG_Example/Inc/main.h                  Header for main.c module
  - WWDG/WWDG_Example/Src/stm32n6xx_it.c          Interrupt handlers
  - WWDG/WWDG_Example/Src/main.c                  Main program
  - WWDG/WWDG_Example/Src/stm32n6xx_hal_msp.c     HAL MSP file
  - WWDG/WWDG_Example/Src/system_stm32n6xx.c      STM32N6xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with NUCLEO-N657X0-Q board and can be
    easily tailored to any other supported device and development board.


  - **EWARM** : To monitor a variable in the live watch window, you must proceed as follow :
    - Start a debugging session.
    - Open the View > Images.
    - Double-click to deselect the second instance of project.out.

  - **MDK-ARM** : To monitor a variable in the live watch window, you must comment out SCB_EnableDCache() in main() function.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files
 
 This example doesn't run when booROM is in development mode.
 User can boot in external Flash as explained below.
 
 This program can be run in boot from flash mode. This is done by following the instructions below :

 - Resort to CubeProgrammer to add a header to the generated binary Project.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin Project.bin -nk -of 0x80000000 -t fsbl -o Project-trusted.bin -hv 2.3 -dump Project-trusted.bin*
   - The resulting binary is Project-trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Project-trusted.bin) in the board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.
 


**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
