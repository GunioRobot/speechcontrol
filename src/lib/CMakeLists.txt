cmake_minimum_required(VERSION 2.8)

## variables
set(MARYTTSQT_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/speechcontrol")
set(MARYTTSQT_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(MARYTTSQT_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/SpeechControl")
set(SPCHCNTRL_LIB_INCLUDE_DIRS "${SPCHNCTRL_INCLUDE_DIRS}")
set(SPCHCNTRL_LIB_LIBRARIES "${SPCHCNTRL_LIBRARIES}")

## setting up sources
file(GLOB SPCHCNTRL_LIB_SOURCES "*.cpp")
file(GLOB SPCHCNTRL_LIB_HEADERS "*.hpp")
QT4_WRAP_CPP(SPCHCNTRL_LIB_SOURCES ${SPCHCNTRL_LIB_HEADERS})

## configures
configure_file("${SPCHCNTRL_CMAKE_SDIR}/SpeechControlConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlConfig.cmake" @ONLY)

configure_file("${SPCHCNTRL_CMAKE_SDIR}/SpeechControlConfigVersion.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlConfigVersion.cmake" @ONLY)

add_library(spchcntrl SHARED ${SPCHCNTRL_LIB_SOURCES})

target_link_libraries(spchcntrl ${SPCHCNTRL_LIB_LIBRARIES})

install(TARGETS spchcntrl
    EXPORT SpeechControlLibraryDepends
    LIBRARY DESTINATION "${SPCHCNTRL_LIB_DIR}")

install(EXPORT SpeechControlLibraryDepends
    DESTINATION "${SPCHCNTRL_CMAKE_DIR}")

install(FILES ${SPCHCNTRL_LIB_HPP}
    DESTINATION "${SPCHCNTRL_INCLUDE_DIR}")

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlConfig.cmake"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlConfigVersion.cmake"
    DESTINATION "${MARYTTSQT_CMAKE_DIR}")
