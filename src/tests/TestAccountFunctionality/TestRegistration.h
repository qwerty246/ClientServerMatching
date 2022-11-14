#pragma once

#include <gtest/gtest.h>

class TestRegistration : public testing::Test
{
public:
    TestRegistration();
    ~TestRegistration() = default;

    void TearDown() override;
protected:
    const std::string _sUsername1;
    const std::string _sUsername2;
    const std::string _sPassword;
};
