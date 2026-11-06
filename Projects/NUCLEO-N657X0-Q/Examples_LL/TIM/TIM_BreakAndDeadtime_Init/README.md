## <b>TIM_BreakAndDeadtime_Init Example Description</b>

Configuration of the TIM peripheral to
generate three center-aligned PWM and complementary PWM signals,
insert a defined deadtime value,
use the break feature,
and lock the break and dead-time configuration.

This example is based on the STM32N6xx TIM LL API. The peripheral initialization
uses LL initialization function to demonstrate LL Init.

TIM1CLK is fixed to 400 MHz, the TIM1 Prescaler is set to have
TIM1 counter clock = 10 MHz.

The TIM1 auto-reload is set to generate PWM signal at 100 Hz:

The Three Duty cycles are computed as the following description:

The channel 1 duty cycle is set to 50% so channel 1N is set to 50%.

The channel 2 duty cycle is set to 25% so channel 2N is set to 75%.

The channel 3 duty cycle is set to 12.5% so channel 3N is set to 87.5%.

A dead time equal to 4 us is inserted between
the different complementary signals, and the Lock level 1 is selected.

  - The OCx output signal is the same as the reference signal except for the rising edge,
    which is delayed relative to the reference rising edge.
  - The OCxN output signal is the opposite of the reference signal except for the rising
    edge, which is delayed relative to the reference falling edge

**Note** that calculated duty cycles apply to the reference signal (OCxREF) from
which outputs OCx and OCxN are generated. As dead time insertion is enabled the
duty cycle measured on OCx will be slightly lower.

The break Polarity is used at High level.

The TIM1 waveforms can be displayed using an oscilloscope.

### <b>Keywords</b>

Timer, TIM, PWM, Signal, Duty cycle, Dead-time, Break-time, Break polarity, Oscilloscope.

### <b>Directory contents</b>

    - TIM/TIM_BreakAndDeadtime_Init/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
    - TIM/TIM_BreakAndDeadtime_Init/FSBL/Inc/main.h                  Header for main.c module
    - TIM/TIM_BreakAndDeadtime_Init/FSBL/Inc/stm32_assert.h          Template file to include assert_failed function
    - TIM/TIM_BreakAndDeadtime_Init/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
    - TIM/TIM_BreakAndDeadtime_Init/FSBL/Src/main.c                  Main program
    - TIM/TIM_BreakAndDeadtime_Init/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with NUCLEO-N657X0-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-N657X0-Q Set-up

    - Connect the TIM1 pins to an oscilloscope to monitor the different waveforms:
      - TIM1_CH1  PE9: connected to pin 31 of CN15 connector
      - TIM1_CH1N PE8: connected to pin 15 of CN16 connector
      - TIM1_CH2  PE11: connected to pin 23 of CN15 connector
      - TIM1_CH2N PE2: connected to pin 30 of CN15 connector
      - TIM1_CH3  PF6: connected to pin 32 of CN3 connector
      - TIM1_CH3N PE12: connected to pin 1 of CN15 connector

    - Connect the TIM1 break to the GND. To generate a break event, switch this
      pin level from 0V to 3.3V.
      - TIM1_BKIN  PD5: connected to pin 25 of CN15 connector

  - **EWARM** : To monitor a variable in the live watch window, you must proceed as follow :
    - Start a debugging session.
    - Open the View > Images.
    - Double-click to deselect the second instance of project.out. 

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
