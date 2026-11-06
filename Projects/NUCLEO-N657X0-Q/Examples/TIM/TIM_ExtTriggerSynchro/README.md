## <b>TIM_ExtTriggerSynchro Example Description</b>

 This example shows how to synchronize TIM peripherals in cascade mode with an external trigger.
 In this example three timers are used:

This project runs from internal RAM. The system clock is set at 600 MHz.
 
  1/TIM1 is configured as Master Timer:

      - Toggle Mode is used
      - The TIM1 Enable event is used as Trigger Output

  2/TIM1 is configured as Slave Timer for an external Trigger connected to TIM1
    TI2 pin (TIM1 CH2 configured as input pin):

      - The TIM1 TI2FP2 is used as Trigger Input
      - Rising edge is used to start and stop the TIM1: Gated Mode.

  3/TIM2 is slave for TIM1 and Master for TIM3,

      - Toggle Mode is used
      - The ITR0 (TIM1) is used as input trigger 
      - Gated mode is used, so start and stop of slave counter
        are controlled by the Master trigger output signal(TIM1 enable event).
      - The TIM2 enable event is used as Trigger Output. 

  4/TIM3 is slave for TIM2,

      - Toggle Mode is used
      - The ITR1 (TIM2) is used as input trigger
      - Gated mode is used, so start and stop of slave counter
        are controlled by the Master trigger output signal(TIM2 enable event).

   The TIM1CLK is set to 400000000, 
   TIM2CLK is set to 400000000 and 
   TIM3CLK frequency is set to 400000000,
   To get TIMx counter clock at 8 MHz the Prescaler is computed as following:

   - Prescaler = (TIMx CLK / TIMx counter clock) - 1

   SystemCoreClock is set to 600 MHz.

   TIMx frequency = TIMx  counter clock/ 2*(TIMx_Period + 1) = 100 KHz.
   
  The starts and stops of the TIM1 counters are controlled by the external trigger.

  The TIM2 starts and stops are controlled by the TIM1, and the TIM3 starts and
  stops are controlled by the TIM2.


#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate
    delay (in milliseconds) based on variable incremented in SysTick ISR. This
    implies that if HAL_Delay() is called from a peripheral ISR process, then 
    the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. This example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Timer, PWM, External Trigger, Synchronization, Cascade mode, Master, Slave, Duty Cycle, Waveform,
Oscilloscope, Output, Signal,

### <b>Directory contents</b>

  - TIM/TIM_ExtTriggerSynchro/FSBL/Inc/stm32n6xx_nucleo_conf.h     BSP configuration file
  - TIM/TIM_ExtTriggerSynchro/FSBL/Inc/stm32n6xx_hal_conf.h        HAL configuration file
  - TIM/TIM_ExtTriggerSynchro/FSBL/Inc/stm32n6xx_it.h              Interrupt handlers header file
  - TIM/TIM_ExtTriggerSynchro/FSBL/Inc/main.h                      Header for main.c module  
  - TIM/TIM_ExtTriggerSynchro/FSBL/Src/stm32n6xx_it.c              Interrupt handlers
  - TIM/TIM_ExtTriggerSynchro/FSBL/Src/main.c                      Main program
  - TIM/TIM_ExtTriggerSynchro/FSBL/Src/stm32n6xx_hal_msp.c         HAL MSP file
  - TIM/TIM_ExtTriggerSynchro/FSBL/Src/system_stm32n6xx.c          STM32N6xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-N657X0-Q 
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-N657X0-Q Set-up
    - Connect an external trigger to the TIM1 CH2 PE.11 (pin 23 in CN15 connector).
    - Connect the following pins to an oscilloscope to monitor the different waveforms:
      - TIM1 CH1 PE.09 (pin 31 in CN15  connector)
      - TIM2 CH1 PA.02 (pin 34 in CN3 connector)
      - TIM3 CH1 PC.06 (pin 28 in CN2 connector)

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
