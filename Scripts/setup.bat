@echo off
echo GTAAudioMetadataTool - Premake Setup
echo =====================================

:: Change to parent directory
cd /d "%~dp0.."

:: Check if premake5.exe exists in project root
if not exist "premake5.exe" (
    echo Error: premake5.exe not found in project root
    echo Please download premake5.exe and place it in the project root folder
    echo Download from: https://premake.github.io/download
    pause
    exit /b 1
)

echo.
echo Setting up dependencies...
premake5.exe setup-deps

echo.
echo Generating Visual Studio 2022 project files...
premake5.exe vs2022

echo.
echo Setup complete!
echo.
pause