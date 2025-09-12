cmake_minimum_required(VERSION 3.15)

# Project name and language
project(craftr LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Executable
add_executable(craftr ${SOURCES})

# Include directories (modern CMake)
target_include_directories(craftr
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# Set output directory for the binary
set_target_properties(craftr PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build
)

add_compile_definitions(PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

find_package(yaml-cpp REQUIRED)
target_link_libraries(craftr PRIVATE yaml-cpp)

# Optional: Enable warnings and extra flags
if(MSVC)
    target_compile_options(craftr PRIVATE /W4 /permissive-)
else()
    target_compile_options(craftr PRIVATE -Wall -Wextra -pedantic -Wshadow)
endif()
