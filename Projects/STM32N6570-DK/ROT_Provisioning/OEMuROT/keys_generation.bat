@echo off
call ../env.bat

:: Enable delayed expansion
setlocal EnableDelayedExpansion

:: Keys folder
set projectdir=%~dp0
pushd "%projectdir%"\Keys
set keys_pem_dir=%cd%
popd

set s_auth_key_pair_pem=%keys_pem_dir%\OEMuROT_Authentication_S.pem
set ns_auth_key_pair_pem=%keys_pem_dir%\OEMuROT_Authentication_NS.pem
set enc_key_pair_pem=%keys_pem_dir%\OEMuROT_Encryption.pem
set s_auth_public_key_hash_bin=%keys_pem_dir%\OEMuROT_Authentication_S_Pub_Hash.bin
set ns_auth_public_key_hash_bin=%keys_pem_dir%\OEMuROT_Authentication_NS_Pub_Hash.bin
set enc_private_key_bin=%keys_pem_dir%\OEMuROT_Encryption_Priv.bin

:: Generate key pairs for signature and encryption
set command="%imgtool%" keygen -k "%s_auth_key_pair_pem%" -t ecdsa-p256
!command!
IF !errorlevel! NEQ 0 goto :error

set command="%imgtool%" keygen -k "%ns_auth_key_pair_pem%" -t ecdsa-p256
!command!
IF !errorlevel! NEQ 0 goto :error

set command="%imgtool%" keygen -k "%enc_key_pair_pem%" -t ecdsa-p256
!command!
IF !errorlevel! NEQ 0 goto :error

:: Generate public keys hash in binary format
set command="%imgtool%" getpubbin --sha -k "%s_auth_key_pair_pem%"
!command! > "%s_auth_public_key_hash_bin%"
IF !errorlevel! NEQ 0 goto :error

set command="%imgtool%" getpubbin --sha -k "%ns_auth_key_pair_pem%"
!command! > "%ns_auth_public_key_hash_bin%"
IF !errorlevel! NEQ 0 goto :error

:: Generate encrpytion private key in binary format
set command="%imgtool%" getprivbin --raw --minimal -k "%enc_key_pair_pem%"
!command! > "%enc_private_key_bin%"
IF !errorlevel! NEQ 0 goto :error

echo Keys generated successfully
cmd /k
exit 0

:error
echo Failed to generate keys
echo "%command% failed"
cmd /k
exit 1