#pragma once

#include <server/user/User.h>
#include <server/user/Offer.h>
#include <hpp/json.hpp>

#include <string>
#include <string_view>

class RequestHandler
{
public:
    static nlohmann::json RegisterNewUser(std::string_view sUsername, std::string_view sPassword);
    static nlohmann::json AuthorizeUser(std::string_view sUsername, std::string_view sPassword);
    static nlohmann::json RemoveUser(std::string_view sUsername, std::string_view sPassword);

    static nlohmann::json ShowBalance(const UserPtr& pUser);
    static nlohmann::json ShowUserActiveOffers(const UserPtr& pUser);
    static nlohmann::json ShowUserCompletedOffers(const UserPtr& pUser);
    static nlohmann::json ShowAllOffers(const UserPtr& pUser, bool bIsSaleOffer);

    static nlohmann::json ReplenishUSD(const UserPtr& pUser, double dAmount);
    static nlohmann::json ReplenishRUB(const UserPtr& pUser, double dAmount);
    static nlohmann::json RemoveUSD(const UserPtr& pUser, double dAmount);
    static nlohmann::json RemoveRUB(const UserPtr& pUser, double dAmount);

    static void AddOfferAndTryToMatch(nlohmann::json& reply, const UserPtr& pUser, const OfferPtr& pNewOffer);
    static nlohmann::json RemoveOffer(const UserPtr& pUser, const std::string& sOfferId);

    static std::string GenerateOfferId(size_t uUserId, bool bIsSaleOffer, size_t uOfferId);
    static void AddToField(nlohmann::json& reply, const char* pParam, const std::string sAddition);
private:
    static void AddOfferAndTryToMatch(nlohmann::json& reply, const UserPtr& pUser, const OfferPtr& pNewOffer, size_t uNumOffers);
};
