@echo off
echo GTAAudioMetadataTool - Clean Dependencies
echo ========================================

:: Change to parent directory
cd /d "%~dp0.."

if not exist "premake5.exe" (
    echo Error: premake5.exe not found in project root
    pause
    exit /b 1
)

echo Cleaning dependencies...
premake5.exe clean-deps

echo.
echo Dependencies cleaned!
echo Run Scripts\setup.bat to reinstall dependencies and regenerate project files.
pause