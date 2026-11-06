## <b>PWR_STOP_RTC Example Description</b>

How to enter the Stop mode and wake-up from this mode by using an external
reset or the RTC wakeup timer.

This event is connected to EXTI_Line17.


LED1 is ON during run mode and OFF during stop mode.
LED2 is ON when the configuration is failed (system will go to an infinite loop).


In the associated software
  - The Low Speed Internal (LSI) clock is used as RTC clock source by default.
  - The EXTI_Line17 connected internally to the RTC Wakeup event is configured
    to generate an interrupt on rising edge each 20s.
  - The SysTick is programmed to generate an interrupt each 1 ms.

LED1 is ON. The system enters Stop mode after 5s and will wait for the RTC Wakeup event to be
generated each 20s, LED1 is OFF. 

After the system woken up from Stop, the clock system is reconfigured because the default clock
after wake up is the HSI.

The above scenario is repeated in an infinite loop.

    - Stop Mode with RTC clocked by LSI
    ===================================
            - RTC Clocked by LSI
            - Regulator in LP mode
            - VREFINT OFF with fast wakeup enabled
            - HSI as SysClk after Wake Up
            - No IWDG
            - Automatic Wakeup using RTC on LSI (after ~20s)


Note: This example can not be used in DEBUG mode, this is due to the fact
      that the Cortex-M7 core is no longer clocked during low power mode
       so debugging features are disabled.

Note: Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
      the RTC clock source; in this case the Backup domain will be reset in
      order to modify the RTC Clock source, as consequence RTC registers (including
      the backup registers) and RCC_CSR register are set to their reset values.

#### <b>Notes</b>

   1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

   2. The example need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Power, PWR, Stop mode, Interrupt, EXTI, Wakeup, Low Power, RTC, External reset, LSI.

### <b>Hardware and Software environment</b>

  - PWR/PWR_STOP_RTC/FSBL/Inc/stm32n6xx_nucleo_conf.h     BSP configuration file
  - PWR/PWR_STOP_RTC/FSBL/Inc/stm32n6xx_conf.h            HAL Configuration file
  - PWR/PWR_STOP_RTC/FSBL/Inc/stm32n6xx_it.h              Header for stm32n6xx_it.c
  - PWR/PWR_STOP_RTC/FSBL/Inc/main.h                      Header file for main.c
  - PWR/PWR_STOP_RTC/FSBL/Src/system_stm32n6xx.c          STM32N6xx system clock configuration file
  - PWR/PWR_STOP_RTC/FSBL/Src/stm32n6xx_it.c              Interrupt handlers
  - PWR/PWR_STOP_RTC/FSBL/FSBL/Src/main.c                 Main program
  - PWR/PWR_STOP_RTC/FSBL/Src/stm32n6xx_hal_msp.c         HAL MSP module

### <b>Hardware and Software environment</b>

  - This example runs on STM32N657X0HxQ devices

  - This example has been tested with STMicroelectronics NUCLEO-N657X0-Q
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-N657X0-Q Set-up
    - Use LED1 and LED2 connected respectively to PG.08 and PG.10 pins

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
