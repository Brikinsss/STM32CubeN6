call ../env.bat

:: Enable delayed expansion
setlocal EnableDelayedExpansion

:: Password binary
set projectdir=%~dp0
set password_bin=%projectdir%Config\DA_password.bin
set hash_bin=%projectdir%hash.bin
set da_pw_hash_otp=292

set connect_reset=-c port=SWD ap=1

::line for window executable
set "applicfg=%cube_fw_path%\Utilities\PC_Software\ROT_AppliConfig\dist\AppliCfg.exe"
set "python="
if exist %applicfg% (
echo run da_programming with windows executable
goto hash
)
:py
::line for python
echo run da_programming with python script
set "applicfg=%cube_fw_path%\Utilities\PC_Software\ROT_AppliConfig\AppliCfg.py"
set "python=python "

:hash
:: Generate password hash binary
%python%%applicfg% hashcontent %hash_bin% -i %password_bin%

:: Write password hash in OTP bits
set command="%stm32programmercli%" %connect_reset% -el "%stm32ExtOTPInterace%" -otp fwrite lock "%hash_bin%" word=%da_pw_hash_otp%
!command!
IF !errorlevel! NEQ 0 goto :error

del %hash_bin%
echo Password hash provisioned successfully
cmd /k
exit 0

:error
del %hash_bin%
echo Error
cmd /k
exit 1