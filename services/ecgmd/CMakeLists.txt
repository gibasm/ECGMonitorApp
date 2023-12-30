set(ECGMD_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/serial.cc
	${CMAKE_CURRENT_SOURCE_DIR}/comm.cc
	${CMAKE_SOURCE_DIR}/src/arg_parser.cc
	${CMAKE_SOURCE_DIR}/src/app_globl_state.cc
	${CMAKE_SOURCE_DIR}/src/ipc_packet_buffer.cc
	${CMAKE_SOURCE_DIR}/src/receiver.cc
	${CMAKE_SOURCE_DIR}/src/sender.cc
	${CMAKE_SOURCE_DIR}/src/ipc_stack.cc
	${CMAKE_SOURCE_DIR}/src/log.cc
	${CMAKE_SOURCE_DIR}/src/ipc_server.cc
	${CMAKE_SOURCE_DIR}/src/ipc_client.cc
	${CMAKE_SOURCE_DIR}/ipc_sockets/ipc_unix_socket.cc
	${CMAKE_SOURCE_DIR}/ipc_proto/ipc_proto.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ecgmd.cc)

set(ECGMD_TARGET_NAME ecgmd)

set(ECGMD_INCLUDE_DIRS
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/ipc_proto
	${CMAKE_SOURCE_DIR}/ipc_sockets
	${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${ECGMD_TARGET_NAME} ${ECGMD_SOURCES})

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_compile_options(${ECGMD_TARGET_NAME} PRIVATE -Werror -Wall -Wextra -pedantic -fdiagnostics-color=always)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_compile_options(${ECGMD_TARGET_NAME} PRIVATE -Weverything -fcolor-diagnostics)
endif()

target_include_directories(${ECGMD_TARGET_NAME} PUBLIC ${ECGMD_INCLUDE_DIRS})

