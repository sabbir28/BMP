cmake_minimum_required(VERSION 3.20)
project(BMP C)

set(CMAKE_C_STANDARD 11)

# Include directories
include_directories(include)

# Source files
set(SOURCES 
    src/BMP.c
)

# Windows resources for DLL metadata (SABBIR28)
if(WIN32)
    list(APPEND SOURCES src/Resources.rc)
endif()

# Build library
add_library(BMP SHARED ${SOURCES})
target_compile_definitions(BMP PRIVATE BMP_EXPORTS)

set_target_properties(BMP PROPERTIES
    OUTPUT_NAME "BMP"
    VERSION "1.0.0"
    SOVERSION "1"
)

# Single Supreme Application
add_executable(BMP_Application examples/BMP_Supreme.c)
target_link_libraries(BMP_Application PRIVATE BMP)

# Set runtime output directory to the root for easier file access
set_target_properties(BMP_Application PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set_target_properties(BMP PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
