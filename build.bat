@echo off
REM Go to project rppt
cd /d %~dp0

REM Create build and output directories if unexistent
if not exist build mkdir build
if not exist output mkdir output
if not exist logs mkdir logs

REM Go to build directory
cd build

REM Run CMake to configure & build project
cmake ..
cmake --build .

REM Paths
set BUILD_DIR=%~dp0build
set DEBUG_EXE=%BUILD_DIR%\Debug\fractal_generator.exe
set RELEASE_EXE=%BUILD_DIR%\Release\fractal_generator.exe

REM Run executable if exists
if exist "%DEBUG_EXE%" (
    "%DEBUG_EXE%"
) else if exist "%RELEASE_EXE%" (
    "%RELEASE_EXE%"
) else (
    echo Executable not found!
)

