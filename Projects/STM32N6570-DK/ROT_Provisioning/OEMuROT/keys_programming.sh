#!/bin/bash -

# Getting the CubeProgammer_CLI path
source ../env.sh

s_auth_public_key_otp=268
ns_auth_public_key_otp=276
enc_private_key_otp=284

s_auth_public_key_bin=Keys/OEMuROT_Authentication_S_Pub_Hash.bin
ns_auth_public_key_bin=Keys/OEMuROT_Authentication_NS_Pub_Hash.bin
enc_private_key_bin=Keys/OEMuROT_Encryption_Priv.bin

connect_reset="-c port=SWD ap=1"

error()
{
  echo "Error when trying to $action"
  echo "keys generation aborted"
  echo
  $SHELL
}

# Write key in OTP could be done only once
action="Write Secure public key in OTP"
echo $action", could be done only once"
"$stm32programmercli" $connect_reset -el "$stm32ExtOTPInterace" -otp fwrite lock "$s_auth_public_key_bin" word=$s_auth_public_key_otp
if [ $? -ne 0 ]; then
	error
fi

action="Write Non Secure public key in OTP"
echo $action", could be done only once"
"$stm32programmercli" $connect_reset -el "$stm32ExtOTPInterace" -otp fwrite lock "$ns_auth_public_key_bin" word=$ns_auth_public_key_otp
if [ $? -ne 0 ]; then
	error
fi

action="Write encryption private key in OTP"
echo $action", could be done only once"
"$stm32programmercli" $connect_reset -el "$stm32ExtOTPInterace" -otp fwrite lock "$enc_private_key_bin" word=$enc_private_key_otp
if [ $? -ne 0 ]; then
	error
fi

echo "Keys programmed successfully"
$SHELL