# Set the minimum version of cmake required to build this project	
cmake_minimum_required(VERSION 2.6)
project(mtrack)

# Use the package PkgConfig to detect GTK+ headers/library files
# Also import OpenCV to use for vision
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)

pkg_check_modules(GTKMM gtkmm-3.0)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

# Add the executables
add_executable(mtrack src/main.cpp)

# Link the target to the GTK+ libraries
target_link_libraries(mtrack ${OPENGL_LIBRARIES})
target_link_libraries(mtrack ${GTKMM_LIBRARIES})

# Only include VideoIO and core at the moment
target_link_libraries(mtrack opencv_core opencv_videoio)
