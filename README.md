# CHIP-8 Emulator
[![Build Status](https://travis-ci.com/mgerhold/Chip8Emulator.svg?branch=master)](https://travis-ci.com/mgerhold/Chip8Emulator)
## What is this?
This is my humble CHIP-8 emulator. If you've never heard of the CHIP-8, please refer to [Wikipedia](https://en.wikipedia.org/wiki/CHIP-8). It is written in C++ and uses OpenGL for rendering. While the sound delay timer is implemented, it is not capable of playing sound at the moment.
## Dependencies
* [glfw](https://www.glfw.org/)
* [glad](https://github.com/Dav1dde/glad)
* [imgui](https://github.com/ocornut/imgui)
* [nativefiledialog](https://github.com/mlabbe/nativefiledialog)
* [Google Test](https://github.com/google/googletest) (for unit tests)
## Documentation
You can find the [doxygen](https://www.doxygen.nl/index.html)-generated documentation [here](https://mgerhold.github.io/Chip8Emulator/).
## How to build?
The easiest way to build this project is to use [CMake](https://cmake.org/) and [Vcpkg](https://github.com/microsoft/vcpkg). Please refer to the [documentation](https://mgerhold.github.io/Chip8Emulator/) for the individual steps.
## Platforms
This project has been tested with Windows 10 (64 Bit, MSVC) and Linux (64 Bit, GCC).
## Where to get roms?
You can find a collection of roms [here](https://github.com/dmatlack/chip8/tree/master/roms).
## Demonstration
You can find a little demonstration video on YouTube:
[![CHIP-8 demo video](https://img.youtube.com/vi/Z-y_b5GIZsQ/0.jpg)](https://www.youtube.com/watch?v=Z-y_b5GIZsQ)