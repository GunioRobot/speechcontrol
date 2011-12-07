project(SpeechControlApplicationSources)
cmake_minimum_required(VERSION 2.8)

# @todo Add support for translations.

file(GLOB SPCHCNTRL_HEADERS "*.hpp")
file(GLOB SPCHCNTRL_SOURCES "*.cpp")
file(GLOB SPCHCNTRL_UIFILES "../ui/*.ui")
file(GLOB SPCHCNTRL_RESDATA "../ui/*.qrc")

#QT4_ADD_RESOURCES(SPCHCNTRL_SOURCES "${SPCHCNTRL_RESDATA}")
#QT4_WRAP_UI(SPCHCNTRL_SOURCES "${SPCHCNTRL_UIFILES}")
#QT4_WRAP_CPP(SPCHCNTRL_SOURCES "${SPCHCNTRL_HEADERS}")

add_executable(speechcontrol "${SPCHCNTRL_SOURCES}")
target_link_libraries(speechcontrol "${SPCHCNTRL_LIBRARIES}")

install(TARGETS speechcontrol
        RUNTIME DESTINATION "${SPCHCNTRL_INSTALL_DIR}")
