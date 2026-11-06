## <b>ROT_Provisioning OEMuROT description</b>

This section provides an overview of the available scripts for OEMuROT.

OEMuROT stands for "OEM Updatable Root of Trust" and it provides two services:

  - Secure Boot: Verification of the integrity, and authenticity of the application code before any execution.
  - Secure Firmware Update: If a new firmware image is available on detection, check of its authenticity.
                            Once the firmware is decrypted, check of the integrity of the code before installing it.

### <b>Keywords</b>

OEMuROT, Root Of Trust, Security, mcuboot

### <b>Directory contents</b>

<b>Sub-directories</b>

- OEMuROT/Binary                               Output binaries and generated images.
- OEMuROT/Config                               OEMuROT configuration files.
- OEMuROT/Images                               Images configuration files.
- OEMuROT/Keys                                 Keys for firmware image authentication and encryption.

<b>Scripts</b>

- OEMuROT/da_programming.bat/.sh               Program DA password hash into OTP.
- OEMuROT/flash_programming.bat/.sh            Programs images on the device.
- OEMuROT/keys_generation.bat/.sh              Generate OEMuROT new keys.
- OEMuROT/keys_programming.bat/.sh             Programs OEMuROT keys into OTP.
- OEMuROT/provisioning.bat/.sh                 Performs device provisioning process.

### <b>Hardware and Software environment</b>

  - This example runs on STM32N657xx devices.

  - This example has been tested with STMicroelectronics STM32N6570-DK (MB1939) board and can be easily tailored for
    STM32N6xx devices by following these steps
    (more details in the wiki article [<b>How to create ROT examples for STM32N6</b>](https://wiki.st.com/stm32mcu/wiki/Security:How_to_create_ROT_examples_for_STM32N6)).



### <b>How to use it ?</b>

The .bat scripts are designed for Windows, whereas the .sh scripts are designed for Linux and Mac-OS.

During the **provisioning process**, the programming scripts and the application files will
be automatically updated according to OEMuRoT configuration.

The **provisioning process** (OEMuROT/provisioning.bat/.sh) is divided into 3 majors steps:

- Step 0: Preliminary stage
- Step 1: Configuration management
- Step 2: Image generations
- Step 3: Provisioning

The provisioning script is relying on flash_programming script.

In the /Config/ folder, two password files are provided:

- **DA_password.bin**: This file contains only the password and is used to program the password hash in OTPs.
- **password.bin**: This file contains the password preceded by a header and is intended for use with
STM32CubeProgrammer to enable non-secure debug.

For more details, refer to STM32N6 Wiki articles:

  - [OEMuRoT for STM32N6](https://wiki.st.com/stm32mcu/wiki/Security:OEMuRoT_for_STM32N6)
  - [How to start with OEMuRoT on STM32N6 MCUs](https://wiki.st.com/stm32mcu/wiki/Security:How_to_start_with_OEMuRoT_on_STM32N6_MCUs)

### <b>Notes</b>

**AppliCfg.py** (located in Utilities/PC_Software/ROT_AppliConfig) is used during provisioning
process. It is needed to have python and some python packages installed in your
environment. Refer to Utilities/PC_Software/ROT_AppliConfig/README.md for more details.