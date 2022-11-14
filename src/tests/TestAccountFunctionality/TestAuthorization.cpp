#include "TestAuthorization.h"
#include <server/RequestHandler.h>
#include <hpp/Common.hpp>

TestAuthorization::TestAuthorization() :
    _sUsername("SomeUsername"),
    _sPassword("password")
{
}

void TestAuthorization::SetUp()
{
    nlohmann::json expectedReply;
    expectedReply["Message"] = "0";
    ASSERT_EQ(RequestHandler::RegisterNewUser(_sUsername, _sPassword), expectedReply);
}

void TestAuthorization::TearDown()
{
    nlohmann::json expectedReply;
    expectedReply["Message"] = Registration::SuccessfullyRemoved;
    ASSERT_EQ(RequestHandler::RemoveUser(_sUsername, _sPassword), expectedReply);
}



TEST_F(TestAuthorization, Successfull)
{
    nlohmann::json expectedReply;
    expectedReply["Message"] = "0";
    ASSERT_EQ(RequestHandler::AuthorizeUser(_sUsername, _sPassword), expectedReply);
}

TEST_F(TestAuthorization, InvalidPassword)
{
    nlohmann::json expectedReply;
    expectedReply["Message"] = Registration::InvalidPassword;
    ASSERT_EQ(RequestHandler::AuthorizeUser(_sUsername, _sPassword + "Invalid"), expectedReply);
}

TEST_F(TestAuthorization, UserNotExist)
{
    nlohmann::json expectedReply;
    expectedReply["Message"] = Registration::UserNotExist;
    ASSERT_EQ(RequestHandler::AuthorizeUser(_sUsername + "Unknown", _sPassword), expectedReply);
}
