# Roadmap

## Phase 0: Repository Setup ✓
- [x] Initialize Git repository
- [x] Create project structure
- [x] Configure CMake with OpenCV
- [x] Create .gitignore
- [x] Create README, LICENSE

## Phase 1: Webcam Capture ✓
- [x] Camera class with open/read/release
- [x] Camera index configuration
- [x] Error handling
- [x] Resource cleanup (RAII)

## Phase 2: ASCII Conversion ✓
- [x] BGR → Grayscale conversion
- [x] Aspect-ratio-aware resizing
- [x] Brightness mapping with configurable charset
- [x] Unit tests with synthetic images

## Phase 3: Terminal Renderer ✓
- [x] ANSI cursor positioning
- [x] Frame replacement (no scrolling)
- [x] Windows ANSI support
- [x] Clean shutdown

## Phase 4: Pipeline Integration ✓
- [x] Main application loop
- [x] Keyboard controls
- [x] FPS display
- [x] Graceful exit

## Phase 5: Configuration ✓
- [x] Config file support
- [x] Command-line arguments
- [x] Runtime parameter adjustment

## Phase 6: Performance Optimization
- [ ] Buffer reuse for frames
- [ ] String capacity reservation
- [ ] OpenCV operation optimization
- [ ] Benchmark measurements
- [ ] Optional multithreading (producer-consumer)

## Phase 7: Visual Modes ✓
- [x] Standard ASCII
- [x] Dense ASCII
- [x] Inverted
- [x] Block characters
- [x] Edge detection
- [x] Runtime mode switching

## Phase 8: Color Rendering
- [ ] ANSI 256-color grayscale
- [ ] True color RGB approximation
- [ ] Color mode toggle
- [ ] Graceful fallback for non-color terminals

## Phase 9: Error Handling
- [ ] Camera disconnect handling
- [ ] Invalid frame recovery
- [ ] Terminal resize handling
- [ ] Configuration validation
- [ ] Resource cleanup on exceptions

## Phase 10: Automated Testing ✓
- [x] Unit tests for AsciiConverter
- [x] Unit tests for FrameProcessor
- [x] Unit tests for Config
- [x] CTest integration

## Phase 11: Code Quality
- [ ] Clang-tidy integration
- [ ] Static analysis
- [ ] Warning cleanup
- [ ] Code formatting (clang-format)

## Phase 12: Documentation ✓
- [x] README
- [x] Architecture docs
- [x] Development guide
- [x] Testing guide
- [x] Configuration reference

## Future Enhancements

- Video file input support
- GPU acceleration (OpenCL/CUDA)
- Unicode/wide character support
- Adaptive terminal resolution
- Mouse interaction (ROI selection)
- Recording/playback
- Web streaming output
- Plugin system for filters
- Mobile/embedded support