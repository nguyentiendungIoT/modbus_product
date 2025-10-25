@echo off
setlocal enabledelayedexpansion
REM debug.bat - Start GDB debug session for STM32 using OpenOCD
REM Auto-detects project name from .elf file in Debug folder

set "CUBE_ROOT=C:\ST\STM32CubeIDE_1.16.1\STM32CubeIDE"
set "BUILD_DIR=%~dp0Debug"

REM Tool paths
set "OPENOCD_PLUGIN=com.st.stm32cube.ide.mcu.externaltools.openocd.win32_2.4.200.202505051030"
set "GNU_PLUGIN=com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.win32_1.0.0.202411081344"

set "OPENOCD_BIN=!CUBE_ROOT!\plugins\!OPENOCD_PLUGIN!\tools\bin"
set "GDB_BIN=!CUBE_ROOT!\plugins\!GNU_PLUGIN!\tools\bin"

set "OPENOCD=!OPENOCD_BIN!\openocd.exe"
set "GDB=!GDB_BIN!\arm-none-eabi-gdb.exe"

REM OpenOCD config for STM32F407
set "OPENOCD_SCRIPTS=!OPENOCD_BIN!\..\scripts"
set "OPENOCD_CFG=-f interface/stlink.cfg -f target/stm32f4x.cfg"

REM Find first .elf file in Debug folder
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

REM Extract project name
for %%F in ("!ELF_FILE!") do set "PROJECT_NAME=%%~nF"
set "PROJECT_NAME=!PROJECT_NAME:.elf=!"

REM Validate tools
if not exist "!OPENOCD!" (
  echo [ERROR] OpenOCD not found at: !OPENOCD!
  pause
  exit /b 1
)

if not exist "!GDB!" (
  echo [ERROR] GDB not found at: !GDB!
  pause
  exit /b 1
)

REM Start debug session
echo ======================================
echo STM32 GDB Debug Session
echo ======================================
echo Project: !PROJECT_NAME!
echo ELF File: !ELF_FILE!
echo.
echo Starting OpenOCD server...

start "OpenOCD Server" /MIN "!OPENOCD!" -s "!OPENOCD_SCRIPTS!" !OPENOCD_CFG!
timeout /t 2 /nobreak >nul

echo Starting GDB...
echo.

"!GDB!" -q ^
  -ex "target extended-remote :3333" ^
  -ex "monitor reset halt" ^
  -ex "file !ELF_FILE!" ^
  -ex "load" ^
  -ex "monitor reset halt"

echo.
echo Stopping OpenOCD server...
taskkill /FI "WINDOWTITLE eq OpenOCD Server*" /T /F >nul 2>&1

echo [DEBUG SESSION ENDED]
pause
endlocal
