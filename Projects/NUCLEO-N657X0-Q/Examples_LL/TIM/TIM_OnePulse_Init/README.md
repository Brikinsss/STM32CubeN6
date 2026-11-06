## <b>TIM_OnePulse_Init Example Description</b>

Configuration of a timer to generate a positive pulse in
Output Compare mode with a length of tPULSE and after a delay of tDELAY. This example
is based on the STM32N6xx TIM LL API. The peripheral initialization uses
LL initialization function to demonstrate LL Init.

The pulse is generated on OC1.

This example uses 2 timer instances:

  - TIM1 generates a positive pulse of 50 us after a delay of 50 us. User push-button
    is used to start TIM1 counter.
                                              ___
                                             |   |
    User push-button ________________________|   |________________________________
                                                            ___________
                                                           |           |
    OC1              ______________________________________|           |________
    (TIM1_CH1)                                <---50 us---><---50 us--->
                                                |            |_ uwMeasuredPulseLength
                                                |_ uwMeasuredDelay

  TIM1_CH1 delay and pulse length are measured every time a pulse is generated.
  Both can be observed through the debugger by monitoring the variables uwMeasuredDelay and
  uwMeasuredPulseLength respectively.

  - TIM4 generates a positive pulse of 3 s after a delay of 2 s. TIM4 counter start
    is controlled through the slave mode controller. TI2FP2 signals is selected as
    trigger input meaning that TIM2 counter starts when a rising edge is detected on
    TI2FP2.
                                  ___
                                 |   |
    TI2 _________________________|   |_________________________________________
    (TIM4_CH2)
                                               ___________________________
                                              |                           |
    OC1 ______________________________________|                           |____
    (TIM4_CH1)                   <-----2s-----><----------3 s------------->



Both TIM1 and TIM4 are configured to generate a single pulse (timer counter
stops automatically at the next update event (UEV).

Connecting TIM1 OC1 to TIM4 TI2 allows to trigger TIM4 counter by pressing
the User push-button.

### <b>Keywords</b>

Timers, Output, signals, One Pulse, PWM, Oscilloscope, External signal, Autoreload, Waveform

### <b>Directory contents</b>

    - TIM/TIM_OnePulse_Init/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
    - TIM/TIM_OnePulse_Init/FSBL/Inc/main.h                  Header for main.c module
    - TIM/TIM_OnePulse_Init/FSBL/Inc/stm32_assert.h          Template file to include assert_failed function
    - TIM/TIM_OnePulse_Init/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
    - TIM/TIM_OnePulse_Init/FSBL/Src/main.c                  Main program
    - TIM/TIM_OnePulse_Init/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with NUCLEO-N657X0-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-N657X0-Q Set-up:
      - TIM1_CH1  PE.09: connected to pin 31 in CN15 connector
      - TIM4_CH1  PE.05: connected to pin 4 of CN15 connector
      - TIM4_CH2  PE.06: connected to pin 2 of CN15 connector
	  - USER push-button PC.13: pin 23 in CN3 connector

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
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Project-trusted.bin) in NUCLEO-N657X0-Q board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.
 - Run the example



**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
