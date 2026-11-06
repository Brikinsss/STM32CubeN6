@echo off
call ../env.bat

:: Enable delayed expansion
setlocal EnableDelayedExpansion

set spw1=rot1
set spw2=rot2
set spw3=rot3
set spw4=rot4
set spw5=rot5
set spw6=rot6
set spw7=rot7
set spw8=rot8

:: Generate 8 key pairs for signature
set command="%stm32keygencli%" -abs Keys -pwd %spw1% %spw2% %spw3% %spw4% %spw5% %spw6% %spw7% %spw8% -n 8
!command!
IF !errorlevel! NEQ 0 goto :error

:: Generate secret key for encryption
set command="%stm32keygencli%" -rand 16 Keys\OEM_SECRET.bin
!command!
IF !errorlevel! NEQ 0 goto :error

:: Generate RMA password
set command="%stm32keygencli%" -rand 16 Keys\RMA_password.bin
!command!
IF !errorlevel! NEQ 0 goto :error

echo Keys generated successfully
cmd /k
exit 0

:error
echo Error
cmd /k
exit 1