cmake_minimum_required(VERSION 2.8)

file(GLOB SPCHCNTRL_WIZARDS_SESSIONCREATE_CPP "*.cpp")
file(GLOB SPCHCNTRL_WIZARDS_SESSIONCREATE_HPP "*.hpp")

QT4_WRAP_CPP(SPCHCNTRL_WIZARDS_SESSIONCREATE_CPP ${SPCHCNTRL_WIZARDS_SESSIONCREATE_HPP})

list(APPEND SPCHCNTRL_APP_SCPP ${SPCHCNTRL_WIZARDS_SESSIONCREATE_CPP})

install(FILES ${SPCHCNTRL_WIZARDS_SESSIONCREATE_HPP}
    DESTINATION "${SPCHCNTRL_INCLUDE_DIR}/app/wizards/session-create"
    COMPONENT "dev")
