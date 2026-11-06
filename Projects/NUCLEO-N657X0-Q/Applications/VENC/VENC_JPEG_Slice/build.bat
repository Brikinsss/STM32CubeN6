@echo off
setlocal enabledelayedexpansion

:: ============================================================================
:: STM32N657 VENC_JPEG_Slice Fast Command-Line Build & Flash Script
:: ============================================================================

set "SCRIPT_DIR=%~dp0"
set "APPLI_DEBUG_DIR=%SCRIPT_DIR%STM32CubeIDE\Appli\Debug"

:: Toolchain paths
set "GCC_DIR=F:\ST\STM32CubeIDE_2.2.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.14.3.rel1.win32_1.0.100.202602081740\tools\bin"
set "MAKE_DIR=F:\ST\STM32CubeIDE_2.2.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.make.win32_2.2.200.202604021615\tools\bin"
set "PROG_DIR=F:\ST\STM32CubeIDE_2.2.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.win32_2.2.500.202603051304\tools\bin"

set "MAKE_EXE=%MAKE_DIR%\make.exe"
set "PROG_EXE=%PROG_DIR%\STM32_Programmer_CLI.exe"
set "LOADER=%PROG_DIR%\ExternalLoader\MX25UM51245G_STM32N6570-NUCLEO.stldr"
set "TARGET_BIN=%APPLI_DEBUG_DIR%\VENC_JPEG_Slice_Nucleo_Appli-Trusted.bin"
set "STLINK_SN=001F00293434511734313937"

:: Add GCC and Make to PATH
set "PATH=%GCC_DIR%;%MAKE_DIR%;%PROG_DIR%;%PATH%"

set ACTION=%1
if "%ACTION%"=="" set ACTION=all

echo ================================================================
echo  STM32N657 VENC JPEG Slice Fast Multi-Core Build Tool
echo  Action : %ACTION%
echo  Cores  : %NUMBER_OF_PROCESSORS% parallel build threads
echo ================================================================

if /i "%ACTION%"=="clean" goto do_clean
if /i "%ACTION%"=="rebuild" goto do_rebuild
if /i "%ACTION%"=="flash" goto do_flash
if /i "%ACTION%"=="reset" goto do_reset
if /i "%ACTION%"=="all" goto do_build

:help
echo Usage:
echo   build.bat         - Fast incremental build (multi-core, ~1s)
echo   build.bat flash   - Build and flash to board via ST-Link
echo   build.bat clean   - Clean build directory
echo   build.bat rebuild - Full clean and rebuild
echo   build.bat reset   - Hardware reset target MCU
exit /b 0

:do_clean
echo [*] Cleaning build files...
"%MAKE_EXE%" -C "%APPLI_DEBUG_DIR%" clean
if exist "%TARGET_BIN%" del /f /q "%TARGET_BIN%"
echo [v] Clean finished.
exit /b 0

:do_build
echo [*] Compiling with %NUMBER_OF_PROCESSORS% parallel threads...
"%MAKE_EXE%" -C "%APPLI_DEBUG_DIR%" -j%NUMBER_OF_PROCESSORS% all
if %ERRORLEVEL% NEQ 0 (
    echo [x] Compilation failed!
    exit /b %ERRORLEVEL%
)
echo.
echo [v] Compilation and Signing finished successfully!
echo [v] Output: %TARGET_BIN%
exit /b 0

:do_rebuild
call :do_clean
goto do_build

:do_flash
call :do_build
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
echo.
echo [*] Flashing to STM32N6 Nucleo Flash (0x70100000)...
"%PROG_EXE%" -c port=swd sn=%STLINK_SN% mode=HOTPLUG -el "%LOADER%" -d "%TARGET_BIN%" 0x70100000 -hardRst
if %ERRORLEVEL% EQU 0 (
    echo [v] Flashing finished successfully!
    exit /b 0
)

echo [!] Direct connect failed, attempting Under-Reset wake-up...
"%PROG_EXE%" -c port=swd sn=%STLINK_SN% mode=UR -hardRst >nul 2>&1
"%PROG_EXE%" -c port=swd sn=%STLINK_SN% mode=HOTPLUG -el "%LOADER%" -d "%TARGET_BIN%" 0x70100000 -hardRst
if %ERRORLEVEL% NEQ 0 (
    echo [x] Flashing failed!
    exit /b 1
)
echo [v] Flashing finished successfully!
exit /b 0

:do_reset
echo [*] Resetting MCU via ST-Link...
"%PROG_EXE%" -c port=swd sn=%STLINK_SN% mode=UR -hardRst
echo [v] Reset done.
exit /b 0
