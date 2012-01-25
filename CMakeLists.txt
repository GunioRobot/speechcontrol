project(SpeechControl)
cmake_minimum_required(VERSION 2.8)

## required packages
find_package(PkgConfig REQUIRED)
find_package(Boost 1.4.6 COMPONENTS
  python
  REQUIRED)
find_package(PythonLibs 2.7 REQUIRED)
find_package(Qt4 4.7.4 COMPONENTS
  QtCore
  QtXml
  QtGui
  REQUIRED)
find_package(QtGStreamer REQUIRED)
pkg_search_module(POCKETSPHINX REQUIRED pocketsphinx sphinxbase)

include("${QT_USE_FILE}")

## variables
set(SPCHCNTRL_INCLUDE_DIRS "${QT_INCLUDE_DIR}"
    ${POCKETSPHINX_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${QTGSTREAMER_INCLUDES})
set(SPCHCNTRL_LIBRARIES "${QT_LIBRARIES}"
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARIES}
    ${PHONON_LIBRARIES}
    ${QTGLIB_LIBRARIES}
    ${QTGSTREAMER_LIBRARIES}
    ${QTGSTREAMER_UI_LIBRARIES}
    ${QTGSTREAMER_UTILS_LIBRARIES}
    ${POCKETSPHINX_LIBRARIES})
set(SPCHCNTRL_CMAKE_SDIR "${PROJECT_SOURCE_DIR}/cmake")
set(SPCHCNTRL_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/SpeechControl")
set(SPCHCNTRL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(SPCHCNTRL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(SPCHCNTRL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/spchcntrl")
ADD_DEFINITIONS("-Wall")

if(CMAKE_BUILD_TYPE EQUAL "Debug")
    set(Boost_USE_DEBUG_PYTHON ON)
endif()

## done here to pick up the files in Qt Creator.
file(GLOB SPCHCNTRL_UIQRESC "ui/*.qrc")
file(GLOB SPCHCNTRL_UIFORMS "ui/*/*.ui"
                            "ui/*/*/*.ui"
                            "ui/*/*/*/*.ui")


## includes
include_directories(${SPCHCNTRL_INCLUDE_DIRS})

## sub-projects
add_subdirectory(src)
