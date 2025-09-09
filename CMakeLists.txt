cmake_minimum_required(VERSION 3.10)

project(fractal_generator)

#Use C++ v17
set(CMAKE_CXX_STANDARD 17)

#Include header files, .hpp
include_directories(include)

#Executable and source files
add_executable(fractal_generator
    main.cpp
    src/fractal.cpp
)
