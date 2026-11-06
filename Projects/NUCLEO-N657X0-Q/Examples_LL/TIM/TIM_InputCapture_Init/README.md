## <b>TIM_InputCapture_Init Example Description</b>

Use of the TIM peripheral to measure a periodic signal frequency
provided either by an external signal generator or by
another timer instance. This example is based on the STM32N6xx TIM
LL API. The peripheral initialization uses LL unitary service functions
for optimization purposes (performance and size).

TIM1_CH1 is configured in input capture mode. The TIM1CLK frequency is set to
its maximum value (Prescaler is 1) in order to get the best possible resolution.

So the TIM1 counter clock is 400 MHz.

SystemCoreClock is set to 600 MHz for STM32N6xx Devices.

The "uwMeasuredFrequency" variable contains the measured signal frequency.

It is calculated within the capture/compare 1 interrupt service routine.

The minimum frequency value to measure is TIM1 counter clock / TIMx_CCR1 MAX
                                              = 400 MHz / 65535

Due to TIM1 interrupt routine processing time (around 1.4us), the maximum
frequency value to measure is around 8 MHz.

TIM4_CH1 is configured to generate a PWM signal.  USER push-button can be used to
change the frequency of this signal from 8 kHz up to 26 kHz by steps of 2 kHz.

It is then possible to run this example without a signal generator by connecting
TIM4_CH1 to TIM1_CH1.

### <b>Keywords</b>

Timer, TIM, Frequency, Input, Capture, External Signal, Measurement

### <b>Directory contents</b>

  - TIM/TIM_InputCapture_Init/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
  - TIM/TIM_InputCapture_Init/FSBL/Inc/main.h                  Header for main.c module
  - TIM/TIM_InputCapture_Init/FSBL/Inc/stm32_assert.h          Template file to include assert_failed function
  - TIM/TIM_InputCapture_Init/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
  - TIM/TIM_InputCapture_Init/FSBL/Src/main.c                  Main program
  - TIM/TIM_InputCapture_Init/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with NUCLEO-N657X0-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-N657X0-Q Set-up
    - When no signal generator is used TIM4_CH1 can be connected to TIM1_CH1:
      - TIM1_CH1  PE9: connected to pin 31 of CN15 connector
      - TIM4_CH1  PE5: connected to pin 4 of CN15 connector

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
