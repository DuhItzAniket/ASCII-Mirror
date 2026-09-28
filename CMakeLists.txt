cmake_minimum_required(VERSION 3.16)
project(ASCII_Mirror VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Find OpenCV
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(OPENCV opencv5)
endif()

if(NOT OPENCV_FOUND)
    find_path(OPENCV_INCLUDE_DIR opencv2/core.hpp
        PATHS
        $ENV{OPENCV_DIR}/include
        /usr/local/include
        /usr/include
        C:/msys64/ucrt64/include/opencv5
        C:/opencv/include
    )
    find_library(OPENCV_CORE_LIB opencv_core
        PATHS
        $ENV{OPENCV_DIR}/lib
        /usr/local/lib
        /usr/lib
        C:/msys64/ucrt64/lib
        C:/opencv/lib
    )
    if(OPENCV_INCLUDE_DIR AND OPENCV_CORE_LIB)
        set(OPENCV_FOUND TRUE)
        get_filename_component(OPENCV_LIB_DIR ${OPENCV_CORE_LIB} DIRECTORY)
        file(GLOB OPENCV_LIBS ${OPENCV_LIB_DIR}/opencv*.lib ${OPENCV_LIB_DIR}/libopencv*.dll.a)
    endif()
endif()

if(NOT OPENCV_FOUND)
    message(FATAL_ERROR "OpenCV not found. Please install OpenCV or set OPENCV_DIR environment variable.")
endif()

include_directories(${OPENCV_INCLUDE_DIRS} ${OPENCV_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(ascii_mirror
    src/main.cpp
    src/Camera.cpp
    src/AsciiConverter.cpp
    src/TerminalRenderer.cpp
    src/FrameProcessor.cpp
    src/Config.cpp
    src/Application.cpp
)

target_link_libraries(ascii_mirror ${OPENCV_LIBS} ${OPENCV_LIBRARIES})

if(OPENCV_LIB_DIR)
    target_link_directories(ascii_mirror PRIVATE ${OPENCV_LIB_DIR})
endif()

# Tests
enable_testing()
add_subdirectory(tests)

# Installation
install(TARGETS ascii_mirror DESTINATION bin)