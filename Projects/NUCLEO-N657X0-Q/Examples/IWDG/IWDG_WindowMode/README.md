## <b>IWDG_WindowMode Example Description</b>

How to periodically update the IWDG reload counter and simulate a software fault that generates
an MCU IWDG reset after a preset laps of time.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.

Then the SystemClock_Config() function is used to configure the system
clock to run at 600 MHz.

The IWDG time-out is set to 762 ms (the time-out may varies due to LSI frequency
dispersion).

The Window option is enabled with a window register value set to 400 ms.

To prevent a reset, the down-counter must be reloaded when its value is:

 - Lower than the window register value (400ms)
 - Greater than 0x0
 
The IWDG counter is therefore refreshed each 450 ms in the main program infinite loop to
prevent a IWDG reset (762 - 450 = 312 within the interval).

LED1 is also toggled each 450 ms indicating that the program is running.

LED2 will turn on if any error occurs.

An EXTI Line is connected to a GPIO pin, configured to generate an interrupt
when the USER push-button (PC.13) is pressed.

Once the EXTI Line event occurs by pressing the USER push-button (PC.13),
the refresh period is set to 200 ms.
That will make refresh being outside window value. As a result, when the IWDG counter is reloaded,
the IWDG reset occurs.

In the ISR, a write to invalid address generates a Hard fault exception containing
an infinite loop and preventing to return to main program (the IWDG counter is
not refreshed).
As a result, when the IWDG counter falls to 0, the IWDG reset occurs.
If the IWDG reset is generated, after the system resumes from reset, LED1 turns on for 4 seconds.

If the EXTI Line event does not occur, the IWDG counter is indefinitely refreshed
in the main program infinite loop, and there is no IWDG reset.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate
    delay (in milliseconds) based on variable incremented in SysTick ISR. This
    implies that if HAL_Delay() is called from a peripheral ISR process, then
    the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

System, IWDG, reload counter, MCU Reset, Window mode, Timeout, Software fault

### <b>Directory contents</b>

  - IWDG/IWDG_WindowMode/Inc/stm32n6xx_nucleo_conf.h      BSP configuration file
  - IWDG/IWDG_WindowMode/FSBL/Inc/stm32n6xx_hal_conf.h    HAL configuration file
  - IWDG/IWDG_WindowMode/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
  - IWDG/IWDG_WindowMode/FSBL/Inc/main.h                  Header for main.c module
  - IWDG/IWDG_WindowMode/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
  - IWDG/IWDG_WindowMode/FSBL/Src/main.c                  Main program
  - IWDG/IWDG_WindowMode/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file

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
