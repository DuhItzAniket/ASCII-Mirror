# ASCII Mirror

Real-time webcam-to-ASCII vision engine written in modern C++ using OpenCV.

## Features

- Real-time webcam capture using OpenCV
- High-quality ASCII conversion with configurable character ramps
- Efficient terminal rendering using ANSI escape sequences (no flickering)
- Multiple visual modes: grayscale, invert, threshold, edge detection, blur
- Configurable ASCII resolution and aspect correction
- Runtime keyboard controls
- Cross-platform (Windows, Linux)
- Modular, testable architecture

## Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 19+)
- CMake 3.16+
- OpenCV 4.x or 5.x

## Building

### Windows (MSYS2/MinGW)

```bash
# Install dependencies
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-opencv

# Build
cmake -S . -B build
cmake --build build
```

### Linux

```bash
# Ubuntu/Debian
sudo apt install g++ cmake libopencv-dev

# Build
cmake -S . -B build
cmake --build build
```

## Usage

```bash
./build/ascii_mirror [options]
```

### Options

```
-h, --help           Show help message
-c, --camera <idx>   Camera index (default: 0)
-w, --width <cols>   ASCII width in characters (default: 120)
--charset <chars>    ASCII character ramp (default: @%#*+=-:. )
--invert             Invert brightness mapping
--no-fps             Disable FPS display
--color              Enable color output (experimental)
--filter <mode>      Filter mode: grayscale, invert, threshold, edge, blur
--config <file>      Load configuration from file
```

### Runtime Controls

```
Q, ESC    Quit
+, =      Increase ASCII width
-         Decrease ASCII width
1         Grayscale filter
2         Invert filter
3         Threshold filter
4         Edge filter
5         Blur filter
0         No filter
```

## Configuration

Create a config file (`config.cfg`):

```ini
# ASCII Mirror Configuration
camera_index=0
ascii_width=120
charset=@%#*+=-:. 
invert=false
show_fps=true
color_enabled=false
filter_mode=grayscale
aspect_correction=0.5
target_fps=30
```

Load with `--config config.cfg`.

## Architecture

```
┌─────────────┐     ┌──────────────┐     ┌────────────────┐     ┌─────────────────┐     ┌────────────────┐
│   Camera    │────▶│FrameProcessor│────▶│ AsciiConverter │────▶│TerminalRenderer │────▶│   Terminal     │
└─────────────┘     └──────────────┘     └────────────────┘     └─────────────────┘     └────────────────┘
```

### Components

- **Camera**: Webcam capture and frame acquisition
- **FrameProcessor**: Image filters (grayscale, invert, threshold, edge, blur)
- **AsciiConverter**: Brightness-to-ASCII mapping with aspect correction
- **TerminalRenderer**: ANSI-based terminal rendering
- **Config**: Runtime configuration management
- **Application**: Main orchestration loop

## Testing

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

## License

MIT License - see LICENSE file for details.