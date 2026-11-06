#!/bin/bash -

# Getting the CubeProgammer_CLI path
source ../env.sh

hconf1_otp=124
bootrom_config_9_otp=18

hconf1_value=0x100000
bootrom_config_9_value=0x1FF

connect_no_reset="-c port=SWD ap=1"

"$stm32programmercli" $connect_no_reset -el "$stm32ExtOTPInterace" -otp write word=$hconf1_otp value=$hconf1_value word=$bootrom_config_9_otp value=$bootrom_config_9_value
if [ $? -ne 0 ]; then
	echo "Error"
	exit 1
fi
echo "Secure boot has been activated"
exit 0
