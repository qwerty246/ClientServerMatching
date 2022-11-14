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
    ASSERT_EQ(RequestHandler::RegisterNewUser(_sUsername, _sPassword), "0");
}

void TestAuthorization::TearDown()
{
    ASSERT_EQ(RequestHandler::RemoveUser(_sUsername, _sPassword), Registration::SuccessfullyRemoved);
}



TEST_F(TestAuthorization, Successfull)
{
    ASSERT_EQ(RequestHandler::AuthorizeUser(_sUsername, _sPassword), "0");
}

TEST_F(TestAuthorization, InvalidPassword)
{
    ASSERT_EQ(RequestHandler::AuthorizeUser(_sUsername, _sPassword + "Invalid"), Registration::InvalidPassword);
}

TEST_F(TestAuthorization, UserNotExist)
{
    ASSERT_EQ(RequestHandler::AuthorizeUser(_sUsername + "Unknown", _sPassword), Registration::UserNotExist);
}
