#pragma once

#include <server/user/User.h>

#include <string>
#include <string_view>
#include <map>

class Core
{
private:
    Core() = default;
    Core(const Core&) = default;
    ~Core() = default;

    Core& operator=(Core&) = delete;
public:
    static Core& GetCore();
    UserPtr GetUser(size_t uUserId);
    UserPtr GetUser(std::string_view sUserName);
    const std::map<size_t, UserPtr>& GetUsers() const;

    std::string RegisterNewUser(std::string_view sUserName, std::string_view sUserPassword);
    std::string AuthorizeUser(std::string_view sUserName, std::string_view sUserPassword);
    std::string RemoveUser(std::string_view sUserName, std::string_view sUserPassword);
private:
    std::map<size_t, UserPtr> _mUsers;
};
