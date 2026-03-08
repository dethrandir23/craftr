cmake_minimum_required(VERSION 3.15)

project(craftr LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${CMAKE_SOURCE_DIR}/postinstall.sh")


if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation directory" FORCE)
endif()

# Sources
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(craftr ${SOURCES})

# Include directories
target_include_directories(craftr PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/libs
)
set_target_properties(craftr PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

add_compile_definitions(PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

# DATA_DIR compile-time definition
if(UNIX AND NOT APPLE)
    set(DATA_DIR "\"${CMAKE_INSTALL_PREFIX}/share/craftr\"")
elseif(APPLE)
    set(DATA_DIR "\"${CMAKE_INSTALL_PREFIX}/Resources\"")
elseif(WIN32)
    set(DATA_DIR "\"$ENV{ProgramFiles}\\\\craftr\\\\data\"")
endif()
add_compile_definitions(DATA_DIR=${DATA_DIR})

# Warnings
if(MSVC)
    target_compile_options(craftr PRIVATE /W4 /permissive-)
else()
    target_compile_options(craftr PRIVATE -Wall -Wextra -pedantic -Wshadow)
endif()

# Libraries
set(YAML_CPP_BUILD_TESTS OFF)
set(YAML_CPP_BUILD_CONTRIB OFF)
add_subdirectory(libs/yaml-cpp)
add_subdirectory(libs/localita)
target_link_libraries(craftr PRIVATE yaml-cpp localita)

# ================= INSTALLATION =================

# Executable
install(TARGETS craftr
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

if(UNIX AND NOT APPLE)
    install(DIRECTORY data/
        DESTINATION share/craftr
        USE_SOURCE_PERMISSIONS
        PATTERN "libs" EXCLUDE
        PATTERN ".git" EXCLUDE
    )

    if(NOT DEFINED USER_HOME)
        set(USER_HOME $ENV{HOME})
    endif()
    set(USER_DATA_DIR "${USER_HOME}/.local/share/craftr")
    message(STATUS "Installing data files to user directory: ${USER_DATA_DIR}")

    install(DIRECTORY ${CMAKE_SOURCE_DIR}/data/
        DESTINATION ${USER_DATA_DIR}
        USE_SOURCE_PERMISSIONS
        PATTERN "libs" EXCLUDE
        PATTERN ".git" EXCLUDE
    )

elseif(APPLE)
    install(DIRECTORY data/
        DESTINATION Resources
        USE_SOURCE_PERMISSIONS
        PATTERN "libs" EXCLUDE
    )
elseif(WIN32)
    install(DIRECTORY data/
        DESTINATION data
        USE_SOURCE_PERMISSIONS
        PATTERN "libs" EXCLUDE
    )
endif()

# ================= CPACK =================
set(CPACK_PACKAGE_NAME "craftr")
set(CPACK_PACKAGE_VENDOR "Bekir Efe Öztürk <efeoztrk23@proton.me>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A project template generator")
set(CPACK_PACKAGE_VERSION "1.1.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

if(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
    set(CPACK_NSIS_DISPLAY_NAME "Craftr")
    set(CPACK_NSIS_PACKAGE_NAME "Craftr")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "craftr")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop;ZIP")
    set(CPACK_DMG_VOLUME_NAME "Craftr")
else()
    set(CPACK_GENERATOR "DEB;RPM;ZIP")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Bekir Efe Öztürk <efeoztrk23@proton.me>")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.28), libstdc++6 (>= 9)")
endif()

include(CPack)
