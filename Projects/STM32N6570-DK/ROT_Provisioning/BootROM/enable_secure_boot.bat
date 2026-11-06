@echo off
call ../env.bat

:: Enable delayed expansion
setlocal EnableDelayedExpansion

set hconf1_otp=124
set bootrom_config_9_otp=18

set hconf1_value=0x100000
set bootrom_config_9_value=0x1FF

set connect_no_reset=-c port=SWD ap=1

set command="%stm32programmercli%" %connect_no_reset% -el "%stm32ExtOTPInterace%" -otp write word=%hconf1_otp% value=%hconf1_value% word=%bootrom_config_9_otp% value=%bootrom_config_9_value%
!command!
IF !errorlevel! NEQ 0 goto :error

echo Secure boot has been activated
cmd /k
exit 0

:error
echo Error
cmd /k
exit 1