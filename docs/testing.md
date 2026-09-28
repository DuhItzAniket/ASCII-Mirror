# Testing Guide

## Test Organization

Tests are organized by component:

- `test_ascii_converter.cpp` - ASCII conversion logic
- `test_frame_processor.cpp` - Image filter processing
- `test_configuration.cpp` - Config parsing and serialization

## Running Tests

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Or run individual test executables:
```bash
./build/test_ascii_converter
./build/test_frame_processor
./build/test_configuration
```

## Test Categories

### Unit Tests
- Deterministic, no hardware dependencies
- Test pure logic: ASCII mapping, filter algorithms, config parsing
- Run in CI/CD pipeline

### Integration Tests
- Test component interactions
- Require OpenCV but no camera hardware
- Run in CI/CD with virtual display

### Hardware Tests
- Require physical webcam
- Manual execution only
- Not automated in CI

### Manual Tests
- Visual verification of ASCII output
- Keyboard control testing
- Terminal rendering quality

## Adding Tests

1. Create test file in `tests/`
2. Add to `tests/CMakeLists.txt`
3. Follow naming convention: `test_<component>.cpp`
4. Use assertions, no test framework dependency

## Test Data

- Synthetic images generated in code (gradients, solid colors, patterns)
- No external test image files needed
- Deterministic results for reliable CI

## Coverage Goals

- Core logic: >90% coverage
- ASCII mapping: 100% (critical path)
- Filter algorithms: >80%
- Config parsing: >80%