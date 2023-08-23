#include <gtest/gtest.h>

class 
should_fail_test :public ::testing::Test
{
};

TEST_F(should_fail_test, this_test_should_fail)
{
    ASSERT_EQ(1, 2);
}
