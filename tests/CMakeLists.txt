include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

set(TEST_TARGET test)

set(TEST_SOURCES
    ipc_unix_socket_tests.cc
    ipc_server_tests.cc
    ipc_proto_tests.cc
)

add_executable(
    ${TEST_TARGET}
    ${TEST_SOURCES}
    ${SOURCES}
)


target_include_directories(
    ${TEST_TARGET} PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/ipc_proto
    ${PROJECT_SOURCE_DIR}/ipc_sockets
)

target_link_libraries(${TEST_TARGET} GTest::gtest_main GTest::gmock ${SDL2_LIBRARIES})

include(GoogleTest)
gtest_discover_tests(${TEST_TARGET})
