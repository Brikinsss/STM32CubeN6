
## <b>Tx_Thread_Sync Application Description</b>

This application provides an example of Azure RTOS ThreadX stack usage. It shows how to develop an application using the ThreadX thread management APIs.
The main entry function tx_application_define() is called by ThreadX during kernel start, at this stage, the application creates 2 threads with the same priorities :

  - 'ThreadOne' (Priority : 10; Preemption Threshold : 10)
  - 'ThreadTwo' (Priority : 10; Preemption Threshold : 10)

The function "Led_Toggle()" is the entry function for both threads to toggle the leds.Therefore it is considered as a "critical section" that needs protection with a 'SyncObject' flag in the file "app_threadx.h"
Each thread is running in an infinite loop as following:

- 'ThreadOne':
  + try to acquire the 'SyncObject' immediately.
  + On Success toggle the 'LED_GREEN' each 500ms for 5 seconds.
  + Release the 'SyncObject'
  + Sleep for 10ms.
  + repeat the steps above

- 'ThreadTwo':
  + try to acquire the 'SyncObject' immediately.
  + On Success toggle the 'LED_RED' each 500ms for 5 seconds.
  + Release the 'SyncObject'
  + Sleep for 10ms.
  + Repeat the steps above.

By default the 'SyncObject' is defined as  "TX_MUTEX" .It is possible to use a binany "TX_SEMAPHORE" by tuning
the compile flags in the file "app_threadx.h".

####  <b>Expected success behavior</b>

  - 'LED_GREEN' toggles every 500ms for 5 seconds
  - 'LED_RED' toggles every 500ms for 5 seconds
  - Messages on HyperTerminal :
    + "** ThreadXXX : waiting for SyncObject !! **" : When thread is waiting for the SyncObject.
    + "** ThreadXXX : SyncObject released **" : When thread put the SyncObject.
    + "** ThreadXXX : SyncObject acquired **" : When thread get the SyncObject.

#### <b>Error behaviors</b>

'LED_RED' toggles every 1 second if any error occurs.
Error message is printed on UART.

#### <b>Assumptions if any</b>
None

#### <b>Known limitations</b>
None

#### <b>ThreadX usage hints</b>

 - ThreadX uses the Systick as time base, thus it is mandatory that the HAL uses a separate time base through the TIM IPs.
 - ThreadX is configured with 100 ticks/sec by default, this should be taken into account when using delays or timeouts at application. It is always possible to reconfigure it, by updating the "TX_TIMER_TICKS_PER_SECOND" define in the "tx_user.h" file. The update should be reflected in "tx_initialize_low_level.S" file too.
 - TrustZone is enabled by default on STM32N6 devices. When running ThreadX in the Secure world, the `TX_SINGLE_MODE_SECURE` flag must be added to the compiler/assembler's defined preprocessor symbols.
 - ThreadX is disabling all interrupts during kernel start-up to avoid any unexpected behavior, therefore all system related calls (HAL, BSP) should be done either at the beginning of the application or inside the thread entry functions.
 - ThreadX offers the "tx_application_define()" function, which is automatically called by the tx_kernel_enter() API.
   It is highly recommended to use it to create all the application's ThreadX related resources (threads, semaphores, memory pools...), but it should not in any way contain a system API call (HAL or BSP).
 - Using dynamic memory allocation requires to apply some changes to the linker file.
   ThreadX needs to pass a pointer to the first free memory location in RAM to the tx_application_define() function,
   using the "first_unused_memory" argument.
   This requires changes in the linker files to expose this memory location.
    + For EWARM add the following section into the .icf file:
     ```
     place in RAM_region    { last section FREE_MEM };
     ```
    + For MDK-ARM:
    ```
    either define the RW_IRAM1 region in the ".sct" file
    or modify the line below in "tx_initialize_low_level.S to match the memory region being used
        LDR r1, =|Image$$RW_IRAM1$$ZI$$Limit|
    ```
    + For STM32CubeIDE add the following section into the .ld file:
    ```
    ._threadx_heap :
      {
         . = ALIGN(8);
         __RAM_segment_used_end__ = .;
         . = . + 64K;
         . = ALIGN(8);
       } >RAM AT> RAM
    ```

    The simplest way to provide memory for ThreadX is to define a new section, see ._threadx_heap above.
    In the example above the ThreadX heap size is set to 64KBytes.
    The ._threadx_heap must be located between the .bss and the ._user_heap_stack sections in the linker script.
    Caution: Make sure that ThreadX does not need more than the provided heap memory (64KBytes in this example).
    Read more in STM32CubeIDE User Guide, chapter: "Linker script".

    + The "tx_initialize_low_level.S" should be also modified to enable the "USE_DYNAMIC_MEMORY_ALLOCATION" flag.

### <b>Keywords</b>

RTOS, ThreadX, Threading, Semaphore, Mutex


### <b>Hardware and Software environment</b>

  - This application runs on STM32N657X0H3QU devices
  - This application has been tested with STMicroelectronics NUCLEO-N657X0 boards revision MB1940-N657X0-C01 and can be easily tailored to any other supported device and development board.
  - This application uses USART1 to display output, the hyperterminal configuration is as follows:
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - Stop Bit = 1
    - Parity = none
    - Flow control = None

  - **EWARM** : To monitor a variable in the live watch window, you must proceed as follow :
    - Start a debugging session.
    - Open the View > Images.
    - Double-click to deselect the second instance of project.out.

###  <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Set the boot mode in development mode (BOOT1 switch position is 2-3, BOOT0 switch position doesn't matter).
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory. Code can be executed in this mode for debugging purposes.

Next, this program can be run in boot from flash mode. This can be done by following the instructions below:

 - Resort to CubeProgrammer to add a header to the generated binary Tx_Thread_Sync.bin with the following command
   - *STM32_SigningTool_CLI.exe -bin Tx_Thread_Sync.bin -nk -of 0x80000000 -t fsbl -o Tx_Thread_Sync-trusted.bin -hv 2.3 -dump Tx_Thread_Sync-trusted.bin*
   - The resulting binary is Tx_Thread_Sync-trusted.bin.
 - Next, in resorting again to CubeProgrammer, load the binary and its header (Tx_Thread_Sync-trusted.bin) in Nucleo board external Flash at address 0x7000'0000.
 - Set the boot mode in boot from external Flash (BOOT0 switch position is 1-2 and BOOT1 switch position is 1-2).
 - Press the reset button. The code then executes in boot from external Flash mode.


**Warning** If using CubeProgrammer v2.21 version or more recent, add *-align* option in the command line.
