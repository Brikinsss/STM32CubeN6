## <b>BootROM related scripts description</b>

This section provides the necessary scripts to generate and program BootROM keys.

To ease the configuration process, enable_secure_boot, keys_generation, and keys_programming scripts are used.

### <b>Keywords</b>

BootROM, Root Of Trust, Security

### <b>Directory contents</b>

File | Description
 --- | ---
  BootROM/enable_secure_boot.bat/.sh    | Change device state to CLOSED_UNLOCKED.
  BootROM/keys_generation.bat/.sh       | Generate new BootROM keys.
  BootROM/keys_programming.bat/.sh      | Program BootROM keys into OTP.
  OEMuROT/Keys/OEM_SECRET.bat/.sh       | Encryption key.
  OEMuROT/Keys/privateKey00.pem         | Private authentication key 0.
  OEMuROT/Keys/privateKey01.pem         | Private authentication key 1.
  OEMuROT/Keys/privateKey02.pem         | Private authentication key 2.
  OEMuROT/Keys/privateKey03.pem         | Private authentication key 3.
  OEMuROT/Keys/privateKey04.pem         | Private authentication key 4.
  OEMuROT/Keys/privateKey05.pem         | Private authentication key 5.
  OEMuROT/Keys/privateKey06.pem         | Private authentication key 6.
  OEMuROT/Keys/privateKey07.pem         | Private authentication key 7.
  OEMuROT/Keys/publicKey00.pem          | Public authentication key 0.
  OEMuROT/Keys/publicKey01.pem          | Public authentication key 1.
  OEMuROT/Keys/publicKey02.pem          | Public authentication key 2.
  OEMuROT/Keys/publicKey03.pem          | Public authentication key 3.
  OEMuROT/Keys/publicKey04.pem          | Public authentication key 4.
  OEMuROT/Keys/publicKey05.pem          | Public authentication key 5.
  OEMuROT/Keys/publicKey06.pem          | Public authentication key 6.
  OEMuROT/Keys/publicKey07.pem          | Public authentication key 7.
  OEMuROT/Keys/publicKeyHash00.pem      | Public authentication key hash 0.
  OEMuROT/Keys/publicKeyHash01.pem      | Public authentication key hash 1.
  OEMuROT/Keys/publicKeyHash02.pem      | Public authentication key hash 2.
  OEMuROT/Keys/publicKeyHash03.pem      | Public authentication key hash 3.
  OEMuROT/Keys/publicKeyHash04.pem      | Public authentication key hash 4.
  OEMuROT/Keys/publicKeyHash05.pem      | Public authentication key hash 5.
  OEMuROT/Keys/publicKeyHash06.pem      | Public authentication key hash 6.
  OEMuROT/Keys/publicKeyHash07.pem      | Public authentication key hash 7.
  OEMuROT/Keys/publicKeysHashHashes.pem | The hash of authentication keys hashes.

### <b>Hardware and Software environment</b>

  - This example runs on STM32N657xx devices.

  - This example has been tested with STMicroelectronics STM32N6570-DK (MB1939) board and can be easily tailored for
    STM32N6xx devices by following these steps
    (more details in the wiki article [<b>How to create ROT examples for STM32N6</b>](https://wiki.st.com/stm32mcu/wiki/Security:How_to_create_ROT_examples_for_STM32N6)).

### <b>How to use it ?</b>

The keys_generation script generate BOOTROM new keys in the folder \Keys\.

The keys_programming script program necessary keys that allows BootROM to run signed/encrypted images.

The enable_secure_boot script allows the switching of the device to CLOSED_LOCKED state. After this step the BootROM only accepts signed images.

For more details, refer to STM32N6 Wiki article:

  - [How to install a trusted application on STM32N6 MCUs](https://wiki.st.com/stm32mcu/wiki/Security:How_to_install_a_trusted_application_on_STM32N6_MCUs).



