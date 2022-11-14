#ifndef CLIENSERVERECN_COMMON_HPP
#define CLIENSERVERECN_COMMON_HPP

#include <string>

static short port = 5555;

namespace Requests
{
    static std::string Registration = "Reg";
    static std::string Authorization = "Auth";
    static std::string RemoveUser = "RemUser";
    static std::string ShowBalance = "ShowB";
    static std::string ReplenishUSD = "RepU";
    static std::string ReplenishRUB = "RepR";
    static std::string RemoveUSD = "RemU";
    static std::string RemoveRUB = "RemR";
    static std::string SaleUSD = "Sale";
    static std::string PurchaseUSD = "Purch";
    static std::string RemoveOffer = "RemOff";
    static std::string ShowUserActiveOffers = "ShowAO";
    static std::string ShowUserCompletedOffers = "ShowCO";
    static std::string ShowAllSaleOffers = "ShowS";
    static std::string ShowAllPurchaseOffers = "ShowP";
}

namespace Registration
{
    static std::string Cancel = "Canc";
    static std::string UnknownCommand = "Unkn";
    static std::string SuccessfullyRemoved = "SucRem";
    static std::string InvalidPassword = "InvPass";
    static std::string UserExist = "UsEx";
    static std::string UserNotExist = "UsNotEx";
}

#endif //CLIENSERVERECN_COMMON_HPP
