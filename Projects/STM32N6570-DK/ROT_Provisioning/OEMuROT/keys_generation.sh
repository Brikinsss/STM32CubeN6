#!/bin/bash -

# Getting the CubeProgammer_CLI path
source ../env.sh

# Absolute path to this script
if [ $# -ge 1 ] && [ -d $1 ]; then
  projectdir=$1
else
  projectdir=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
fi

# Keys folder
pushd "$projectdir/Keys"
keys_pem_dir=$(pwd)
popd

s_auth_key_pair_pem=$keys_pem_dir/OEMuROT_Authentication_S.pem
ns_auth_key_pair_pem=$keys_pem_dir/OEMuROT_Authentication_NS.pem
enc_key_pair_pem=$keys_pem_dir/OEMuROT_Encryption.pem
s_auth_public_key_hash_bin=$keys_pem_dir/OEMuROT_Authentication_S_Pub_Hash.bin
ns_auth_public_key_hash_bin=$keys_pem_dir/OEMuROT_Authentication_NS_Pub_Hash.bin
enc_private_key_bin=$keys_pem_dir/OEMuROT_Encryption_Priv.bin

error()
{
  echo "Error when trying to $action"
  echo "keys generation aborted"
  echo
  $SHELL
}

# Generate key pairs for signature and encryption
action="Generate Secure Authentication key pair"
echo $action
"$imgtool" keygen -k "$s_auth_key_pair_pem" -t ecdsa-p256
if [ $? -ne 0 ]; then
	error
fi

action="Generate Non Secure Authentication key pair"
echo $action
"$imgtool" keygen -k "$ns_auth_key_pair_pem" -t ecdsa-p256 
if [ $? -ne 0 ]; then
	error
fi

action="Generate Encryption Authentication key pair"
echo $action
"$imgtool" keygen -k "$enc_key_pair_pem" -t ecdsa-p256
if [ $? -ne 0 ]; then
	error
fi

# Generate public keys hash in binary format
action="Generate Secure public keys hash in binary format"
echo $action
"$imgtool" getpubbin --sha -k "$s_auth_key_pair_pem" >  "$s_auth_public_key_hash_bin"
if [ $? -ne 0 ]; then
	error
fi

action="Generate Non Secure public keys hash in binary format"
echo $action
"$imgtool" getpubbin --sha -k "$ns_auth_key_pair_pem" > "$ns_auth_public_key_hash_bin"
if [ $? -ne 0 ]; then
	error
fi


# Generate encrpytion private key in binary format
action="Generate encrpytion private keys in binary format"
echo $action
"$imgtool" getprivbin --raw --minimal -k "$enc_key_pair_pem" > "$enc_private_key_bin"
if [ $? -ne 0 ]; then
	error
fi

echo "Keys generated successfully"
$SHELL
