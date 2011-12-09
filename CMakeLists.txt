project(SpeechControl)
cmake_minimum_required(VERSION 2.8)

file(GLOB SPCHCNTRL_SOURCES "src/*.cpp")
file(GLOB SPCHCNTRL_HEADERS "src/*.hpp")
file(GLOB SPCHCNTRL_UIFORMS "ui/*.ui")
file(GLOB SPCHCNTRL_UIQRESC "ui/*.qrc")

find_package(Qt4 COMPONENTS
  QtCore
  QtGui REQUIRED)

include("${QT_USE_FILE}")

set(SPCHCNTRL_INCLUDE_DIRS "${QT_INCLUDE_DIR}")
set(SPCHCNTRL_LIBRARIES "${QT_LIBRARIES}")

QT4_WRAP_UI(SPCHCNTRL_HEADERS "${SPCHCNTRL_UIFORMS}")
QT4_WRAP_CPP(SPCHCNTRL_SOURCES "${SPCHCNTRL_HEADERS}")

include_directories("${SPCHCNTRL_INCLUDE_DIRS}")

add_executable(speechcontrol
  ${SPCHCNTRL_SOURCES})