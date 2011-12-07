project(SpeechControlApplication)
cmake_minimum_required(VERSION 2.8)

find_package(Qt4 4.7.4 COMPONENTS
  QtGui
  QtCore
  QtXml
  QtNetwork
  REQUIRED)

include("${QT_USE_FILE}")

set(PROJECT_LABEL "SpeechControl")
set(SPCHCNTRL_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(SPCHCNTRL_LIBRARY_DIRS "${Qt_INCLUDE_DIRS}")
set(SPCHCNTRL_LIBRARIES "${Qt_LIBRARIES}")

include_directories(src)
add_subdirectory(src)
