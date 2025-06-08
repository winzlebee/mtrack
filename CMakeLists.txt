# Set the minimum version of cmake required to build this project
cmake_minimum_required(VERSION 3.18)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/res/cmake)
project(mtrack)

find_package(PkgConfig REQUIRED)

# Use the package PkgConfig to detect GTK+ headers/library files
# Also import OpenCV to use for vision
find_package(OpenGL REQUIRED)
find_package(GLEW 2.0 REQUIRED)
find_package(avutil REQUIRED)
find_package(avcodec REQUIRED)
find_package(avformat REQUIRED)
find_package(swscale REQUIRED)

pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${AVUTIL_INCLUDE_DIR})
include_directories(${AVFORMAT_INCLUDE_DIR})
include_directories(${AVCODEC_INCLUDE_DIR})
include_directories(${SWSCALE_INCLUDE_DIR})

link_directories(${GTKMM_LIBRARY_DIRS})

# Add the executables
add_executable(mtrack
    src/main.cpp
    src/commands/undo_command.cpp
    src/commands/import_clip_command.cpp
    src/render/context_manager.cpp
    src/ui/main_window.cpp
    src/ui/timeline_widget.cpp
    src/project/project.cpp
    src/project/project_item.cpp
    src/project/ffmpeg_video_clip.cpp
    src/models/projectitem_model.cpp
	src/playback/playback_manager.cpp
	src/playback/playback_source.cpp
	)

# Link the target to the GTK+ libraries
target_link_libraries(mtrack ${OPENGL_LIBRARIES})
target_link_libraries(mtrack GLEW::GLEW)
target_link_libraries(mtrack ${GTKMM_LIBRARIES})
target_link_libraries(mtrack ${AVUTIL_LIBRARY} ${AVFORMAT_LIBRARY} ${AVCODEC_LIBRARY} ${SWSCALE_LIBRARY})

# Only include VideoIO and core at the moment
# target_link_libraries(mtrack 1_core opencv_videoio opencv_imgproc)

# Copy the resources to a resource folder in the build directory
add_custom_command(TARGET mtrack PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E remove_directory
                       $<TARGET_FILE_DIR:mtrack>/res)

add_custom_command(TARGET mtrack PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:mtrack>/res)
