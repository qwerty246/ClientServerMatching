#include "RequestHandler.h"
#include "Core.h"
#include <hpp/Common.hpp>

std::string RequestHandler::AuthorizeUser(std::string_view sUsername, std::string_view sPassword)
{
    return Core::GetCore().AuthorizeUser(sUsername, sPassword);
}

std::string RequestHandler::RegisterNewUser(std::string_view sUsername, std::string_view sPassword)
{
    return Core::GetCore().RegisterNewUser(sUsername, sPassword);
}

std::string RequestHandler::RemoveUser(std::string_view sUsername, std::string_view sPassword)
{
    return Core::GetCore().RemoveUser(sUsername, sPassword);
}

nlohmann::json RequestHandler::ShowBalance(const UserPtr& pUser)
{
    nlohmann::json reply;
    reply["AmountRUB"] = std::to_string(pUser->GetAmountRUB());
    reply["AmountUSD"] = std::to_string(pUser->GetAmountUSD());
    return reply;
}

nlohmann::json RequestHandler::ShowUserActiveOffers(const UserPtr& pUser)
{
    nlohmann::json reply;
    reply["SaleNum"] = "0";
    for (auto itOffer : pUser->GetOffersSale())
    {
        if (reply["SaleNum"] != "0")
        {
            AddToField(reply, "SaleOfferId", itOffer->GetId() + "/");
            AddToField(reply, "SaleAmount", std::to_string(itOffer->GetAmount()) + "/");
            AddToField(reply, "SalePrice", std::to_string(itOffer->GetPrice()) + "/");
        }
        else
        {
            reply["SaleOfferId"] = itOffer->GetId() + "/";
            reply["SaleAmount"] = std::to_string(itOffer->GetAmount()) + "/";
            reply["SalePrice"] = std::to_string(itOffer->GetPrice()) + "/";
            reply["SaleNum"] = std::to_string(std::size(pUser->GetOffersSale()));
        }
    }

    reply["PurchaseNum"] = "0";
    for (auto itOffer : pUser->GetOffersPurchase())
    {
        if (reply["PurchaseNum"] != "0")
        {
            AddToField(reply, "PurchaseOfferId", itOffer->GetId() + "/");
            AddToField(reply, "PurchaseAmount", std::to_string(itOffer->GetAmount()) + "/");
            AddToField(reply, "PurchasePrice", std::to_string(itOffer->GetPrice()) + "/");
        }
        else
        {
            reply["PurchaseOfferId"] = itOffer->GetId() + "/";
            reply["PurchaseAmount"] = std::to_string(itOffer->GetAmount()) + "/";
            reply["PurchasePrice"] = std::to_string(itOffer->GetPrice()) + "/";
            reply["PurchaseNum"] = std::to_string(std::size(pUser->GetOffersPurchase()));
        }
    }
    return reply;
}

nlohmann::json RequestHandler::ShowUserCompletedOffers(const UserPtr& pUser)
{
    nlohmann::json reply;
    reply["OffersNum"] = "0";
    for (auto itOffer : pUser->GetCompletedpOffers())
    {
        if (reply["OffersNum"] != "0")
        {
            AddToField(reply, "OfferId", itOffer->GetId() + "/");
            AddToField(reply, "OriginalAmount", std::to_string(itOffer->GetAmount()) + "/");
            AddToField(reply, "Price", std::to_string(itOffer->GetPrice()) + "/");
            AddToField(reply, "Type", std::string((itOffer->IsSaleOffer() ? "Sale" : "Purchase")) + "/");
        }
        else
        {
            reply["OfferId"] = itOffer->GetId() + "/";
            reply["OriginalAmount"] = std::to_string(itOffer->GetAmount()) + "/";
            reply["Price"] = std::to_string(itOffer->GetPrice()) + "/";
            reply["Type"] = std::string((itOffer->IsSaleOffer() ? "Sale" : "Purchase")) + "/";
            reply["OffersNum"] = std::to_string(std::size(pUser->GetCompletedpOffers()));
        }
    }
    return reply;
}

nlohmann::json RequestHandler::ShowAllOffers(const UserPtr& pUser, bool bIsSaleOffer)
{
    nlohmann::json reply;
    reply["UsersNum"] = "0";
    double dPriceMax = 0;
    double dPriceMin = -1;
    double dPriceSum = 0;
    size_t uCount = 0;
    for (auto itUser : Core::GetCore().GetUsers())
    {
        if (itUser.first == pUser->GetId())
        {
            continue;
        }
        for (auto itOffer : (bIsSaleOffer ? itUser.second->GetOffersSale() : itUser.second->GetOffersPurchase()))
        {
            double dPrice = itOffer->GetPrice();
            if (dPriceMax < dPrice)
            {
                dPriceMax = dPrice;
            }

            if (dPriceMin == -1)
            {
                dPriceMin = dPrice;
            }
            else if (dPriceMin > dPrice)
            {
                dPriceMin = dPrice;
            }
            dPriceSum += dPrice;
            uCount++;

            if (reply["UsersNum"] != "0")
            {
                AddToField(reply, "Username", itUser.second->GetName() + "/");
                AddToField(reply, "Amount", std::to_string(itOffer->GetAmount()) + "/");
                AddToField(reply, "Price", std::to_string(itOffer->GetPrice()) + "/");
            }
            else
            {
                reply["Username"] = itUser.second->GetName() + "/";
                reply["Amount"] = std::to_string(itOffer->GetAmount()) + "/";
                reply["Price"] = std::to_string(itOffer->GetPrice()) + "/";
                reply["UsersNum"] = "1";
            }
        }
    }
    if (!uCount)
    {
        reply["UsersNum"] = "0";
        return reply;
    }
    reply["UsersNum"] = std::to_string(uCount);
    reply["Max"] = std::to_string(dPriceMax);
    reply["Min"] = std::to_string(dPriceMin);
    reply["Mean"] = std::to_string(dPriceSum / uCount);
    return reply;
}

nlohmann::json RequestHandler::ReplenishUSD(const UserPtr& pUser, double dAmount)
{
    pUser->ReplenishUSD(dAmount);
    nlohmann::json reply;
    reply["Amount"] = std::to_string(dAmount);
    return reply;
}

nlohmann::json RequestHandler::ReplenishRUB(const UserPtr& pUser, double dAmount)
{
    pUser->ReplenishRUB(dAmount);
    nlohmann::json reply;
    reply["Amount"] = std::to_string(dAmount);
    return reply;
}

nlohmann::json RequestHandler::RemoveUSD(const UserPtr& pUser, double dAmount)
{
    pUser->RemoveUSD(dAmount);
    nlohmann::json reply;
    reply["Amount"] = std::to_string(dAmount);
    return reply;
}

nlohmann::json RequestHandler::RemoveRUB(const UserPtr& pUser, double dAmount)
{
    pUser->RemoveRUB(dAmount);
    nlohmann::json reply;
    reply["Amount"] = std::to_string(dAmount);
    return reply;
}

void RequestHandler::AddOfferAndTryToMatch(nlohmann::json& reply, const UserPtr& pUser, const OfferPtr& pNewOffer)
{
    AddOfferAndTryToMatch(reply, pUser, pNewOffer, 0);
}

nlohmann::json RequestHandler::RemoveOffer(const UserPtr& pUser, const std::string& sOfferId)
{
    nlohmann::json reply;
    std::string sError = "Incorrect id.\n";
    bool bIsSaleOffer;
    try
    {
        size_t pos1 = sOfferId.find("_");
        std::string sUserId = sOfferId.substr(0, pos1);
        std::string sId = sOfferId.substr(pos1 + 3);

        std::stoi(sUserId);
        size_t uId = std::stoi(sId);
        size_t uCount = 1;
        while (uId != uId % 10)
        {
            uId = uId / 10;
            uCount++;
        }
        if (std::size(sOfferId) != (std::size(sUserId) + 3 + uCount))
        {
            throw sError;
        }

        if (sOfferId.substr(pos1, 3) == "_s_")
        {
            bIsSaleOffer = true;
        }
        else if (sOfferId.substr(pos1, 3) == "_p_")
        {
            bIsSaleOffer = false;
        }
        else
        {
            throw sError;
        }
    }
    catch (...)
    {
        reply["OfferId"] = "-1";
        return reply;
    }

    auto itBegin = bIsSaleOffer ? pUser->GetOffersSale().begin() : pUser->GetOffersPurchase().begin();
    auto itEnd = bIsSaleOffer ? pUser->GetOffersSale().end() : pUser->GetOffersPurchase().end();
    for (auto itOffer = itBegin; itOffer != itEnd; itOffer++)
    {
        if ((*itOffer)->GetId() == sOfferId)
        {
            pUser->RemoveOffer(itOffer);
            reply["OfferId"] = sOfferId;
            return reply;
        }
    }
    reply["OfferId"] = "0";
    return reply;
}

std::string RequestHandler::GenerateOfferId(size_t uUserId, bool bIsSaleOffer, size_t uOfferId)
{
    std::string sOfferId = std::to_string(uUserId);
    sOfferId += bIsSaleOffer ? "_s_" : "_p_";
    sOfferId += std::to_string(uOfferId);
    return sOfferId;
}

void RequestHandler::AddToField(nlohmann::json& reply, const char* pParam, const std::string sAddition)
{
    std::string sTemp = reply[pParam];
    sTemp += sAddition;
    reply[pParam] = sTemp;
}

void RequestHandler::AddOfferAndTryToMatch(nlohmann::json& reply, const UserPtr& pUser, const OfferPtr& pNewOffer, size_t uNumOffers)
{
    Core& core = Core::GetCore();
    auto mUsers = core.GetUsers();
    bool bIsSaleOffer = pNewOffer->IsSaleOffer();
    auto itLastOffer = bIsSaleOffer ?
                mUsers.begin()->second->GetOffersPurchase().end() :
                mUsers.begin()->second->GetOffersSale().end();
    auto itBestOffer = itLastOffer;
    for (auto itUser = mUsers.begin(); itUser != mUsers.end(); itUser++)
    {
        if (pUser->GetId() == itUser->second->GetId())
        {
            continue;
        }
        auto itOfferBegin = bIsSaleOffer ?
                    itUser->second->GetOffersPurchase().begin() :
                    itUser->second->GetOffersSale().begin();
        auto itOfferEnd = bIsSaleOffer ?
                    itUser->second->GetOffersPurchase().end() :
                    itUser->second->GetOffersSale().end();
        for (auto itOffer = itOfferBegin; itOffer != itOfferEnd; itOffer++)
        {
            if (itBestOffer != itLastOffer)
            {
                if ((*itBestOffer)->IsMoreProfitable(*itOffer))
                {
                    itBestOffer = itOffer;
                }
            }
            else
            {
                itBestOffer = itOffer;
            }
        }
    }

    auto AcceptOffer = [](const UserPtr& pUser, const OfferPtr& pNewOffer, nlohmann::json& reply)
    {
        pUser->AddOffer(pNewOffer);
        reply["Accept"] = "1";
        reply["Completed"] = "0";
        reply["FinalAmount"] = std::to_string(pNewOffer->GetAmount());
        reply["FinalPrice"] = std::to_string(pNewOffer->GetPrice());
    };

    if (itBestOffer == itLastOffer)
    {
        AcceptOffer(pUser, pNewOffer, reply);
        reply["NumOffers"] = std::to_string(uNumOffers);
        return;
    }

    auto pBestOffer = *itBestOffer;
    if (pNewOffer->IsMoreProfitable(pBestOffer))
    {
        AcceptOffer(pUser, pNewOffer, reply);
        reply["NumOffers"] = std::to_string(uNumOffers);
        return;
    }

    UserPtr pSeller, pBuyer;
    if (bIsSaleOffer)
    {
        pSeller = pUser;
        pBuyer = core.GetUser(std::stoi(pBestOffer->GetUserId()));
    }
    else
    {
        pBuyer = pUser;
        pSeller = core.GetUser(std::stoi(pBestOffer->GetUserId()));
    }

    auto MakeDeal = [](const UserPtr& pSeller, const UserPtr& pBuyer, double dAmountUSD, double dPrice)
    {
        pSeller->RemoveUSD(dAmountUSD);
        pSeller->ReplenishRUB(dAmountUSD * dPrice);
        pBuyer->RemoveRUB(dAmountUSD * dPrice);
        pBuyer->ReplenishUSD(dAmountUSD);
    };

    if (pNewOffer->GetAmount() == pBestOffer->GetAmount())
    {
        reply["Accept"] = "0";
        reply["Completed"] = "1";
        reply["NumOffers"] = std::to_string(uNumOffers);
        if (uNumOffers)
        {
            AddToField(reply, "Amount", std::to_string(pBestOffer->GetAmount()) + "/");
            AddToField(reply, "Price", std::to_string(pBestOffer->GetPrice()) + "/");
        }
        else
        {
            reply["Amount"] = std::to_string(pBestOffer->GetAmount()) + "/";
            reply["Price"] = std::to_string(pBestOffer->GetPrice()) + "/";
            reply["Type"] = std::string(pNewOffer->IsSaleOffer() ? "Sale" : "Purchase");
        }
        MakeDeal(pSeller, pBuyer, pBestOffer->GetAmount(), pBestOffer->GetPrice());
        if (bIsSaleOffer)
        {
            pBuyer->RemoveOffer(itBestOffer);
            pSeller->AddCompletedOffer(pNewOffer);
        }
        else
        {
            pSeller->RemoveOffer(itBestOffer);
            pBuyer->AddCompletedOffer(pNewOffer);
        }
    }
    else if (pNewOffer->GetAmount() < pBestOffer->GetAmount())
    {
        reply["Accept"] = "0";
        reply["Completed"] = "1";
        reply["NumOffers"] = std::to_string(uNumOffers);
        if (uNumOffers)
        {
            AddToField(reply, "Amount", std::to_string(pNewOffer->GetAmount()) + "/");
            AddToField(reply, "Price", std::to_string(pBestOffer->GetPrice()) + "/");
        }
        else
        {
            reply["Amount"] = std::to_string(pNewOffer->GetAmount()) + "/";
            reply["Price"] = std::to_string(pBestOffer->GetPrice()) + "/";
            reply["Type"] = std::string(pNewOffer->IsSaleOffer() ? "Sale" : "Purchase");
        }
        MakeDeal(pSeller, pBuyer, pNewOffer->GetAmount(), pBestOffer->GetPrice());
        pBestOffer->RemoveAmount(pNewOffer->GetAmount());
        if (bIsSaleOffer)
        {
            pSeller->AddCompletedOffer(pNewOffer);
        }
        else
        {
            pBuyer->AddCompletedOffer(pNewOffer);
        }
    }
    else
    {
        if (uNumOffers)
        {
            AddToField(reply, "Amount", std::to_string(pBestOffer->GetAmount()) + "/");
            AddToField(reply, "Price", std::to_string(pBestOffer->GetPrice()) + "/");
        }
        else
        {
            reply["Amount"] = std::to_string(pBestOffer->GetAmount()) + "/";
            reply["Price"] = std::to_string(pBestOffer->GetPrice()) + "/";
            reply["Type"] = std::string(pNewOffer->IsSaleOffer() ? "Sale" : "Purchase");
        }
        MakeDeal(pSeller, pBuyer, pBestOffer->GetAmount(), pBestOffer->GetPrice());
        pNewOffer->RemoveAmount(pBestOffer->GetAmount());
        if (bIsSaleOffer)
        {
            pBuyer->RemoveOffer(itBestOffer);
        }
        else
        {
            pSeller->RemoveOffer(itBestOffer);
        }
        AddOfferAndTryToMatch(reply, pUser, pNewOffer, ++uNumOffers);
    }
}
