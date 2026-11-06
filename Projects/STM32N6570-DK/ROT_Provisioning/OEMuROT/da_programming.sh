#!/bin/bash -

# Getting the CubeProgammer_CLI path
source ../env.sh

# Absolute path to this script
if [ $# -ge 1 ] && [ -d $1 ]; then
    projectdir=$1
else
    projectdir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
fi

# Password binary
password_bin="$projectdir/Config/DA_password.bin"
hash_bin="$projectdir/hash.bin"
da_pw_hash_otp=292

connect_reset="-c port=SWD ap=1"

applicfg="$cube_fw_path/Utilities/PC_Software/ROT_AppliConfig/dist/AppliCfg.exe"
uname | grep -i -e windows -e mingw
if [ $? == 0 ] && [ -e "$applicfg" ]; then
  #line for window executable
  echo AppliCfg with windows executable
  python=""
else
  #line for python
  echo AppliCfg with python script
  applicfg="$cube_fw_path/Utilities/PC_Software/ROT_AppliConfig/AppliCfg.py"
  #determine/check python version command
  python="python3 "
fi

# Generate password hash binary
$python$applicfg hashcontent $hash_bin -i $password_bin

# Write password hash in OTP bits
"$stm32programmercli" "$connect_reset" -el "$stm32ExtOTPInterace" -otp fwrite lock "$hash_bin" word=$da_pw_hash_otp
if [ $? -ne 0 ]; then
	rm -f "$hash_bin"
	echo "Error"
	exec "$SHELL"
	exit 1
fi

rm -f "$hash_bin"
echo "Password hash provisioned successfully"
exec "$SHELL"
exit 0
