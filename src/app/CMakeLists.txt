cmake_minimum_required(VERSION 2.8)

file(GLOB SPCHCNTRL_APP_SOURCES "*.cpp"
                                "*/*.cpp")
file(GLOB SPCHCNTRL_APP_HEADERS "*.hpp"
                                "*/*.hpp")

QT4_WRAP_CPP(SPCHCNTRL_APP_SOURCES ${SPCHCNTRL_APP_HEADERS})

include_directories("../lib")
include_directories(".")
include_directories("${PROJECT_BINARY_DIR}/src/app")

add_executable(speechcontrol ${SPCHCNTRL_APP_SOURCES})
target_link_libraries(speechcontrol ${SPCHCNTRL_LIBRARIES} spchcntrl)

install(TARGETS speechcontrol
    DESTINATION "${SPCHCNTRL_BIN_DIR}")
