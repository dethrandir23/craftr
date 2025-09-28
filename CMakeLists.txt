cmake_minimum_required(VERSION 3.15)

project(craftr LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Sources
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(craftr ${SOURCES})

# Include
target_include_directories(craftr PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Binary output
set_target_properties(craftr PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
add_compile_definitions(PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

# Warnings
if(MSVC)
    target_compile_options(craftr PRIVATE /W4 /permissive-)
else()
    target_compile_options(craftr PRIVATE -Wall -Wextra -pedantic -Wshadow)
endif()

set(YAML_CPP_BUILD_TESTS OFF)
set(YAML_CPP_BUILD_CONTRIB OFF)
add_subdirectory(libs/yaml-cpp)

add_subdirectory(libs/localita)

target_link_libraries(craftr PRIVATE yaml-cpp localita)
