@echo off
echo Executing STM32 code formatting script...
echo Temporarily bypassing PowerShell execution policy (for current process only)
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0format_stm32.ps1"
if %errorlevel% neq 0 (
    echo Script execution error, error code: %errorlevel%
    pause
) else (
    echo Formatting completed.
    pause
)