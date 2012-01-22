cmake_minimum_required(VERSION 2.8)

file(GLOB SPCHCNTRL_APP_SCPP "*.cpp")
file(GLOB SPCHCNTRL_APP_SHPP "*.hpp")

QT4_WRAP_UI(SPCHCNTRL_APP_UICODE ${SPCHCNTRL_UIFORMS})
QT4_ADD_RESOURCES(SPCHCNTRL_APP_RESCODE ${SPCHCNTRL_UIQRESC})
QT4_WRAP_CPP(SPCHCNTRL_APP_SCPP ${SPCHCNTRL_APP_SHPP})

add_subdirectory(plugins)
add_subdirectory(sessions)

include_directories("${PROJECT_SOURCE_DIR}/src/lib"
    "${PROJECT_SOURCE_DIR}/src/app"
    "${PROJECT_BINARY_DIR}/src/app")

add_executable(speechcontrol ${SPCHCNTRL_APP_SCPP}
    ${SPCHCNTRL_APP_UICODE}
    ${SPCHCNTRL_APP_RESCODE})
target_link_libraries(speechcontrol ${SPCHCNTRL_LIBRARIES} spchcntrl)


install(TARGETS speechcontrol
    DESTINATION "${SPCHCNTRL_BIN_DIR}")

## @todo Install the headers needed to build 'plug-ins' to SpeechControl.
#install(FILES ${SPCHCNTRL_APP_SHPP}
#    DESTINATION "${SPCHCNTRL_INCLUDE_DIR}/app"
#    COMPONENT "dev")
