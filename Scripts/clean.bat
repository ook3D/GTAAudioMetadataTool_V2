@echo off
echo GTAAudioMetadataTool - Clean Script
echo ==================================

:: Change to parent directory (project root)
cd /d "%~dp0.."

echo Cleaning build outputs...

if exist "bin" (
    rmdir /s /q "bin"
    echo - Removed bin directory
)

if exist "bin-int" (
    rmdir /s /q "bin-int"
    echo - Removed bin-int directory
)

if exist "*.sln" (
    del /q "*.sln"
    echo - Removed solution files
)

if exist "*.vcxproj*" (
    del /q "*.vcxproj*"
    echo - Removed project files
)

if exist ".vs" (
    rmdir /s /q ".vs"
    echo - Removed Visual Studio cache
)

echo.
echo Clean complete!
pause