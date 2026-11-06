## <b>OEMuROT_Boot application description</b>

This project provides an OEMuROT boot example.

OEMuRoT stands for OEM updatable Root of Trust and acts as a second boot stage right after immutable BOOTROM code.
OEMuRoT is an example based on the MCUboot open-source software provided with STM32N6Cube. OEMuRoT offers two services:

- The Secure Boot (root of trust service) is an updatable code, which is always executed after BOOTROM code.
It activates runtime protections, and then verifies the authenticity and integrity of the user application code
before every execution.
- The Secure Firmware Update application is an updatable code that detects new firmware image candidate.
It checks its version (version downgrade prevention), authenticity, and integrity before installing it after decryption.

To ease the development process, a prebuild command and postbuild command are integrated in the project.
The prebuild command is in charge of preparing the OEMuROT_Boot scatter file according to flash layout description.
The postbuild command is in charge of preparing the provisioning scripts and images configuration files in
ROT_Provisioning/OEMuROT folder, according to the flash layout and OEMuROT configuration.

The maximum system clock frequency 600Mhz is configured.

### <b>Keywords</b>

OEMuROT, TrustZone, Root Of Trust, Security, mcuboot

### <b>Directory contents</b>

File | Description |
 --- | --- |
  OEMuROT_Boot/Inc/mcuboot_config/mcuboot_config.h  | Mcuboot configuration file
  OEMuROT_Boot/Inc/boot_hal_cfg.h                   | Platform configuration file for OEMuROT_Boot
  OEMuROT_Boot/Inc/boot_hal_flowcontrol.h           | Header file for flow control code in boot_hal.c
  OEMuROT_Boot/Inc/boot_hal_hash_ref.h              | Header file for hash reference code in boot_hal.c
  OEMuROT_Boot/Inc/boot_hal_imagevalid.h            | Header file for image validation code in boot_hal.c
  OEMuROT_Boot/Inc/boot_hal_mce.h                   | Header file for boot_hal_mce.c
  OEMuROT_Boot/Inc/cmsis.h                          | Header file for CMSIS
  OEMuROT_Boot/Inc/debug_authentication.h           | Header file for debug_authentication.c
  OEMuROT_Boot/Inc/flash_layout.h                   | Flash mapping
  OEMuROT_Boot/Inc/low_level_ext_flash.h            | Header file for low_level_ext_flash.c
  OEMuROT_Boot/Inc/low_level_ext_ram.h              | Header file for low_level_ext_ram.c
  OEMuROT_Boot/Inc/low_level_mce.h                  | Header file for low_level_mce.c
  OEMuROT_Boot/Inc/low_level_otp.h                  | Header file for low_level_otp.c
  OEMuROT_Boot/Inc/low_level_rng.h                  | Header file for low_level_rng.c
  OEMuROT_Boot/Inc/low_level_security.h             | Header file for low_level_security.c
  OEMuROT_Boot/Inc/mbedtls_alt_config.h             | Config file for mbedtls_alt middleware
  OEMuROT_Boot/Inc/mbedtls_config.h                 | Minimal configuration for using TLS in the bootloader
  OEMuROT_Boot/Inc/region_defs.h                    | RAM and FLASH regions definitions
  OEMuROT_Boot/Inc/sha256_alt.h                     | Header file for sha256_alt.c
  OEMuROT_Boot/Inc/stm32_extmem_conf.h              | Header configuration for extmem module
  OEMuROT_Boot/Inc/stm32n6xx_hal_conf.h             | HAL driver configuration file
  OEMuROT_Boot/Inc/target_cfg.h                     | Header file for target start up
  OEMuROT_Boot/Src/bl2_nv_services.c                | Non Volatile services for OEMuROT_Boot
  OEMuROT_Boot/Src/boot_hal.c                       | Platform initialization
  OEMuROT_Boot/Src/debug_authentication.c           | Debug authentication module
  OEMuROT_Boot/Src/image_macros_to_preprocess_bl2.c | Images definitions to preprocess for bl2
  OEMuROT_Boot/Src/keys_map.c                       | Keys interface
  OEMuROT_Boot/Src/low_level_com.c                  | UART low level interface
  OEMuROT_Boot/Src/low_level_device.c               | Flash low level device configuration
  OEMuROT_Boot/Src/low_level_ext_flash.c            | External flash low level interface
  OEMuROT_Boot/Inc/low_level_ext_ram.c              | External RAM low level interface
  OEMuROT_Boot/Inc/low_level_extmem_device.c        | External memory module interface
  OEMuROT_Boot/Inc/low_level_mce.c                  | MCE interface
  OEMuROT_Boot/Inc/low_level_otp.c                  | OTP interface
  OEMuROT_Boot/Src/low_level_rng.c                  | Random generator interface
  OEMuROT_Boot/Src/low_level_security.c             | Security low level services
  OEMuROT_Boot/Src/sha256_alt.c                     | Implementation of mbedtls_alt SHA256 module
  OEMuROT_Boot/Src/stm32n6xx_hal_msp.c              | HAL MSP module
  OEMuROT_Boot/Src/system_stm32n6xx.c               | System Init file
  OEMuROT_Boot/Src/tick.c                           | HAL Tick implementation

### <b>Hardware and Software environment</b>

- This example runs on STM32N657xx devices.
- This example has been tested with STMicroelectronics STM32N6570-DK (MB1939) board and can be easily tailored
to any other supported device and development board.
- To get debug print in your UART console you have to configure it using these parameters:
Speed: 115200, Data: 8bits, Parity: None, stop bits: 1, Flow control: none.

### <b>How to use it ?</b>

<u>Before compiling the project, you should first start the provisioning process</u>.

Before starting the provisioning process, select the application project to use (application example or template),
through oemurot_boot_path_project variable in ROT_Provisioning/env.bat or env.sh.
Then start provisioning process by running provisioning.bat (.sh) from ROT_Provisioning/OEMuROT, and follow its instructions.
Refer to ROT_Provisioning/OEMuROT readme for more information on the provisioning process.

Refer to OEMuROT_Appli readme for example of application booted through OEMuROT boot path.

For more details, refer to STM32N6 Wiki articles:

  - [OEMuRoT for STM32N6](https://wiki.st.com/stm32mcu/wiki/Security:OEMuRoT_for_STM32N6).
  - [How to start with OEMuRoT on STM32N6 MCUs](https://wiki.st.com/stm32mcu/wiki/Security:How_to_start_with_OEMuRoT_on_STM32N6_MCUs).

#### <b>Notes:</b>

By default the anti-tamper is enabled for internal tamper events. It is possible to change this configuration with
OEMUROT_TAMPER_ENABLE defined in Inc\boot_hal_cfg.h.

```
#define NO_TAMPER            (0)                   /*!< No tamper activated */
#define INTERNAL_TAMPER_ONLY (1)                   /*!< Only Internal tamper activated */
#define OEMUROT_TAMPER_ENABLE INTERNAL_TAMPER_ONLY /*!< TAMPER configuration flag  */
```






