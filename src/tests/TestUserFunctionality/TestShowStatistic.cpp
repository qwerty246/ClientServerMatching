#include "TestShowStatistic.h"

void TestShowStatistic::CheckActiveOffers(const std::vector<std::string>* vOffersSale, const std::vector<std::string>* vOffersPurchase) const
{
    nlohmann::json expectedReply;
    expectedReply["SaleNum"] = "0";
    if (vOffersSale)
    {
        for (auto itOffer : *vOffersSale)
        {
            if (expectedReply["SaleNum"] != "0")
            {
                size_t pos1 = itOffer.find("/");
                std::string str = itOffer.substr(0, pos1 + 1);
                RequestHandler::AddToField(expectedReply, "SaleOfferId", str);
                size_t pos2 = itOffer.find("/", pos1 + 1);
                str = itOffer.substr(pos1 + 1, pos2 - pos1);
                RequestHandler::AddToField(expectedReply, "SaleAmount", str);
                size_t pos3 = itOffer.find("/", pos2 + 1);
                str = itOffer.substr(pos2 + 1, pos3 - pos2);
                RequestHandler::AddToField(expectedReply, "SalePrice", str);
            }
            else
            {
                size_t pos1 = itOffer.find("/");
                std::string str = itOffer.substr(0, pos1 + 1);
                expectedReply["SaleOfferId"] = str;
                size_t pos2 = itOffer.find("/", pos1 + 1);
                str = itOffer.substr(pos1 + 1, pos2 - pos1);
                expectedReply["SaleAmount"] = str;
                size_t pos3 = itOffer.find("/", pos2 + 1);
                str = itOffer.substr(pos2 + 1, pos3 - pos2);
                expectedReply["SalePrice"] = str;
                expectedReply["SaleNum"] = std::to_string(std::size(_pCurrentUser->GetOffersSale()));
            }
        }
    }

    expectedReply["PurchaseNum"] = "0";
    if (vOffersPurchase)
    {
        for (auto itOffer : *vOffersPurchase)
        {
            if (expectedReply["PurchaseNum"] != "0")
            {
                size_t pos1 = itOffer.find("/");
                std::string str = itOffer.substr(0, pos1 + 1);
                RequestHandler::AddToField(expectedReply, "PurchaseOfferId", str);
                size_t pos2 = itOffer.find("/", pos1 + 1);
                str = itOffer.substr(pos1 + 1, pos2 - pos1);
                RequestHandler::AddToField(expectedReply, "PurchaseAmount", str);
                size_t pos3 = itOffer.find("/", pos2 + 1);
                str = itOffer.substr(pos2 + 1, pos3 - pos2);
                RequestHandler::AddToField(expectedReply, "PurchasePrice", str);
            }
            else
            {
                size_t pos1 = itOffer.find("/");
                std::string str = itOffer.substr(0, pos1 + 1);
                expectedReply["PurchaseOfferId"] = str;
                size_t pos2 = itOffer.find("/", pos1 + 1);
                str = itOffer.substr(pos1 + 1, pos2 - pos1);
                expectedReply["PurchaseAmount"] = str;
                size_t pos3 = itOffer.find("/", pos2 + 1);
                str = itOffer.substr(pos2 + 1, pos3 - pos2);
                expectedReply["PurchasePrice"] = str;
                expectedReply["PurchaseNum"] = std::to_string(std::size(_pCurrentUser->GetOffersPurchase()));
            }
        }
    }
    ASSERT_EQ(_reply, expectedReply);
}

void TestShowStatistic::CheckCompletedOffers(const std::vector<std::string>* vCompletedOffers) const
{
    nlohmann::json expectedReply;
    expectedReply["OffersNum"] = "0";
    if (vCompletedOffers)
    {
        for (auto itOffer : *vCompletedOffers)
        {
            if (expectedReply["OffersNum"] != "0")
            {
                size_t pos1 = itOffer.find("/");
                std::string str = itOffer.substr(0, pos1 + 1);
                RequestHandler::AddToField(expectedReply, "OfferId", str);
                size_t pos2 = itOffer.find("/", pos1 + 1);
                str = itOffer.substr(pos1 + 1, pos2 - pos1);
                RequestHandler::AddToField(expectedReply, "OriginalAmount", str);
                size_t pos3 = itOffer.find("/", pos2 + 1);
                str = itOffer.substr(pos2 + 1, pos3 - pos2);
                RequestHandler::AddToField(expectedReply, "Price", str);
                size_t pos4 = itOffer.find("/", pos3 + 1);
                str = itOffer.substr(pos3 + 1, pos4 - pos3);
                RequestHandler::AddToField(expectedReply, "Type", str);
            }
            else
            {
                size_t pos1 = itOffer.find("/");
                std::string str = itOffer.substr(0, pos1 + 1);
                expectedReply["OfferId"] = str;
                size_t pos2 = itOffer.find("/", pos1 + 1);
                str = itOffer.substr(pos1 + 1, pos2 - pos1);
                expectedReply["OriginalAmount"] = str;
                size_t pos3 = itOffer.find("/", pos2 + 1);
                str = itOffer.substr(pos2 + 1, pos3 - pos2);
                expectedReply["Price"] = str;
                size_t pos4 = itOffer.find("/", pos3 + 1);
                str = itOffer.substr(pos3 + 1, pos4 - pos3);
                expectedReply["Type"] = str;
                expectedReply["OffersNum"] = std::to_string(std::size(_pCurrentUser->GetCompletedpOffers()));
            }
        }
    }
    ASSERT_EQ(_reply, expectedReply);
}

std::string TestShowStatistic::GenerateActiveOfferLine(size_t uOfferId, double dAmount, double dPrice) const
{
    return RequestHandler::GenerateOfferId(_pCurrentUser->GetId(), _bIsCurrentOfferSale, uOfferId) + "/"
           + std::to_string(dAmount) + "/"
           + std::to_string(dPrice) + "/";
}

std::string TestShowStatistic::GenerateCompletedOfferLine(size_t uOfferId, double dAmount, double dPrice) const
{
    return RequestHandler::GenerateOfferId(_pCurrentUser->GetId(), _bIsCurrentOfferSale, uOfferId) + "/"
           + std::to_string(dAmount) + "/"
           + std::to_string(dPrice) + "/"
           + (_bIsCurrentOfferSale ? "Sale" : "Purchase") + "/";
}



TEST_F(TestShowStatistic, NoUserActiveOffers)
{
    SetCurrentUser(_vSellers[0]);
    _reply = RequestHandler::ShowUserActiveOffers(_pCurrentUser);
    CheckActiveOffers(nullptr, nullptr);
}

TEST_F(TestShowStatistic, ShowUserActiveOffers)
{
    std::vector<std::string> vOffersSale;
    {
        SetCurrentUser(_vSellers[0]);
        double dAmount1 = 2;
        double dPrice = 61;
        AddOffer(dAmount1, dPrice);
        CheckReplyOfferAdded(dAmount1, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmount1, dPrice, 0);
        vOffersSale.push_back(GenerateActiveOfferLine(0, dAmount1, dPrice));

        double dAmount2 = 10;
        dPrice = 59;
        AddOffer(dAmount2, dPrice);
        CheckReplyOfferAdded(dAmount2, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(2, 0, dAmount2, dPrice, 1);
        vOffersSale.push_back(GenerateActiveOfferLine(1, dAmount2, dPrice));
    }
    _reply = RequestHandler::ShowUserActiveOffers(_pCurrentUser);
    CheckActiveOffers(&vOffersSale, nullptr);
    vOffersSale.clear();

    {
        SetCurrentUser(_vSellers[1]);
        double dAmount1 = 2;
        double dPrice = 61;
        AddOffer(dAmount1, dPrice);
        CheckReplyOfferAdded(dAmount1, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmount1, dPrice, 0);
        vOffersSale.push_back(GenerateActiveOfferLine(0, dAmount1, dPrice));

        double dAmount2 = 15;
        dPrice = 59;
        AddOffer(dAmount2, dPrice);
        CheckReplyOfferAdded(dAmount2, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(2, 0, dAmount2, dPrice, 1);
        vOffersSale.push_back(GenerateActiveOfferLine(1, dAmount2, dPrice));
    }
    _reply = RequestHandler::ShowUserActiveOffers(_pCurrentUser);
    CheckActiveOffers(&vOffersSale, nullptr);

    std::vector<std::string> vOffersPurchase;
    {
        double dAmount1 = 10;
        double dPrice = 59;
        _bIsCurrentOfferSale = false;
        AddOffer(dAmount1, dPrice);
        CheckReplyOfferCompleted(dAmount1, dPrice);
        CheckBalance(_dInitialAmountUSD + dAmount1,
                     _dInitialAmountRUB - dPrice * dAmount1);

        double dAmount2 = 5;
        AddOffer(dAmount2, dPrice);
        CheckReplyOfferAdded(dAmount2, dPrice);
        CheckBalance(_dInitialAmountUSD + dAmount1,
                     _dInitialAmountRUB - dPrice * dAmount1);
        vOffersPurchase.push_back(GenerateActiveOfferLine(1, dAmount2, dPrice));
    }
    _reply = RequestHandler::ShowUserActiveOffers(_pCurrentUser);
    CheckActiveOffers(&vOffersSale, &vOffersPurchase);
}

TEST_F(TestShowStatistic, NoUserCompletedOffers)
{
    SetCurrentUser(_vSellers[0]);
    _reply = RequestHandler::ShowUserCompletedOffers(_pCurrentUser);
    CheckCompletedOffers(nullptr);

    double dAmount1 = 2;
    double dPrice = 61;
    AddOffer(dAmount1, dPrice);
    CheckReplyOfferAdded(dAmount1, dPrice);
    CheckBalanceEqualToOriginal();
    CheckOffer(1, 0, dAmount1, dPrice, 0);
    _reply = RequestHandler::ShowUserCompletedOffers(_pCurrentUser);
    CheckCompletedOffers(nullptr);
}

TEST_F(TestShowStatistic, ShowUserCompletedOffers)
{
    double dAmount1 = 10;
    double dPrice1 = 60;
    std::vector<std::string> vCompletedOffersSale;
    {
        SetCurrentUser(_vSellers[0]);
        AddOffer(dAmount1, dPrice1);
        CheckReplyOfferAdded(dAmount1, dPrice1);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmount1, dPrice1, 0);
        vCompletedOffersSale.push_back(GenerateCompletedOfferLine(0, dAmount1, dPrice1));
    }

    std::vector<std::string> vCompletedOffersPurchase;
    {
        SetCurrentUser(_vBuyers[0]);
        AddOffer(dAmount1, dPrice1);
        CheckReplyOfferCompleted(dAmount1, dPrice1);
        CheckBalance(_dInitialAmountUSD + dAmount1,
                     _dInitialAmountRUB - dPrice1 * dAmount1);
        CheckListsOffersEmpty();
        vCompletedOffersPurchase.push_back(GenerateCompletedOfferLine(0, dAmount1, dPrice1));
    }
    _reply = RequestHandler::ShowUserCompletedOffers(_pCurrentUser);
    CheckCompletedOffers(&vCompletedOffersPurchase);

    double dAmount2 = 15;
    double dPrice2 = 61;
    {
        SetCurrentUser(_vSellers[0]);
        AddOffer(dAmount2, dPrice2);
        CheckReplyOfferAdded(dAmount2, dPrice2);
        CheckBalance(_dInitialAmountUSD - dAmount1,
                     _dInitialAmountRUB + dPrice1 * dAmount1);
        CheckOffer(1, 0, dAmount2, dPrice2, 1);
    }
    _reply = RequestHandler::ShowUserCompletedOffers(_pCurrentUser);
    CheckCompletedOffers(&vCompletedOffersSale);

    {
        SetCurrentUser(_vBuyers[0]);
        AddOffer(dAmount2, dPrice2);
        CheckReplyOfferCompleted(dAmount2, dPrice2);
        CheckBalance(_dInitialAmountUSD + dAmount1 + dAmount2,
                     _dInitialAmountRUB - (dPrice1 * dAmount1 + dPrice2 * dAmount2));
        CheckListsOffersEmpty();
        vCompletedOffersPurchase.push_back(GenerateCompletedOfferLine(1, dAmount2, dPrice2));
    }
    _reply = RequestHandler::ShowUserCompletedOffers(_pCurrentUser);
    CheckCompletedOffers(&vCompletedOffersPurchase);
}
