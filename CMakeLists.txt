# Set the minimum version of cmake required to build this project	
cmake_minimum_required(VERSION 2.8)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(mtrack)

# Use the package PkgConfig to detect GTK+ headers/library files
# Also import OpenCV to use for vision
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)
find_package(GLEW 2.0 REQUIRED)

pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${GTKMM_LIBRARY_DIRS})

# Add the executables
add_executable(mtrack 
    src/main.cpp
    src/render/cmanager.cpp
    src/ui/main_window.cpp
    src/project/project.cpp
    src/project/project_item.cpp
    src/models/projectitem_model.cpp)

# Link the target to the GTK+ libraries
target_link_libraries(mtrack ${OPENGL_LIBRARIES})
target_link_libraries(mtrack GLEW::GLEW)
target_link_libraries(mtrack ${GTKMM_LIBRARIES})

# Only include VideoIO and core at the moment
target_link_libraries(mtrack opencv_core opencv_videoio)

# Copy the resources to a resource folder in the build directory
add_custom_command(TARGET mtrack PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:mtrack>/res)
