project(SpeechControl)
cmake_minimum_required(VERSION 2.8)

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

set(SPCHCNTRL_INCLUDE_DIRS "${QT_INCLUDE_DIRS}"
    ${POCKETSPHINX_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${QTGSTREAMER_INCLUDE_DIR}
    ${QTGSTREAMER_INCLUDES}
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_BINARY_DIR}")
set(SPCHCNTRL_LIBRARIES "${QT_LIBRARIES}"
    ${PHONON_LIBRARIES}
    ${Boost_LIBRARIES}
    ${QTGLIB_LIBRARIES}
    ${QTGSTREAMER_LIBRARIES}
    ${QTGSTREAMER_UI_LIBRARIES}
    ${QTGSTREAMER_UTILS_LIBRARIES}
    ${POCKETSPHINX_LIBRARIES})

include_directories(${SPCHCNTRL_INCLUDE_DIRS})

file(GLOB SPCHCNTRL_UIQRESC "ui/*.qrc")
file(GLOB SPCHCNTRL_SOURCES "src/*.cpp"
                            "src/*/*.cpp"
                            "src/*/*/*.cpp")
file(GLOB SPCHCNTRL_HEADERS "src/*.hpp"
                            "src/*/*.hpp"
                            "src/*/*/*.hpp")
file(GLOB SPCHCNTRL_UIFORMS "ui/*/*.ui"
                            "ui/*/*/*.ui")

QT4_WRAP_UI(SPCHCNTRL_HEADERS ${SPCHCNTRL_UIFORMS})
QT4_ADD_RESOURCES(SPCHCNTRL_SOURCES ${SPCHCNTRL_UIQRESC})
QT4_WRAP_CPP(SPCHCNTRL_SOURCES ${SPCHCNTRL_HEADERS})

add_executable(speechcontrol ${SPCHCNTRL_SOURCES})
target_link_libraries(speechcontrol ${SPCHCNTRL_LIBRARIES})
