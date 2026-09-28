# Configuration Reference

## Configuration File Format

Simple key=value format with `#` comments:

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

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `camera_index` | int | 0 | Webcam device index |
| `ascii_width` | int | 120 | Output width in characters |
| `charset` | string | `@%#*+=-:. ` | Character ramp (dark to light) |
| `invert` | bool | false | Invert brightness mapping |
| `show_fps` | bool | true | Display FPS counter |
| `color_enabled` | bool | false | Enable ANSI color (experimental) |
| `filter_mode` | string | `grayscale` | Filter: grayscale, invert, threshold, edge, blur |
| `aspect_correction` | float | 0.5 | Terminal character aspect ratio correction |
| `target_fps` | int | 30 | Target frame rate |

## Command Line Arguments

All config parameters available as CLI args:

```bash
ascii_mirror -c 1 -w 80 --invert --filter edge
```

## Character Ramps

### Standard (default)
```
@%#*+=-:. 
```

### Dense
```
$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'.
```

### Blocks
```
█▓▒░ 
```

### Minimal
```
@# 
```

## Environment Variables

- `OPENCV_DIR` - OpenCV installation directory (for CMake find_package)

## Platform Notes

### Windows
- ANSI escape sequences enabled via `ENABLE_VIRTUAL_TERMINAL_PROCESSING`
- Terminal size detected via Windows Console API

### Linux
- ANSI sequences natively supported
- Terminal size via `ioctl(TIOCGWINSZ)`