/* Tests specified here will run only on Unix/Linux platform, as they are platform specific */
#ifdef __unix__
#include <gtest/gtest.h>
#include "ipc_unix_socket.hh" 

class
ipc_unix_socket_tests :public ::testing::Test
{};

TEST_F(ipc_unix_socket_tests, server_socket_should_initialize_without_exceptions)
{
    EXPECT_NO_THROW(ecgm::ipc_unix_server_socket socket("./test.socket"));
}

#endif /* __unix__ */
