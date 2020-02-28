# Locate AVCODEC library
# This module defines
# AVCODEC_LIBRARY, the name of the library to link against
# AVCODEC_FOUND, if false, do not try to link to AVCODEC
# AVCODEC_INCLUDE_DIR, where to find avcodec.h

set( AVCODEC_FOUND "NO" )

find_path( AVCODEC_INCLUDE_DIR libavcodec/avcodec.h
  HINTS
  $ENV{AVCODECDIR}
  PATH_SUFFIXES include 
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include 
  /opt/include
  /mingw/include
)

find_library( AVCODEC_LIBRARY
  NAMES avcodec
  HINTS
  $ENV{AVCODECDIR}
  PATH_SUFFIXES lib64 lib bin
  PATHS
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  /mingw
)

set( AVCODEC_FOUND "YES" )
