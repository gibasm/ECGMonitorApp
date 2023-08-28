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
mock_socket :public ecgm::ipc_socket_iface
{
public:
    MOCK_METHOD(void, connect, (), (override));
    MOCK_METHOD(void, allow_connections, (size_t), (override));
    MOCK_METHOD(void, accept_connection, (), (override));
    MOCK_METHOD(void, write, (void*, size_t), (override));
    MOCK_METHOD(size_t, read, (void*, size_t), (override));
};

class 
ipc_server_tests :public ::testing::Test
{};

static constexpr ecgm::ipc_packet sample_packet = {
    .any = {
        .type = ecgm::ipc_packet_type::START,
        .length = sizeof(ecgm::ipc_any_packet)
    }
};

TEST_F(ipc_server_tests, ipc_server_should_call_socket_write_when_sending_packet)
{
    auto socket = NiceMock<mock_socket>();
    ecgm::ipc_server server(&socket);  

    EXPECT_CALL(socket, write(_, sizeof(ecgm::ipc_any_packet)));

    EXPECT_NO_THROW(server.send(sample_packet));
}

TEST_F(ipc_server_tests, ipc_server_should_receive_and_deserialize_incoming_packet)
{
    auto socket = NiceMock<mock_socket>();
    ecgm::ipc_server server(&socket); 

    ecgm::serialized_ipc_packet incoming = ecgm::serialize_ipc_packet(sample_packet);

    EXPECT_CALL(socket, read(_,_))
        .WillRepeatedly(
        Invoke([&] (void* data, size_t size) -> size_t
        {
            memcpy(data, (void*)incoming.data(), sizeof(ecgm::ipc_any_packet)); 
            return sizeof(ecgm::ipc_any_packet);
        }
    ));

    EXPECT_NO_THROW(server.receive());
}
