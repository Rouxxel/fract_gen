# fract_gen

A simple **fractal generator** written in **C++**.  
Generates Mandelbrot fractals and saves them as `.ppm` images and also common formats like `.png` using **stb_image_write**, **json**.

---

## Features
- Generate Mandelbrot fractals with configurable resolution and iterations.
- Save images in **PPM** (default) and convert them to **PNG/JPG/BMP/TGA**.
- Allows custom file names, paths, and output formats.
- Uses modern C++17 and **CMake** for easy building.
- Supports Windows, Linux, and macOS.

---

## Project Structure

```
fract_gen/
├─ assets/ 
|   ├─ config/
|   |   └─ default_config.json
|   └─ palettes/
|       └─ *.json
├─ include/ 
|   ├─ color_rgb.hpp
|   ├─ extension_conver.hpp
|   ├─ fractal.hpp
|   ├─ img_writer.hpp
|   ├─ json.hpp
|   ├─ palette.hpp
|   └─ stb_image_write.hpp
├─ src/ 
|   ├─ color.cpp
|   ├─ extension_conver.cpp
|   ├─ fractal.cpp
|   ├─ img_writer.cpp
|   └─ palette.cpp
├─ .gitignore
├─ build.sh
├─ build.bat
├─ CMakeLists.txt
├─ main.cpp
└─ README.md
```

---

## Requirements
- **C++17** compatible compiler (MSVC, GCC, Clang)
- **CMake ≥ 3.10**
- Optional: **Git Bash / WSL** on Windows if using the shell script.

---

## Build & Run (Windows)

```powershell or terminal
#Create build and output folder
mkdir build
mkdir output
cd build

#Generate build files and compile
cmake ..
cmake --build .

#Run executable
.\Debug\fractal_generator.exe
```

**OR**

```powershell or terminal
#To simply build executable
.\build.bat

#To build and run
.\build.bat run  
```

---

## Build & Run (Linux/macOS)

```bash
#Go to project directory
cd /path/to/fract_gen

#Make executable (first time only)
chmod +x build.sh

#Build only
./build.sh

#Build and run
./build.sh run

```
---

## Usage

When you run the program, it will ask for:

- **Color palette json file** (default: `hardcoded`)

- **Output file name** (default: `output`)
- **Conversion format** (default: `.png`)
- **Image width/height** (default: `800x600`)
- **Maximum iterations** (default: `500`)

Press **Enter** to accept default values in any of the aforementioned.

The program will generate the fractal and save the image(s) to the output folder.

---

### Notes

- The project uses `stb_image_write.h` for exporting PNG/JPG/BMP/TGA.  
- The `build/` folder is used for CMake build files, and `output/` stores generated images.
