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
    ASSERT_EQ(RequestHandler::RegisterNewUser(_sUsername1, _sPassword), "0");
    ASSERT_EQ(RequestHandler::RegisterNewUser(_sUsername2, _sPassword), "1");
    ASSERT_EQ(RequestHandler::RemoveUser(_sUsername1, _sPassword), Registration::SuccessfullyRemoved);
    ASSERT_EQ(RequestHandler::RemoveUser(_sUsername2, _sPassword), Registration::SuccessfullyRemoved);
}

TEST_F(TestRegistration, UserExist)
{
    ASSERT_EQ(RequestHandler::RegisterNewUser(_sUsername1, _sPassword), "0");
    ASSERT_EQ(RequestHandler::RegisterNewUser(_sUsername1, _sPassword), Registration::UserExist);
    ASSERT_EQ(RequestHandler::RemoveUser(_sUsername1, _sPassword), Registration::SuccessfullyRemoved);
}
