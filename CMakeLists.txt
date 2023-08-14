cmake_minimum_required(VERSION 3.20)

project(ECGMonitorApp C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_NAME ecgm_app)

set(SOURCES
src/main.cc
src/renderer.cc
src/lineplot.cc)

add_executable(${TARGET_NAME} ${SOURCES})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(${TARGET_NAME} PRIVATE -Werror -Wall -Wextra -pedantic -fdiagnostics-color=always)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(${TARGET_NAME} PRIVATE -Weverything -fcolor-diagnostics)
endif()

target_include_directories(${TARGET_NAME} PUBLIC include)
target_link_libraries(${TARGET_NAME} PRIVATE ${SDL2_LIBRARIES})
