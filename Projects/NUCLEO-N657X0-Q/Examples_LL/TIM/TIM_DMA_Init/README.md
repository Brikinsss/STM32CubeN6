## <b>TIM_DMA_Init Example Description</b>

Use of the DMA with a timer update request
to transfer data from memory to Timer Capture Compare Register 3 (TIMx_CCR3). This
example is based on the STM32N6xx TIM LL API. The peripheral initialization
uses LL unitary service functions for optimization purposes (performance and size).

The following configuration values are used in this example:

  - TIM1CLK = SystemClock/TIM_PRESCALER
  - Counter repetition = 3
  - Prescaler = 0
  - TIM1 counter clock = SystemClock/TIM_PRESCALER
  - CPU clock is set to 600 MHz for STM32N6xx Devices.

The objective is to configure TIM1 channel 3 to generate PWM edge aligned
signal with a frequency equal to 17.57 KHz, and a variable duty cycle that
is changed by the DMA after a specific number of Update DMA request.

The number of this repetitive requests is defined by the TIM1 Repetition counter,
each 4 Update Requests, the TIM1 Channel 3 Duty Cycle changes to the next new
value defined by the aCCValue buffer.

The PWM waveform can be displayed using an oscilloscope.

Whenever a DMA transfer fails LED1 flashes with a frequency of 1 Hz.

#### <b>Notes</b>

 1. PWM signal frequency value mentioned above is theoretical (obtained when
    the system clock frequency is exactly 400 MHz). Since the generated system
    clock frequency may vary from one board to another observed PWM signal
    frequency might be slightly different.

### <b>Keywords</b>

Timers, DMA, PWM, Frequency, Duty Cycle, Waveform, Oscilloscope, Output, Signal

### <b>Directory contents</b>

  - TIM/TIM_DMA_Init/FSBL/Inc/stm32n6xx_it.h          Interrupt handlers header file
  - TIM/TIM_DMA_Init/FSBL/Inc/main.h                  Header for main.c module
  - TIM/TIM_DMA_Init/FSBL/Inc/stm32_assert.h          Template file to include assert_failed function
  - TIM/TIM_DMA_Init/FSBL/Src/stm32n6xx_it.c          Interrupt handlers
  - TIM/TIM_DMA_Init/FSBL/Src/main.c                  Main program
  - TIM/TIM_DMA_Init/FSBL/Src/system_stm32n6xx.c      STM32N6xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.

  - This example has been tested with NUCLEO-N657X0-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-N657X0-Q Set-up
    - Connect the TIM1 pins to an oscilloscope to monitor the different waveforms:
      - TIM1_CH3 : : PF.06 (pin 32 in CN3 connector) 

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
