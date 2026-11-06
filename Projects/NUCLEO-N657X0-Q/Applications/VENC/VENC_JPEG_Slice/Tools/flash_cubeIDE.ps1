# PowerShell script to sign and flash VENC_USB for NUCLEO-N657X0-Q
$ErrorActionPreference = "Stop"

$candidatePaths = @(
    "F:\ST\STM32CubeIDE_2.2.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.win32_2.2.500.202603051304\tools\bin",
    "F:\STM32CubeProgrammer\bin",
    "C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin",
    "D:\STM32CubeProgrammer\bin"
)
$progPath = ""
foreach ($p in $candidatePaths) {
    if (Test-Path "$p\STM32_Programmer_CLI.exe") {
        $progPath = $p
        break
    }
}
if (-not $progPath) {
    $found = Get-Command "STM32_Programmer_CLI.exe" -ErrorAction SilentlyContinue
    if ($found) {
        $progPath = Split-Path $found.Source
    } else {
        Write-Error "STM32CubeProgrammer not found! Please ensure it is installed."
    }
}

$env:Path = "$progPath;$env:Path"
$loader = "$progPath\ExternalLoader\MX25UM51245G_STM32N6570-NUCLEO.stldr"
$offsetImage = "0x70100000"
$offsetFsbl  = "0x70000000"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
Set-Location $scriptDir

$tmpDir = Join-Path $scriptDir "tmp"
if (-not (Test-Path $tmpDir)) {
    New-Item -ItemType Directory -Path $tmpDir | Out-Null
} else {
    Get-ChildItem -Path $tmpDir | Remove-Item -Force
}

$appliTrusted = Resolve-Path "../STM32CubeIDE/Appli/Debug/VENC_JPEG_Slice_Nucleo_Appli-Trusted.bin" -ErrorAction SilentlyContinue
if (-not $appliTrusted) {
    $appliTrusted = Resolve-Path "../STM32CubeIDE/Appli/Debug/VENC_USB_Appli-Trusted.bin" -ErrorAction SilentlyContinue
}
$fsblTrusted  = Resolve-Path "../STM32CubeIDE/FSBL/Debug/VENC_USB_Nucleo_FSBL-Trusted.bin" -ErrorAction SilentlyContinue
if (-not $fsblTrusted) {
    $fsblTrusted  = Resolve-Path "../STM32CubeIDE/FSBL/Debug/VENC_USB_FSBL-Trusted.bin" -ErrorAction SilentlyContinue
}

$appliBin = Resolve-Path "../STM32CubeIDE/Appli/Debug/VENC_JPEG_Slice_Nucleo_Appli.bin" -ErrorAction SilentlyContinue
if (-not $appliBin) {
    $appliBin = Resolve-Path "../STM32CubeIDE/Appli/Debug/VENC_USB_Appli.bin" -ErrorAction SilentlyContinue
}
$fsblBin  = Resolve-Path "../STM32CubeIDE/FSBL/Debug/VENC_USB_Nucleo_FSBL.bin" -ErrorAction SilentlyContinue
if (-not $fsblBin) {
    $fsblBin  = Resolve-Path "../STM32CubeIDE/FSBL/Debug/VENC_USB_FSBL.bin" -ErrorAction SilentlyContinue
}

Write-Host "=== NUCLEO-N657X0-Q VENC_JPEG_Slice Sign & Flash Utility ===" -ForegroundColor Cyan
Write-Host "External loader:    $loader"
Write-Host "FSBL binary:        $($fsblTrusted.Path)"
Write-Host "Appli binary:       $($appliTrusted.Path)"

if (-not $fsblTrusted -or -not (Test-Path $fsblTrusted)) {
    if ($fsblBin -and (Test-Path $fsblBin)) {
        Write-Host "Signing FSBL..." -ForegroundColor Yellow
        $fsblTrustedPath = Join-Path $tmpDir "VENC_USB_FSBL-Trusted.bin"
        & "$progPath\STM32_SigningTool_CLI.exe" -bin "$($fsblBin.Path)" -nk -t ssbl -hv 2.3 -align -o "$fsblTrustedPath"
        $fsblTrusted = Get-Item $fsblTrustedPath
    } else {
        Write-Error "FSBL binary not found! Please build the FSBL project first."
    }
}

if (-not $appliTrusted -or -not (Test-Path $appliTrusted)) {
    if ($appliBin -and (Test-Path $appliBin)) {
        Write-Host "Signing Appli..." -ForegroundColor Yellow
        $appliTrustedPath = Join-Path $tmpDir "VENC_USB_Appli-Trusted.bin"
        & "$progPath\STM32_SigningTool_CLI.exe" -bin "$($appliBin.Path)" -nk -t ssbl -hv 2.3 -align -o "$appliTrustedPath"
        $appliTrusted = Get-Item $appliTrustedPath
    } else {
        Write-Error "Appli binary not found! Please build the Appli project first."
    }
}

Write-Host "`nReady to flash target NUCLEO-N657X0-Q via ST-LINK SWD..." -ForegroundColor Green
Write-Host "Flashing FSBL to $offsetFsbl..." -ForegroundColor Yellow
& "$progPath\STM32_Programmer_CLI.exe" -c port=swd mode=HOTPLUG -el "$loader" -hardRst -d "$($fsblTrusted.Path)" $offsetFsbl

Write-Host "Flashing Appli to $offsetImage..." -ForegroundColor Yellow
& "$progPath\STM32_Programmer_CLI.exe" -c port=swd mode=HOTPLUG -el "$loader" -hardRst -d "$($appliTrusted.Path)" $offsetImage

Write-Host "`nFlashing completed successfully!" -ForegroundColor Green
Write-Host "Ensure boot switches are configured to Boot from Flash and reset the board."
