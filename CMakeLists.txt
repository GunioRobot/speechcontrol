project(SpeechControl)
cmake_minimum_required(VERSION 2.8)

file(GLOB SPCHCNTRL_SOURCES "src/*.cpp")
file(GLOB SPCHCNTRL_HEADERS "src/*.hpp")
file(GLOB SPCHCNTRL_UIFORMS "ui/*.ui")
file(GLOB SPCHCNTRL_UIQRESC "ui/*.qrc")

find_package(Qt4 4.7.4 COMPONENTS
  QtCore
  QtXml
  QtGui
  REQUIRED)

include("${QT_USE_FILE}")

QT4_WRAP_UI(SPCHCNTRL_HEADERS "${SPCHCNTRL_UIFORMS}")
QT4_WRAP_CPP(SPCHCNTRL_SOURCES "${SPCHCNTRL_HEADERS}")
QT4_ADD_RESOURCES(SPCHCNTRL_SOURCES "${SPCHCNTRL_UIQRESC}")

set(SPCHCNTRL_INCLUDE_DIRS "${QT_INCLUDE_DIRS}")
set(SPCHCNTRL_LIBRARIES "${QT_LIBRARIES}")

include_directories(${SPCHCNTRL_INCLUDE_DIRS})
include_directories(${PROJECT_BINARY_DIR})

add_executable(speechcontrol ${SPCHCNTRL_SOURCES})
target_link_libraries(speechcontrol ${QT_LIBRARIES})
