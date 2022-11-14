#pragma once

#include <gtest/gtest.h>

class TestAuthorization : public testing::Test
{
public:
    TestAuthorization();
    ~TestAuthorization() = default;

    void SetUp() override;
    void TearDown() override;
protected:
    const std::string _sUsername;
    const std::string _sPassword;
};
