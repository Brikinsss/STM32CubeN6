@echo off
call ../env.bat

:: Enable delayed expansion
setlocal EnableDelayedExpansion

set s_auth_public_key_otp=268
set ns_auth_public_key_otp=276
set enc_private_key_otp=284

set s_auth_public_key_bin=Keys/OEMuROT_Authentication_S_Pub_Hash.bin
set ns_auth_public_key_bin=Keys/OEMuROT_Authentication_NS_Pub_Hash.bin
set enc_private_key_bin=Keys/OEMuROT_Encryption_Priv.bin

set connect_reset=-c port=SWD ap=1

set command="%stm32programmercli%" %connect_reset% -el "%stm32ExtOTPInterace%" -otp fwrite lock "%s_auth_public_key_bin%" word=%s_auth_public_key_otp%
!command!
IF !errorlevel! NEQ 0 goto :error

set command="%stm32programmercli%" %connect_reset% -el "%stm32ExtOTPInterace%" -otp fwrite lock "%ns_auth_public_key_bin%" word=%ns_auth_public_key_otp%
!command!
IF !errorlevel! NEQ 0 goto :error

set command="%stm32programmercli%" %connect_reset% -el "%stm32ExtOTPInterace%" -otp fwrite lock "%enc_private_key_bin%" word=%enc_private_key_otp%
!command!
IF !errorlevel! NEQ 0 goto :error

echo Keys programmed successfully
cmd /k
exit 0

:error
echo Error
cmd /k
exit 1