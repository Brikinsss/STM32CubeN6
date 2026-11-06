@echo off
call ../env.bat

:: Enable delayed expansion
setlocal EnableDelayedExpansion

set hashtable_otp=160
set oem_secret_otp=364
set rma_password_otp=256

set hashtable_binary=Keys/publicKeysHashHashes.bin
set oem_secret=Keys/OEM_SECRET.bin
set rma_password=Keys/RMA_password.bin

set connect_no_reset=-c port=SWD ap=1

set command="%stm32programmercli%" %connect_no_reset% -el "%stm32ExtOTPInterace%" -otp fwrite lock "%hashtable_binary%" word=%hashtable_otp%
!command!
IF !errorlevel! NEQ 0 goto :error

set command="%stm32programmercli%" %connect_no_reset% -el "%stm32ExtOTPInterace%" -otp fwrite lock "%oem_secret%" word=%oem_secret_otp%
!command!
IF !errorlevel! NEQ 0 goto :error

set command="%stm32programmercli%" %connect_no_reset% -el "%stm32ExtOTPInterace%" -otp fwrite lock "%rma_password%" word=%rma_password_otp%
!command!
IF !errorlevel! NEQ 0 goto :error

echo Keys programmed successfully
cmd /k
exit 0

:error
echo Error
cmd /k
exit 1