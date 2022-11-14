#pragma once

#include <gtest/gtest.h>

class TestRemovingUser : public testing::Test
{
public:
    TestRemovingUser();
    ~TestRemovingUser() = default;

    void SetUp() override;
    void TearDown() override;
protected:
    const std::string _sUsername;
    const std::string _sPassword;
};
