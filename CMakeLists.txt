# Set the minimum version of cmake required to build this project
cmake_minimum_required(VERSION 2.6)
project(mtrack)

# Use the package PkgConfig to detect GTK+ headers/library files
# Also import OpenCV to use for vision
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})
# Add an executable compiled from hello.c
add_executable(mtrack main.cpp)
# Link the target to the GTK+ libraries
target_link_libraries(mtrack ${OPENGL_LIBRARIES})
target_link_libraries(mtrack ${GTK3_LIBRARIES})

# TODO: Link specific openCV libraries we want to use
# target_link_libraries(mtrack ${OpenCV_LIBS})
