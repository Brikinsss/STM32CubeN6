## <b>ADC_AnalogWatchdog Example Description</b>

How to use an ADC peripheral with an ADC analog watchdog to monitor a channel
and detect when the corresponding conversion data is outside the window
thresholds.
This example is based on the STM32N6xx ADC HAL API.

Example configuration:

ADC is configured to convert a single channel, in continuous conversion mode,
from SW trigger.
Analog watchdog 1 is configured to monitor all channels on ADC group regular
(therefore, including the selected channel).

Analog watchdog thresholds values:

- high threshold is set to Vref+/2 (1.8V/2=0.9V on NUCLEO-N657X0-Q), refer to literal ADC_AWD_THRESHOLD_HIGH.
- low threshold is set to 0V, refer to literal ADC_AWD_THRESHOLD_LOW.
ADC interruption enabled: Analog watchdog 1.

Example execution:

From the main program execution, the ADC converts the selected channel continuously.
When conversion data is out of analog watchdog window, ADC interruption occurs
and analog watchdog callback function is called to process the event.
Every 5 seconds, the ADC analog watchdog is rearmed to be ready for another trig.

LED1 is used to monitor program execution status:

- Normal operation: Voltage of ADC channel monitored versus analog watchdog window
  can be observed with LED toggle frequency:
  
  - Voltage within analog watchdog window: toggle frequency slow (2Hz)
  - Voltage below analog watchdog window: toggle frequency fast (10Hz)
  - Voltage above analog watchdog window: toggle frequency very fast (20Hz)
- Error: LED remaining turned on

Note: In this example, analog watchdog low threshold is set to 0V
      for test simplicity purpose, therefore case of voltage below low threshold
      is not achievable unless cutomizing low threshold value.

Note: In case of noise on voltage applied on ADC channel input (due to connectors and wires parasitics),
      ADC analog watchdog may trig at a voltage level with an uncertainty of tens of mV.

Debug: variables to monitor with debugger:

- "ubAnalogWatchdog1Status": analog watchdog state

Connection needed:

Use an external power supply to generate a voltage in range [0V; 1.8V]
and connect it to analog input pin (cf pin below).

Note: If no power supply available, this pin can be let floating (in this case
      ADC conversion data will be undetermined).

Other peripherals used:

  1 GPIO for LED1

  1 GPIO for analog input: ADC channel 1 on pin PA.01 (Morpho connector CN15 pin 38)

### <b>Keywords</b>

ADC, analog digital converter, analog, conversion, voltage, channel, analog input, trigger, analog watchdog, threshold, window

### <b>Directory contents</b>

  - ADC/ADC_AnalogWatchdog/FSBL/Inc/stm32n6xx_nucleo_conf.h     BSP configuration file
  - ADC/ADC_AnalogWatchdog/FSBL/Inc/stm32n6xx_hal_conf.h        HAL configuration file
  - ADC/ADC_AnalogWatchdog/FSBL/Inc/stm32n6xx_it.h              Interrupt handlers header file
  - ADC/ADC_AnalogWatchdog/FSBL/Inc/main.h                      Header for main.c module
  - ADC/ADC_AnalogWatchdog/FSBL/Src/stm32n6xx_it.c              Interrupt handlers
  - ADC/ADC_AnalogWatchdog/FSBL/Src/stm32n6xx_hal_msp.c         HAL MSP module
  - ADC/ADC_AnalogWatchdog/FSBL/Src/main.c                      Main program
  - ADC/ADC_AnalogWatchdog/FSBL/Src/system_stm32n6xx.c          STM32N6xx system source file


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
