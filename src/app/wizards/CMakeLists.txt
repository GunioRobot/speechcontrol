cmake_minimum_required(VERSION 2.8)

file(GLOB SPCHCNTRL_WIZARDS_CPP "*.cpp")
file(GLOB SPCHCNTRL_WIZARDS_HPP "*.hpp")

QT4_WRAP_CPP(SPCHCNTRL_WIZARDS_CPP ${SPCHCNTRL_WIZARDS_HPP})

list(APPEND SPCHCNTRL_APP_SCPP ${SPCHCNTRL_WIZARDS_CPP})

add_subdirectory(backup)
add_subdirectory(contents)
add_subdirectory(micsetup)
add_subdirectory(quickstart)
add_subdirectory(session-create)
add_subdirectory(voxforge)

install(FILES ${SPCHCNTRL_WIZARDS_HPP}
    DESTINATION "${SPCHCNTRL_INCLUDE_DIR}/app/wizards"
    COMPONENT "dev")
