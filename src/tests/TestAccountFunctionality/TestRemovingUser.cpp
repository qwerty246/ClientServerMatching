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
    ASSERT_EQ(RequestHandler::RegisterNewUser(_sUsername, _sPassword), "0");
}

void TestRemovingUser::TearDown()
{
    RequestHandler::RemoveUser(_sUsername, _sPassword);
}



TEST_F(TestRemovingUser, InvalidPassword)
{
    ASSERT_EQ(RequestHandler::RemoveUser(_sUsername, _sPassword + "Invalid"), Registration::InvalidPassword);
}

TEST_F(TestRemovingUser, UserNotExist)
{
    ASSERT_EQ(RequestHandler::RemoveUser(_sUsername + "Unknown", _sPassword), Registration::UserNotExist);
}
