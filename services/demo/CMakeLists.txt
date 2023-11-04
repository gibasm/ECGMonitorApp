set(DEMO_SOURCES
	${CMAKE_SOURCE_DIR}/src/arg_parser.cc
	${CMAKE_SOURCE_DIR}/src/log.cc
	${CMAKE_SOURCE_DIR}/src/ipc_server.cc
	${CMAKE_SOURCE_DIR}/ipc_sockets/ipc_unix_socket.cc
	${CMAKE_SOURCE_DIR}/ipc_proto/ipc_proto.cc
	${CMAKE_CURRENT_SOURCE_DIR}/demo.cc)

set(DEMO_TARGET_NAME ecgmd_demo)

set(DEMO_INCLUDE_DIRS
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/ipc_proto
	${CMAKE_SOURCE_DIR}/ipc_sockets)

add_executable(${DEMO_TARGET_NAME} ${DEMO_SOURCES})

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_compile_options(${DEMO_TARGET_NAME} PRIVATE -Werror -Wall -Wextra -pedantic -fdiagnostics-color=always)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_compile_options(${DEMO_TARGET_NAME} PRIVATE -Weverything -fcolor-diagnostics)
endif()

target_include_directories(${DEMO_TARGET_NAME} PUBLIC ${DEMO_INCLUDE_DIRS})

