#include "TestRegistration.h"
#include <server/RequestHandler.h>
#include <hpp/Common.hpp>

TestRegistration::TestRegistration() :
    _sUsername1("FirstUsername"),
    _sUsername2("SecondUsername"),
    _sPassword("password")
{
}

void TestRegistration::TearDown()
{
    RequestHandler::RemoveUser(_sUsername1, _sPassword);
    RequestHandler::RemoveUser(_sUsername2, _sPassword);
}



TEST_F(TestRegistration, Successfull)
{
    nlohmann::json expectedReply;
    expectedReply["Message"] = "0";
    ASSERT_EQ(RequestHandler::RegisterNewUser(_sUsername1, _sPassword), expectedReply);
    expectedReply["Message"] = "1";
    ASSERT_EQ(RequestHandler::RegisterNewUser(_sUsername2, _sPassword), expectedReply);

    expectedReply["Message"] = Registration::SuccessfullyRemoved;
    ASSERT_EQ(RequestHandler::RemoveUser(_sUsername1, _sPassword), expectedReply);
    expectedReply["Message"] = Registration::SuccessfullyRemoved;
    ASSERT_EQ(RequestHandler::RemoveUser(_sUsername2, _sPassword), expectedReply);
}

TEST_F(TestRegistration, UserExist)
{
    nlohmann::json expectedReply;
    expectedReply["Message"] = "0";
    ASSERT_EQ(RequestHandler::RegisterNewUser(_sUsername1, _sPassword), expectedReply);
    expectedReply["Message"] = Registration::UserExist;
    ASSERT_EQ(RequestHandler::RegisterNewUser(_sUsername1, _sPassword), expectedReply);

    expectedReply["Message"] = Registration::SuccessfullyRemoved;
    ASSERT_EQ(RequestHandler::RemoveUser(_sUsername1, _sPassword), expectedReply);
}
