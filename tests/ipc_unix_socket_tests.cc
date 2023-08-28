/* Tests specified here will run only on Unix/Linux platform, as they are platform specific */
#ifdef __unix__
#include <gtest/gtest.h>
#include "ipc_unix_socket.hh" 

class
ipc_unix_socket_tests :public ::testing::Test
{};

TEST_F(ipc_unix_socket_tests, should_open_upon_creation_without_exception)
{
    EXPECT_NO_THROW(ecgm::ipc_unix_socket socket("./test.socket"));
}

TEST_F(ipc_unix_socket_tests, should_throw_when_reading_without_connection)
{
    ecgm::ipc_unix_socket socket("./test.socket");

    socket.allow_connections(1);
    uint8_t byte_buffer;

    EXPECT_THROW(socket.read(&byte_buffer, 1UL), ecgm::ipc_socket_exception);
}

TEST_F(ipc_unix_socket_tests, should_throw_when_writing_without_connection)
{
    ecgm::ipc_unix_socket socket("./test.socket");

    socket.allow_connections(1);
    uint8_t byte = 0x21;

    EXPECT_THROW(socket.write(&byte, 1UL), ecgm::ipc_socket_exception);
}

#endif /* __unix__ */
