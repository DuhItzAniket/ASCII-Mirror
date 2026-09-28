# Development Guide

## Building from Source

### Prerequisites

- C++17 compiler
- CMake 3.16+
- OpenCV 4.x or 5.x

### Windows (MSYS2)

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-opencv
cmake -S . -B build
cmake --build build
```

### Linux

```bash
sudo apt install g++ cmake libopencv-dev
cmake -S . -B build
cmake --build build
```

## Project Structure

```
ascii-mirror/
├── CMakeLists.txt
├── README.md
├── LICENSE
├── .gitignore
├── docs/
│   ├── architecture.md
│   ├── development.md
│   ├── testing.md
│   ├── configuration.md
│   └── roadmap.md
├── include/
│   ├── Camera.hpp
│   ├── AsciiConverter.hpp
│   ├── TerminalRenderer.hpp
│   ├── FrameProcessor.hpp
│   ├── Config.hpp
│   └── Application.hpp
├── src/
│   ├── main.cpp
│   ├── Camera.cpp
│   ├── AsciiConverter.cpp
│   ├── TerminalRenderer.cpp
│   ├── FrameProcessor.cpp
│   ├── Config.cpp
│   └── Application.cpp
├── tests/
│   ├── CMakeLists.txt
│   ├── test_ascii_converter.cpp
│   ├── test_frame_processor.cpp
│   └── test_configuration.cpp
├── assets/
│   └── examples/
└── build/
```

## Coding Standards

- C++17 features preferred
- RAII for resource management
- No raw owning pointers
- `const` correctness
- Explicit ownership semantics
- Header guards with `#pragma once`
- Doxygen-style comments for public APIs

## Adding a New Filter

1. Add enum value to `FilterType` in `FrameProcessor.hpp`
2. Declare private method in `FrameProcessor.hpp`
3. Implement method in `FrameProcessor.cpp`
4. Add case in `process()` switch
5. Add test in `test_frame_processor.cpp`

## Adding a New Renderer

1. Create new renderer header in `include/`
2. Implement in `src/`
3. Update `Application` to use new renderer
4. Add configuration option in `Config`

## Debugging

Enable debug build:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Run with GDB:
```bash
gdb ./build/ascii_mirror
```

## Profiling

Use `perf` on Linux or Visual Studio Profiler on Windows to identify bottlenecks in:
- Frame capture
- Image processing
- ASCII conversion
- Terminal output