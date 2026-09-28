# Architecture

## Overview

ASCII Mirror follows a modular pipeline architecture:

```
Camera → FrameProcessor → AsciiConverter → TerminalRenderer → Terminal
```

Each component is a separate class with a single responsibility, making the system testable and extensible.

## Component Details

### Camera
- Encapsulates `cv::VideoCapture`
- Handles camera initialization, frame reading, and cleanup
- Supports camera index selection and resolution configuration

### FrameProcessor
- Applies image processing filters
- Supports: Grayscale, Invert, Threshold, Edge (Canny), Blur
- Easily extensible for new filters

### AsciiConverter
- Converts grayscale frames to ASCII strings
- Handles aspect ratio correction for terminal characters
- Configurable character ramp and output width
- Supports brightness inversion

### TerminalRenderer
- Manages terminal state and ANSI escape sequences
- Efficient frame rendering using cursor positioning (no screen clearing)
- Windows-specific ANSI enablement
- Automatic terminal size detection

### Config
- Centralized configuration management
- Supports file-based and command-line configuration
- Simple key=value format

### Application
- Main orchestration class
- Runs the capture-process-render loop
- Handles keyboard input and FPS calculation

## Data Flow

1. `Camera::read()` captures BGR frame from webcam
2. `FrameProcessor::process()` applies selected filter
3. `AsciiConverter::convert()` maps brightness to ASCII characters
4. `TerminalRenderer::render()` outputs to terminal using ANSI codes
5. Loop repeats at target FPS

## Threading

Currently single-threaded. Future optimization may introduce producer-consumer pattern for capture/processing separation.

## Extensibility

Adding new filters:
1. Add enum value to `FilterType`
2. Implement filter method in `FrameProcessor`
3. Add case in `process()` switch

Adding new renderers:
1. Create new renderer class implementing render interface
2. Update `Application` to use new renderer