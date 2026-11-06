#!/bin/bash -

# Getting the CubeProgammer_CLI path
source ../env.sh

spw1=rot1
spw2=rot2
spw3=rot3
spw4=rot4
spw5=rot5
spw6=rot6
spw7=rot7
spw8=rot8

error()
{
  echo "Error when trying to $action"
  echo "keys generation aborted"
  echo
  $SHELL
}

# Generate 8 key pairs for signature
action="Generate 8 key pairs for signature"
"$stm32keygencli" -abs Keys -pwd $spw1 $spw2 $spw3 $spw4 $spw5 $spw6 $spw7 $spw8 -n 8
if [ $? -ne 0 ]; then
	error
fi

# Generate secret key for encryption
action="Generate secret key for encryption"
"$stm32keygencli" -rand 16 Keys/OEM_SECRET.bin
if [ $? -ne 0 ]; then
	error
fi

# Generate RMA password
action="Generate RMA password"
"$stm32keygencli" -rand 16 Keys/RMA_password.bin
if [ $? -ne 0 ]; then
	error
fi

echo "Keys generated successfully"
$SHELL
