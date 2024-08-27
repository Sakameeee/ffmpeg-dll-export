# FFmpeg DLL Export

This project demonstrates how to compile your own functions using FFmpeg's static or dynamic libraries and export them as a DLL file on a Windows platform using CMake and MinGW64. The project includes an example function that merges audio and video files. You can modify this framework to implement your own functions.

## Prerequisites

- **Platform:** CMake + MinGW64 + Windows
- **FFmpeg Libraries:** Precompiled using MinGW on Windows
- **Pkg-config:** Required for linking FFmpeg static libraries

### Notes

- Since FFmpeg was compiled on Windows using MinGW, this project also uses the MinGW compiler rather than MSVC to export the DLL file.
- Static library files compiled with MinGW on Windows (`.a` files) cannot be read on Linux. To compile `.so` files for the Linux platform, you must recompile FFmpeg on Linux and replace the existing files.
- When linking static libraries, it's recommended to use `pkg-config` to automatically find dependencies, as manually linking libraries using `link_libraries` might result in errors.

## FFmpeg Compilation on Windows

Below is an example of the configuration parameters used to compile FFmpeg's static libraries on the Windows platform:

```bash
./configure \
    --prefix=/d/download/ffmpeg-full \
    --disable-shared \
    --enable-static \
    --disable-doc \
    --disable-programs \
    --enable-protocols \
    --disable-debug \
    --disable-x86asm
```

## Quick Start

1. Navigate to the `script` directory.
2. Run the `build.py` script.
3. Compiled files will be located in the `build` directory.
