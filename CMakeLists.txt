cmake_minimum_required(VERSION 3.10)

project(fractal_generator)

#Use C++ v17
set(CMAKE_CXX_STANDARD 17)

#Include header files, .hpp
include_directories(include)

#output location
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

#Executable and source files
add_executable(fractal_generator
    main.cpp
    src/fractal.cpp
    src/color.cpp
    src/img_writer.cpp
    src/extension_conver.cpp
    src/palette.cpp
)
