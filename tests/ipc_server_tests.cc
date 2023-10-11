#include "gmock/gmock.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ipc_server.hh"

using ::testing::NiceMock;
using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;
using ::testing::Invoke;

class 
mock_server_socket :public ecgm::ipc_server_socket_iface
{
public:
    MOCK_METHOD(void, send, (void*, size_t), (override));
    MOCK_METHOD(size_t, receive, (void*, size_t), (override));
    MOCK_METHOD(void, listen, (), (override));
    MOCK_METHOD(ecgm::socket_ptr, accept_connection, (), (override));
};

class
mock_client_socket :public ecgm::ipc_client_socket_iface
{
    MOCK_METHOD(void, send, (void*, size_t), (override));
    MOCK_METHOD(size_t, receive, (void*, size_t), (override));
    MOCK_METHOD(void, connect, (), (override));
};

class 
ipc_server_tests :public ::testing::Test
{};

TEST_F(ipc_server_tests, should_call_listen_and_accept_connection_when_wait_for_connection_called)
{
    auto socket = NiceMock<mock_server_socket>();
    ecgm::ipc_server server(&socket);

    EXPECT_CALL(socket, listen);
    EXPECT_CALL(socket, accept_connection());

    server.wait_for_connection();
}

static constexpr ecgm::ipc_packet sample_packet = {
    .any = {
        .type = ecgm::ipc_packet_type::START,
        .length = sizeof(ecgm::ipc_any_packet)
    }
};

