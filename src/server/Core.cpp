#include "Core.h"
#include <hpp/Common.hpp>

Core& Core::GetCore()
{
    static Core core;
    return core;
}

UserPtr Core::GetUser(size_t uUserId)
{
    auto userIt = _mUsers.find(uUserId);
    if (userIt == _mUsers.cend())
    {
        return nullptr;
    }
    return userIt->second;
}

UserPtr Core::GetUser(std::string_view sUserName)
{
    for (auto pUser : _mUsers)
    {
        if (pUser.second->GetName() == sUserName)
        {
            return pUser.second;
        }
    }
    return nullptr;
}

const std::map<size_t, UserPtr>& Core::GetUsers() const
{
    return _mUsers;
}

std::string Core::RegisterNewUser(std::string_view sUserName, std::string_view sUserPassword)
{
    auto pUser = GetUser(sUserName);
    if (pUser)
    {
        return Registration::UserExist;
    }
    size_t uNewUserId = std::size(_mUsers);
    UserPtr pNewUser(new User(sUserName, sUserPassword, uNewUserId));
    _mUsers[uNewUserId] = pNewUser;
    return std::to_string(pNewUser->GetId());
}

std::string Core::AuthorizeUser(std::string_view sUserName, std::string_view sUserPassword)
{
    auto pUser = GetUser(sUserName);
    if (pUser)
    {
        if (pUser->GetPassword() == sUserPassword)
        {
            return std::to_string(pUser->GetId());
        }
        return Registration::InvalidPassword;
    }
    return Registration::UserNotExist;
}

std::string Core::RemoveUser(std::string_view sUserName, std::string_view sUserPassword)
{
    for (auto itUser = _mUsers.begin(); itUser != _mUsers.end(); itUser++)
    {
        if (itUser->second->GetName() == sUserName)
        {
            if (itUser->second->GetPassword() != sUserPassword)
            {
                return Registration::InvalidPassword;
            }
            _mUsers.erase(itUser);
            return Registration::SuccessfullyRemoved;
        }
    }
    return Registration::UserNotExist;
}
