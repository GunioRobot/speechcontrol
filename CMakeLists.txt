project(SpeechControl)
cmake_minimum_required(VERSION 2.8)

## required packages
find_package(PkgConfig REQUIRED)
find_package(Boost 1.46 REQUIRED)
find_package(Qt4 4.7.4 COMPONENTS
  QtCore
  QtXml
  QtGui
  REQUIRED)
find_package(QtGStreamer REQUIRED)
include("${QT_USE_FILE}")
pkg_search_module(POCKETSPHINX pocketsphinx)

## variables
set(SPCHCNTRL_INCLUDE_DIRS "${QT_INCLUDE_DIR}"
    ${POCKETSPHINX_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${QTGSTREAMER_INCLUDES})
set(SPCHCNTRL_LIBRARIES "${QT_LIBRARIES}"
    ${PHONON_LIBRARIES}
    ${Boost_LIBRARIES}
    ${QTGLIB_LIBRARIES}
    ${QTGSTREAMER_LIBRARIES}
    ${QTGSTREAMER_UI_LIBRARIES}
    ${QTGSTREAMER_UTILS_LIBRARIES}
    ${POCKETSPHINX_LIBRARIES} )
set(SPCHCNTRL_CMAKE_SDIR "${PROJECT_SOURCE_DIR}/cmake")
set(SPCHCNTRL_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/SpeechControl")
set(SPCHCNTRL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(SPCHCNTRL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")

## done here to pick up the files in Qt Creator.
file(GLOB SPCHCNTRL_UIQRESC "ui/*.qrc")
file(GLOB SPCHCNTRL_UIFORMS "ui/*/*.ui"
                            "ui/*/*/*.ui")


## includes
include_directories(${SPCHCNTRL_INCLUDE_DIRS})

## sub-projects
add_subdirectory(src)
