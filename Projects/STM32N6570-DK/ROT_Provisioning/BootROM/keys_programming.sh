#!/bin/bash -

# Getting the CubeProgammer_CLI path
source ../env.sh

hashtable_otp=160
oem_secret_otp=364
rma_password_otp=256

hashtable="Keys/publicKeysHashHashes.bin"
oem_secret="Keys/OEM_SECRET.bin"
rma_password="Keys/RMA_password.bin"

connect_no_reset="-c port=SWD ap=1"

error()
{
  echo "Error when trying to $action"
  echo "keys generation aborted"
  echo
  $SHELL
}

# Write key in OTP could be done only once
action="Write public key hash table in OTP"
echo $action", could be done only once"
"$stm32programmercli" $connect_no_reset -el "$stm32ExtOTPInterace" -otp fwrite lock "$hashtable" word=$hashtable_otp
if [ $? -ne 0 ]; then
	error
fi

action="Write OEM secret key in OTP"
echo $action", could be done only once"
"$stm32programmercli" $connect_no_reset -el "$stm32ExtOTPInterace" -otp fwrite lock "$oem_secret" word=$oem_secret_otp
if [ $? -ne 0 ]; then
	error
fi

action="Write RMA password in OTP"
echo $action", could be done only once"
"$stm32programmercli" $connect_no_reset -el "$stm32ExtOTPInterace" -otp fwrite lock "$rma_password" word=$rma_password_otp
if [ $? -ne 0 ]; then
	error
fi

echo "Keys programmed successfully"
$SHELL