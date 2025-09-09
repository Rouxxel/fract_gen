#!/bin/bash
set -e  #exit on first error

#Project root
cd "$(dirname "$0")"

#Create build and output dirs if unexistent
mkdir -p build
mkdir -p output

#Go into build folder
cd build

#Run CMake to configure & build executable
cmake ..
cmake --build .

#Optional argument: if "run" is passed, execute the program
if [ "$1" = "run" ]; then
    #Try Debug folder first, fallback to Release
    if [ -f "./Debug/fractal_generator.exe" ]; then
        ./Debug/fractal_generator.exe
    elif [ -f "./Release/fractal_generator.exe" ]; then
        ./Release/fractal_generator.exe
    else
        echo "Executable not found!"
    fi
else
    echo "Build completed. Run './build.sh run' to execute the program."
fi
