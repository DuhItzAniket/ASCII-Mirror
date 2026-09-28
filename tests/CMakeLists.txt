cmake_minimum_required(VERSION 3.16)

enable_testing()

# Create a library from the main sources for testing
add_library(ascii_mirror_lib
    ../src/Camera.cpp
    ../src/AsciiConverter.cpp
    ../src/TerminalRenderer.cpp
    ../src/FrameProcessor.cpp
    ../src/Config.cpp
    ../src/Application.cpp
)
target_include_directories(ascii_mirror_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(ascii_mirror_lib ${OPENCV_LIBS} ${OPENCV_LIBRARIES})

add_executable(test_ascii_converter test_ascii_converter.cpp)
target_link_libraries(test_ascii_converter ascii_mirror_lib)
add_test(NAME ascii_converter_test COMMAND test_ascii_converter)

add_executable(test_frame_processor test_frame_processor.cpp)
target_link_libraries(test_frame_processor ascii_mirror_lib)
add_test(NAME frame_processor_test COMMAND test_frame_processor)

add_executable(test_configuration test_configuration.cpp)
target_link_libraries(test_configuration ascii_mirror_lib)
add_test(NAME configuration_test COMMAND test_configuration)