@echo off
setlocal
REM build_debug.bat - build modbus_example using CubeIDE tools (adjust CUBE_ROOT if needed)

REM --- Edit this path if your CubeIDE is installed elsewhere ---
set "CUBE_ROOT=C:\ST\STM32CubeIDE_1.16.1\STM32CubeIDE"

REM plugin folder names found in your environment (use dir to discover if different)
set "MAKE_PLUGIN=com.st.stm32cube.ide.mcu.externaltools.make.win32_2.2.0.202409170845"
set "GNU_PLUGIN=com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.win32_1.0.0.202411081344"

set "MAKE_BIN=%CUBE_ROOT%\plugins\%MAKE_PLUGIN%\tools\bin"
set "GNU_BIN=%CUBE_ROOT%\plugins\%GNU_PLUGIN%\tools\bin"

set "PATH=%MAKE_BIN%;%GNU_BIN%;%PATH%"

REM Ensure NUMBER_OF_PROCESSORS has a sensible default
if "%NUMBER_OF_PROCESSORS%"=="" set "NUMBER_OF_PROCESSORS=1"

REM Pre-clean Debug folder if present
if exist "%~dp0Debug" (
 echo Cleaning previous build in "%~dp0Debug"...
 pushd "%~dp0Debug"
 make clean
 popd
) else (
 echo Debug folder not found, skipping pre-clean.
)

REM go to Debug folder and build
cd /d "%~dp0Debug"
echo Working dir: %CD%
make clean
make all -j%NUMBER_OF_PROCESSORS%

if errorlevel 1 (
 echo Build failed (exit %ERRORLEVEL%). You can inspect the console output above.
 pause
 exit /b %ERRORLEVEL%
)

echo Build succeeded. Artifacts in "%~dp0Debug"
pause
endlocal
