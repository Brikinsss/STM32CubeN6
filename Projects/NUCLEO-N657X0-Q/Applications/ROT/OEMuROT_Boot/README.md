## <b>OEMuROT_Boot application description</b>

This project provides an OEMuROT boot example.

OEMuRoT stands for OEM updatable Root of Trust and acts as a second boot stage right after immutable BOOTROM code.
OEMuRoT is an example based on the MCUboot open-source software provided with STM32N6Cube. OEMuRoT offers two services:

- The Secure Boot (root of trust service) is an updatable code, which is always executed after BOOTROM code.
It activates runtime protections, and then verifies the authenticity and integrity of the user application code
before every execution.
- The Secure Firmware Update application is an updatable code that detects new firmware image candidate.
It checks its version (version downgrade prevention), authenticity, and integrity before installing it after decryption.

Please follow the article [<b>How to create ROT examples for STM32N6</b>](https://wiki.st.com/stm32mcu/wiki/Security:How_to_create_ROT_examples_for_STM32N6)
to generate this example based on `Firmware/Projects/STM32N6570-DK/Applications/ROT/OEMuROT_Boot` project.






