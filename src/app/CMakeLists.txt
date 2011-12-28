cmake_minimum_required(VERSION 2.8)

file(GLOB SPCHCNTRL_APP_SPP "*.cpp"
                                "*/*.cpp"
                                "*/*/*.cpp")
file(GLOB SPCHCNTRL_APP_HPP "*.hpp"
                                "*/*.hpp"
                                "*/*/*.hpp")

QT4_WRAP_UI(SPCHCNTRL_APP_HPP ${SPCHCNTRL_UIFORMS})
QT4_ADD_RESOURCES(SPCHCNTRL_APP_SPP ${SPCHCNTRL_UIQRESC})
QT4_WRAP_CPP(SPCHCNTRL_APP_SPP ${SPCHCNTRL_APP_HPP})

include_directories("../lib")
include_directories(".")
include_directories("${PROJECT_BINARY_DIR}/src/app")

add_executable(speechcontrol ${SPCHCNTRL_APP_SPP})
target_link_libraries(speechcontrol ${SPCHCNTRL_LIBRARIES} spchcntrl)

install(TARGETS speechcontrol
    DESTINATION "${SPCHCNTRL_BIN_DIR}")

## @todo Install the headers needed to build 'plug-ins' to SpeechControl.
#install(FILES ${SPCHCNTRL_APP_HPP}
#    DESTINATION "${SPCHCNTRL_INCLUDE_DIR}/app"
#    COMPONENT "dev")
