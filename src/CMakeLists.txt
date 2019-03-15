link_directories( ${GTKMM_LIBRARY_DIRS} )

include_directories( ${GTKMM_INCLUDE_DIRS} )

# Add the executables
add_executable(mtrack main.cpp render/cmanager.cpp ui/main_window.cpp)

# Link the target to the GTK+ libraries
target_link_libraries(mtrack ${OPENGL_LIBRARIES})
target_link_libraries(mtrack ${GTKMM_LIBRARIES})

# Only include VideoIO and core at the moment
target_link_libraries(mtrack opencv_core opencv_videoio)