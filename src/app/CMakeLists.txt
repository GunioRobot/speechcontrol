project(SpeechControlApplication)
cmake_minimum_required(VERSION 2.8)

file(GLOB SPCHCNTRL_APP_SCPP "*.cpp")
file(GLOB SPCHCNTRL_APP_SHPP "*.hpp")
file(GLOB SPCHCNTRL_UIQRESC "ui/*.qrc")
file(GLOB SPCHCNTRL_UIFORMS "ui/*/*.ui"
                            "ui/*/*/*.ui"
                            "ui/*/*/*/*.ui")

QT4_WRAP_UI(SPCHCNTRL_APP_SCPP ${SPCHCNTRL_UIFORMS})
QT4_ADD_RESOURCES(SPCHCNTRL_APP_SCPP ${SPCHCNTRL_UIQRESC})
QT4_WRAP_CPP(SPCHCNTRL_APP_SCPP ${SPCHCNTRL_APP_SHPP} OPTIONS
    -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)

include_directories("${PROJECT_SOURCE_DIR}/../li")

add_subdirectory(sessions)
add_subdirectory(plugins)
add_subdirectory(windows)
add_subdirectory(wizards)
add_subdirectory(dictation)
add_subdirectory(desktopcontrol)


configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
               "${PROJECT_SOURCE_DIR}/config.hpp")

add_executable(speechcontrol ${SPCHCNTRL_APP_SCPP})
add_dependencies(speechcontrol spchcntrl)
target_link_libraries(speechcontrol ${SPCHCNTRL_LIBRARIES} spchcntrl)

install(TARGETS speechcontrol
    DESTINATION "${SPCHCNTRL_BIN_DIR}")

install(FILES ${SPCHCNTRL_APP_SHPP}
    DESTINATION "${SPCHCNTRL_INCLUDE_DIR}/app"
    COMPONENT "dev")
