#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include "ipc_proto.hh"

class 
ipc_proto_tests :public ::testing::Test
{};

TEST_F(ipc_proto_tests, 
       ipc_serialized_packet_should_be_the_same_after_deserailzed)
{
    ecgm::ipc_packet ecg_sample = {
        .any = {
            .type = ecgm::ipc_packet_type::ECG_SAMPLE,
            .length = sizeof(ecgm::ipc_ecg_sample_packet)
        }
    };

    ecg_sample.sample.sample_no = 0xDEADBEEF;
    ecg_sample.sample.value = 0.75f;
   
    auto serialized = ecgm::serialize_ipc_packet(ecg_sample);

    auto deserialized = ecgm::deserialize_ipc_packet(serialized);
    
    EXPECT_EQ(ecg_sample.any.type,         deserialized.any.type);
    EXPECT_EQ(ecg_sample.any.length,       deserialized.any.length); 
    EXPECT_EQ(ecg_sample.sample.sample_no, deserialized.sample.sample_no); 
    EXPECT_EQ(ecg_sample.sample.value,     deserialized.sample.value); 
}

class
ipc_proto_tests_deserialize_param :public ipc_proto_tests,
    public ::testing::WithParamInterface<ecgm::serialized_ipc_packet>
{};

INSTANTIATE_TEST_SUITE_P(ipc_proto_tests, 
                         ipc_proto_tests_deserialize_param,
                         ::testing::Values(
                            ecgm::serialized_ipc_packet({0xFF, 2U}),
                            ecgm::serialized_ipc_packet({
                                ecgm::ipc_packet_type::ECG_SAMPLE,
                                0x01
                            }),
                            ecgm::serialized_ipc_packet({
                                ecgm::ipc_packet_type::START,
                                3U,
                                0xFFU
                            })
                         )
);

TEST_P(ipc_proto_tests_deserialize_param,
       deserialize_ipc_packet_should_throw_ipc_exception_when_provided_w_malformed_packet)
{
    auto serialized_packet = GetParam(); 

    EXPECT_THROW(
    {
        ecgm::deserialize_ipc_packet(serialized_packet);
    }, 
    ecgm::ipc_exception);
}
