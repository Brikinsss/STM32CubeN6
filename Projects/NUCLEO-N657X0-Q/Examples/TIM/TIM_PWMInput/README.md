## <b>TIM_PWMInput Example Description</b>

How to use the TIM peripheral to measure the frequency and
duty cycle of an external signal.

The TIM1CLK frequency is set to 400 (MHz), the Prescaler is 0 so the
counter clock is SystemCoreClock 400 (MHz).

The system clock runs at 600 MHz and external memory interface at the highest speed.


TIM1 is configured in PWM Input Mode: the external signal is connected to
TIM1 Channel2 used as input pin.

To measure the frequency and the duty cycle, we use the TIM1 CC2 interrupt request,
so in the timer IRQ routine (via call to function HAL_TIM_IC_CaptureCallback() ),
the frequency and the duty cycle of the external signal are computed.

"uwFrequency" variable contains the external signal frequency:

TIM1 counter clock = 400000000,

uwFrequency = TIM1 counter clock / TIM1_CCR2 in Hz,

"uwDutyCycle" variable contains the external signal duty cycle:

uwDutyCycle = (TIM1_CCR1*100)/(TIM1_CCR2) in %.

The minimum frequency value to measure is (TIM1 counter clock / CCR MAX)

                                         = (400 MHz)/ 65535

In case of error, LED2 is turned ON.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. This example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Timer, TIM, Input, signals, PWM, External signal, Frequency, Duty cycle, Measure

### <b>Directory contents</b>

  - TIM/TIM_PWMInput/FSBL/Inc/stm32n6xx_nucleo_conf.h    BSP configuration file
  - TIM/TIM_PWMInput/FSBL/Inc/stm32n6xx_hal_conf.h       HAL configuration file
  - TIM/TIM_PWMInput/FSBL/Inc/stm32n6xx_it.h             Interrupt handlers header file
  - TIM/TIM_PWMInput/FSBL/Inc/main.h                     Header for main.c module  
  - TIM/TIM_PWMInput/FSBL/Src/stm32n6xx_it.c             Interrupt handlers
  - TIM/TIM_PWMInput/FSBL/Src/main.c                     Main program
  - TIM/TIM_PWMInput/FSBL/Src/stm32n6xx_hal_msp.c        HAL MSP file
  - TIM/TIM_PWMInput/FSBL/Src/system_stm32n6xx.c         STM32N6xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with STMicroelectronics NUCLEO-N657X0-Q
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-N657X0-Q Set-up
    - Connect the external signal to measure to the TIM1 CH2 pin (PE11) (pin 23 in CN15 connector).

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
