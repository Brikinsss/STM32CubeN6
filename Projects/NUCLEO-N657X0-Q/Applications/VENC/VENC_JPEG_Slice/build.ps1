<#
.SYNOPSIS
    Fast Multi-core Build & Flash script for STM32N657 VENC_JPEG_Slice
.DESCRIPTION
    Uses ARM GCC 14.3 + GNU Make multi-threading (-j) for ultra-fast builds.
    Automatically signs binary with STM32_SigningTool_CLI and flashes via ST-LINK.
.EXAMPLE
    .\build.ps1          # Fast incremental build (~1-2 seconds)
    .\build.ps1 flash    # Fast build + Flash to board + Reset
    .\build.ps1 clean    # Clean build artifacts
    .\build.ps1 rebuild  # Clean and rebuild
    .\build.ps1 reset    # Hardware reset MCU via ST-Link
#>

param(
    [ValidateSet("all", "flash", "clean", "rebuild", "reset")]
    [string]$Action = "all"
)

$ErrorActionPreference = "Stop"

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$AppliDebugDir = Join-Path $ScriptDir "STM32CubeIDE\Appli\Debug"

$GccBin  = "F:\ST\STM32CubeIDE_2.2.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.14.3.rel1.win32_1.0.100.202602081740\tools\bin"
$MakeBin = "F:\ST\STM32CubeIDE_2.2.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.make.win32_2.2.200.202604021615\tools\bin"
$ProgBin = "F:\ST\STM32CubeIDE_2.2.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.win32_2.2.500.202603051304\tools\bin"

$MakeExe   = Join-Path $MakeBin "make.exe"
$ProgExe   = Join-Path $ProgBin "STM32_Programmer_CLI.exe"
$Loader    = Join-Path $ProgBin "ExternalLoader\MX25UM51245G_STM32N6570-NUCLEO.stldr"
$TargetBin = Join-Path $AppliDebugDir "VENC_JPEG_Slice_Nucleo_Appli-Trusted.bin"
$StlinkSn  = "001F00293434511734313937"

# Setup PATH for sub-processes
$env:PATH = "$GccBin;$MakeBin;$ProgBin;" + $env:PATH
$Threads = [string]$env:NUMBER_OF_PROCESSORS

Write-Host "================================================================" -ForegroundColor Cyan
Write-Host " STM32N657 VENC JPEG Slice Fast Multi-Core Build Tool" -ForegroundColor Cyan
Write-Host " Action : $Action" -ForegroundColor White
Write-Host " Cores  : $Threads parallel build threads" -ForegroundColor White
Write-Host "================================================================" -ForegroundColor Cyan

function Invoke-Clean {
    Write-Host "[*] Cleaning build directory..." -ForegroundColor Yellow
    & $MakeExe -C $AppliDebugDir clean
    if (Test-Path $TargetBin) {
        Remove-Item -Force $TargetBin
    }
    Write-Host "[✓] Clean completed." -ForegroundColor Green
}

function Invoke-Build {
    $sw = [System.Diagnostics.Stopwatch]::StartNew()
    Write-Host "[*] Compiling with $Threads threads..." -ForegroundColor Yellow
    $jArg = "-j" + $Threads
    & $MakeExe "-C" "$AppliDebugDir" "$jArg" "all"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "[x] Build failed with error code $LASTEXITCODE" -ForegroundColor Red
        exit $LASTEXITCODE
    }
    $sw.Stop()
    Write-Host ("[✓] Build & Signing succeeded in {0:N2}s" -f $sw.Elapsed.TotalSeconds) -ForegroundColor Green
    Write-Host "[✓] Output: $TargetBin" -ForegroundColor Gray
}

function Invoke-Flash {
    Write-Host "`n[*] Flashing to NUCLEO-N657X0-Q (0x70100000)..." -ForegroundColor Yellow
    $sw = [System.Diagnostics.Stopwatch]::StartNew()

    $out = & $ProgExe -c port=swd sn=$StlinkSn mode=HOTPLUG -el "$Loader" -d "$TargetBin" 0x70100000 -hardRst 2>&1
    $outText = $out -join "`n"
    Write-Host $outText

    if ($outText -notmatch "File download complete") {
        Write-Host "[!] Direct connect failed, executing Under-Reset wake-up..." -ForegroundColor Magenta
        & $ProgExe -c port=swd sn=$StlinkSn mode=UR -hardRst 2>&1 | Out-Null
        $out2 = & $ProgExe -c port=swd sn=$StlinkSn mode=HOTPLUG -el "$Loader" -d "$TargetBin" 0x70100000 -hardRst 2>&1
        $outText2 = $out2 -join "`n"
        Write-Host $outText2
        if ($outText2 -notmatch "File download complete") {
            Write-Host "[x] Flash failed!" -ForegroundColor Red
            exit 1
        }
    }
    $sw.Stop()
    Write-Host ("[✓] Flashing and reset complete in {0:N2}s" -f $sw.Elapsed.TotalSeconds) -ForegroundColor Green
}

function Invoke-Reset {
    Write-Host "[*] Performing hardware reset via ST-Link..." -ForegroundColor Yellow
    & $ProgExe -c port=swd sn=$StlinkSn mode=UR -hardRst
    Write-Host "[✓] Target reset complete." -ForegroundColor Green
}

switch ($Action) {
    "clean"   { Invoke-Clean }
    "rebuild" { Invoke-Clean; Invoke-Build }
    "all"     { Invoke-Build }
    "flash"   { Invoke-Build; Invoke-Flash }
    "reset"   { Invoke-Reset }
}
