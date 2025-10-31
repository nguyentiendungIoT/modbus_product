@echo off
setlocal enabledelayedexpansion
REM flash.bat - Flash STM32 firmware using STM32_Programmer_CLI
REM Auto-detects project name from .elf file in Debug folder

set "STM32_PROGRAMMER=C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe"
set "BUILD_DIR=%~dp0Debug"

REM Try CubeIDE's embedded programmer if standalone not found
if not exist "!STM32_PROGRAMMER!" (
  set "STM32_PROGRAMMER=C:\ST\STM32CubeIDE_1.16.1\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.win32_2.2.200.202503041107\tools\bin\STM32_Programmer_CLI.exe"
)

if not exist "!STM32_PROGRAMMER!" (
  echo [ERROR] STM32_Programmer_CLI not found
  pause
  exit /b 1
)

REM Auto-detect: find first .elf file in Debug folder
set "ELF_FILE="
for /f "delims=" %%F in ('dir /b "!BUILD_DIR!\*.elf" 2^>nul') do (
  set "ELF_FILE=!BUILD_DIR!\%%F"
  goto found_elf
)

echo [ERROR] No .elf file found in !BUILD_DIR!
pause
exit /b 1

:found_elf
if not exist "!ELF_FILE!" (
  echo [ERROR] ELF file not found: !ELF_FILE!
  pause
  exit /b 1
)

REM Extract project name from filename
for %%F in ("!ELF_FILE!") do set "PROJECT_NAME=%%~nF"
set "PROJECT_NAME=!PROJECT_NAME:.elf=!"

echo ======================================
echo STM32 Flash Tool
echo ======================================
echo Project: !PROJECT_NAME!
echo ELF File: !ELF_FILE!
echo.
echo Connecting to ST-Link...
echo.

"!STM32_PROGRAMMER!" -c port=SWD -w "!ELF_FILE!" -v -rst

if errorlevel 1 (
  echo.
  echo [ERROR] Flash failed
  pause
  exit /b 1
)

echo.
echo [SUCCESS] Firmware flashed successfully!
pause
endlocal
