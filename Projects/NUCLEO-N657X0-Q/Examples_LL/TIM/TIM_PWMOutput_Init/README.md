## <b>TIM_PWMOutput_Init Example Description</b>

Use of a timer peripheral to generate a PWM output signal and update the PWM duty cycle.
This example is based on the STM32N6xx TIM LL API.
The peripheral initialization uses LL initialization function to demonstrate LL Init.

  In this example TIM1 input clock (TIM1CLK) is set to TIMPRE clock,
    since TIMPRE prescaler is equal to 1.
      TIMPRE = SYSB
      => TIM1CLK = HCLK = SystemBusClock (400 MHz)

To set the TIM1 counter clock frequency to 10 KHz, the pre-scaler (PSC) is
calculated as follows:

	PSC = (TIM1CLK / TIM1 counter clock) - 1
	PSC = ((SystemBusClock/1) /10 KHz) - 1

SystemCoreClock is set to 600 MHz for STM32N6xx Devices.

Auto-reload (ARR) is calculated to get a time base period of 10 ms,
meaning a time base frequency of 100 Hz.

	ARR = (TIM1 counter clock / time base frequency) - 1
	ARR = (TIM1 counter clock / 100) - 1

Initially, the capture/compare register (CCR1) of the output channel is set to
half the auto-reload value meaning a initial duty cycle of 50%.

Generally speaking this duty cycle is calculated as follows:

	Duty cycle = (CCR1 / ARR) * 100

The timer output channel must be connected to TIM1_CH1 on board NUCLEO-N657X0-Q.

Thus TIM1_CH1 status (on/off) mirrors the timer output level (active v.s. inactive).

USER push-button can be used to change the duty cycle from 0% up to 100% by
steps of 10%. Duty cycle is periodically measured. It can be observed through
the debugger by watching the variable uwMeasuredDutyCycle.

Initially the output channel is configured in output compare toggle mode.
### <b>Keywords</b>

Timers, TIM, Output, signal, PWM, Oscilloscope, Frequency, Duty cycle, Waveform

### <b>Directory contents</b>

  - TIM/TIM_PWMOutput_Init/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
  - TIM/TIM_PWMOutput_Init/FSBL/Inc/main.h                  Header for main.c module
  - TIM/TIM_PWMOutput_Init/FSBL/Inc/stm32_assert.h          Template file to include assert_failed function
  - TIM/TIM_PWMOutput_Init/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
  - TIM/TIM_PWMOutput_Init/FSBL/Src/main.c                  Main program
  - TIM/TIM_PWMOutput_Init/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.
    
  - This example has been tested with NUCLEO-N657X0-Q board and can be
    easily tailored to any other supported device and development board.
  - NUCLEO-N657X0-Q Set-up:
      - TIM1_CH1  PE9: connected to pin 31 of CN15 connector 

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
