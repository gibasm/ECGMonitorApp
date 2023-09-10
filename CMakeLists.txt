cmake_minimum_required(VERSION 3.20)

project(ECGMonitorApp C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_NAME ecgm_app)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/ipc_server.cc
    ${CMAKE_SOURCE_DIR}/ipc_sockets/ipc_unix_socket.cc
    ${CMAKE_SOURCE_DIR}/ipc_proto/ipc_proto.cc
    ${CMAKE_SOURCE_DIR}/src/renderer.cc
    ${CMAKE_SOURCE_DIR}/src/render_context.cc
    ${CMAKE_SOURCE_DIR}/src/lineplot.cc)

add_executable(${TARGET_NAME} ${SOURCES} ${CMAKE_SOURCE_DIR}/src/main.cc)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(${TARGET_NAME} PRIVATE -Werror -Wall -Wextra -pedantic -fdiagnostics-color=always)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(${TARGET_NAME} PRIVATE -Weverything -fcolor-diagnostics)
endif()

target_include_directories(${TARGET_NAME} PUBLIC include)
target_include_directories(${TARGET_NAME} PUBLIC ipc_proto)
target_include_directories(${TARGET_NAME} PUBLIC ipc_sockets)

target_link_libraries(${TARGET_NAME} PRIVATE ${SDL2_LIBRARIES})

if(${BUILD_WITH_TESTS} STREQUAL "ON")
    add_subdirectory(${CMAKE_SOURCE_DIR}/tests)   
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/services)
