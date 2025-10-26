@echo off
setlocal enabledelayedexpansion

echo GTAAudioMetadataTool - Build Script
echo ===================================

:: Change to parent directory
cd /d "%~dp0.."

set "CONFIG=Release"

:: Parse command line arguments
:parse_args
if "%~1"=="" goto :build
if /i "%~1"=="debug" set "CONFIG=Debug"
if /i "%~1"=="release" set "CONFIG=Release"
shift
goto :parse_args

:build
echo Building configuration: %CONFIG%
echo.

:: Check if solution exists
if not exist "GTAAudioMetadataTool.sln" (
    echo Solution file not found. Running setup first...
    call Scripts\setup.bat
    if errorlevel 1 (
        echo Setup failed!
        exit /b 1
    )
)

:: Find MSBuild
set "MSBUILD_PATH="
for /f "usebackq tokens=*" %%i in (`where msbuild 2^>nul`) do set "MSBUILD_PATH=%%i"

if "%MSBUILD_PATH%"=="" (
    echo MSBuild not found in PATH. Searching for Visual Studio installation...
    
    :: Try common Visual Studio 2022 locations
    if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
    ) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe"
    ) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
    ) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe"
    ) else (
        echo Error: MSBuild not found!
        echo Please install Visual Studio 2022 or Visual Studio Build Tools 2022
        echo Or run this script from a Developer Command Prompt
        exit /b 1
    )
)

:: Build the solution
echo Building solution...
echo Using MSBuild: %MSBUILD_PATH%
"%MSBUILD_PATH%" GTAAudioMetadataTool.sln /p:Configuration=%CONFIG% /p:Platform=x64 /m

if errorlevel 1 (
    echo Build failed!
    exit /b 1
)

echo.
echo Build successful!
echo Output: bin\%CONFIG%-windows-x86_64\ivam\ivam.exe
echo.
pause