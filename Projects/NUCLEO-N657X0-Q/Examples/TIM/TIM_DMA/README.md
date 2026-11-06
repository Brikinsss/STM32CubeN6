## <b>TIM_DMA Example Description</b>

Use of the DMA with TIMER Update request to transfer data from memory to TIMER Capture Compare Register 3 (TIMx_CCR3).

The system clock runs at 600 MHz and external memory interface at the highest speed.

  The following configuration values are used in this example:

    - TIM1CLK = SystemCoreClock
    - Counter repetition = 3 
    - Prescaler = 0 
    - TIM1 counter clock = SystemCoreClock
    - SystemCoreClock is set to 600 MHz for STM32N6xx

  The objective is to configure TIM1 channel 3 to generate complementary PWM 
  (Pulse Width Modulation) signal with a frequency equal to 17.57 KHz, and a variable 
  duty cycle that is changed by the DMA after a specific number of Update DMA request.

  The number of this repetitive requests is defined by the TIM1 Repetition counter,
  each 4 Update Requests, the TIM1 Channel 3 Duty Cycle changes to the next new 
  value defined by the aCCValue_Buffer.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32N657X0HxQ Devices :
The CPU at 600 MHz  

  The PWM waveform can be displayed using an oscilloscope.
 
  In nominal mode (except at start) , it should looks like this :
    
       .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   
        ___________     _______         ___             ___________    __
      _|           |___|       |_______|   |___________|           |__|             
       <----57us -----><----57us -----><----57us -----><----57us ----->


#### <b>Notes</b>  
 1. PWM signal frequency value mentioned above is theoretical (obtained when the system clock frequency 
      is exactly 600 MHz). Since the generated system clock frequency may vary from one board to another observed
      PWM signal frequency might be slightly different.

 2. Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in SysTick ISR. This
      implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower)

 3. This example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
  

### <b>Keywords</b>

Timer, DMA, PWM, Frequency, Duty Cycle, Waveform, Oscilloscope, Output, Signal

### <b>Directory contents</b>

  - TIM/TIM_DMA/FSBL/Inc/stm32n6xx_nucleo_conf.h     BSP configuration file
  - TIM/TIM_DMA/FSBL/Inc/stm32n6xx_hal_conf.h        HAL configuration file
  - TIM/TIM_DMA/FSBL/Inc/stm32n6xx_it.h              Interrupt handlers header file
  - TIM/TIM_DMA/FSBL/Inc/main.h                      Header for main.c module  
  - TIM/TIM_DMA/FSBL/Src/stm32n6xx_it.c              Interrupt handlers
  - TIM/TIM_DMA/FSBL/Src/main.c                      Main program
  - TIM/TIM_DMA/FSBL/Src/stm32n6xx_hal_msp.c         HAL MSP file
  - TIM/TIM_DMA/FSBL/Src/system_stm32n6xx.c          STM32N6xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.
  - In this example, the clock is set to 600 MHz.
    
  - This example has been tested with STMicroelectronics NUCLEO-N657X0-Q 
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-N657X0-Q Set-up
    - Connect the TIM1 pin to an oscilloscope to monitor the different waveforms: 
    - TIM1 CH3 (PF.6 (pin 32 in CN3 connector))

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
