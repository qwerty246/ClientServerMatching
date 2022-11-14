#include "TestRemovingUser.h"
#include <server/RequestHandler.h>
#include <hpp/Common.hpp>

TestRemovingUser::TestRemovingUser() :
    _sUsername("SomeUsername"),
    _sPassword("password")
{
}

void TestRemovingUser::SetUp()
{
    nlohmann::json expectedReply;
    expectedReply["Message"] = "0";
    ASSERT_EQ(RequestHandler::RegisterNewUser(_sUsername, _sPassword), expectedReply);
}

void TestRemovingUser::TearDown()
{
    RequestHandler::RemoveUser(_sUsername, _sPassword);
}



TEST_F(TestRemovingUser, InvalidPassword)
{
    nlohmann::json expectedReply;
    expectedReply["Message"] = Registration::InvalidPassword;
    ASSERT_EQ(RequestHandler::RemoveUser(_sUsername, _sPassword + "Invalid"), expectedReply);
}

TEST_F(TestRemovingUser, UserNotExist)
{
    nlohmann::json expectedReply;
    expectedReply["Message"] = Registration::UserNotExist;
    ASSERT_EQ(RequestHandler::RemoveUser(_sUsername + "Unknown", _sPassword), expectedReply);
}
