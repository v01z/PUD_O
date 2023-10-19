cmake_minimum_required(VERSION 3.26)
project(pudo)

if(NOT LINUX)
        message(FATAL_ERROR "You can not do this at all, CMake will exit.")
else()
        find_program(LSB_RELEASE_EXEC lsb_release)
        execute_process(COMMAND ${LSB_RELEASE_EXEC} -is
        OUTPUT_VARIABLE LSB_RELEASE_ID
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    # TOD0: What if sys-apps/lsb-release doesnt installed?
    if(NOT LSB_RELEASE_ID STREQUAL "Gentoo")
        message(FATAL_ERROR "You should have got Gentoo Linux, CMake will exit.")
    else()
        set(CMAKE_CXX_STANDARD 20)
        set(APP_NAME pudo)
        include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)
        file(GLOB ALL_SRCS
            "${PROJECT_SOURCE_DIR}/include/*.h"
            "${PROJECT_SOURCE_DIR}/src/*.cpp")
        add_executable(${APP_NAME} ${ALL_SRCS})
        install(TARGETS ${APP_NAME})
    endif()
endif()
