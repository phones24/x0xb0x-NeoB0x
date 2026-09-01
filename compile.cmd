@echo off
setlocal

set "MCU=%1"
if not defined MCU set "MCU=atmega162"

if "%MCU%"=="atmega162" goto build
if "%MCU%"=="atmega2561" goto build

echo Usage: compile.cmd [atmega2561^|atmega162]
echo   atmega162  - original x0xb0x CPU
echo   atmega2561 - x0xlarge ^(default^)
exit /b 1

:build
cd /d "%~dp0"

make clean
if errorlevel 1 exit /b 1

make MCU=%MCU%
