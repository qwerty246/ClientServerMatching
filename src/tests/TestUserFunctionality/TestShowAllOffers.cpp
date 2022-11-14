#include "TestShowAllOffers.h"

void TestShowAllOffers::CheckNoOffers() const
{
    nlohmann::json expectedReply;
    expectedReply["UsersNum"] = "0";
    ASSERT_EQ(_reply, expectedReply);
}

void TestShowAllOffers::CheckAllOffers(bool bIsSaleOffer, double dPriceMax, double dPriceMin, double dPriceMean,
                                       const std::vector<std::string>* vOffersSale1,
                                       const std::vector<std::string>* vOffersSale2) const
{
    nlohmann::json expectedReply;
    expectedReply["UsersNum"] = "0";
    size_t uNum = 0;
    for (auto itOffer : *vOffersSale1)
    {
        if (expectedReply["UsersNum"] != "0")
        {
            size_t pos1 = itOffer.find("/");
            std::string str = itOffer.substr(0, pos1 + 1);
            RequestHandler::AddToField(expectedReply, "Username", str);
            size_t pos2 = itOffer.find("/", pos1 + 1);
            str = itOffer.substr(pos1 + 1, pos2 - pos1);
            RequestHandler::AddToField(expectedReply, "Amount", str);
            size_t pos3 = itOffer.find("/", pos2 + 1);
            str = itOffer.substr(pos2 + 1, pos3 - pos2);
            RequestHandler::AddToField(expectedReply, "Price", str);
            uNum++;
        }
        else
        {
            size_t pos1 = itOffer.find("/");
            std::string str = itOffer.substr(0, pos1 + 1);
            expectedReply["Username"] = str;
            size_t pos2 = itOffer.find("/", pos1 + 1);
            str = itOffer.substr(pos1 + 1, pos2 - pos1);
            expectedReply["Amount"] = str;
            size_t pos3 = itOffer.find("/", pos2 + 1);
            str = itOffer.substr(pos2 + 1, pos3 - pos2);
            expectedReply["Price"] = str;
            uNum++;
            expectedReply["UsersNum"] = "1";
        }
    }
    if (vOffersSale2)
    {
        for (auto itOffer : *vOffersSale2)
        {
            if (expectedReply["UsersNum"] != "0")
            {
                size_t pos1 = itOffer.find("/");
                std::string str = itOffer.substr(0, pos1 + 1);
                RequestHandler::AddToField(expectedReply, "Username", str);
                size_t pos2 = itOffer.find("/", pos1 + 1);
                str = itOffer.substr(pos1 + 1, pos2 - pos1);
                RequestHandler::AddToField(expectedReply, "Amount", str);
                size_t pos3 = itOffer.find("/", pos2 + 1);
                str = itOffer.substr(pos2 + 1, pos3 - pos2);
                RequestHandler::AddToField(expectedReply, "Price", str);
                uNum++;
            }
            else
            {
                size_t pos1 = itOffer.find("/");
                std::string str = itOffer.substr(0, pos1 + 1);
                expectedReply["Username"] = str;
                size_t pos2 = itOffer.find("/", pos1 + 1);
                str = itOffer.substr(pos1 + 1, pos2 - pos1);
                expectedReply["Amount"] = str;
                size_t pos3 = itOffer.find("/", pos2 + 1);
                str = itOffer.substr(pos2 + 1, pos3 - pos2);
                expectedReply["Price"] = str;
                uNum++;
                expectedReply["UsersNum"] = "1";
            }
        }
    }

    expectedReply["UsersNum"] = std::to_string(uNum);
    expectedReply["Max"] = std::to_string(dPriceMax);
    expectedReply["Min"] = std::to_string(dPriceMin);
    expectedReply["Mean"] = std::to_string(dPriceMean);
    ASSERT_EQ(_reply, expectedReply);
}

std::string TestShowAllOffers::GenerateOfferLine(const std::string& sUsername, double dAmount, double dPrice) const
{
    return sUsername + "/"
           + std::to_string(dAmount) + "/"
           + std::to_string(dPrice) + "/";
}



TEST_F(TestShowAllOffers, NoOffers)
{
    SetCurrentUser(_vSellers[0]);
    _reply = RequestHandler::ShowAllOffers(_pCurrentUser, true);
    CheckNoOffers();
    _reply = RequestHandler::ShowAllOffers(_pCurrentUser, false);
    CheckNoOffers();
}

TEST_F(TestShowAllOffers, ShowAllOffers)
{
    std::vector<std::string> vOffersSale1;
    double dAmountSale = 5;
    double dPriceSaleMin = 70;
    double dPriceSaleMax = 80;
    double dPriceSaleMean = (dPriceSaleMin + dPriceSaleMax) / 2;
    {
        SetCurrentUser(_vSellers[0]);
        AddOffer(dAmountSale, dPriceSaleMin);
        CheckReplyOfferAdded(dAmountSale, dPriceSaleMin);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmountSale, dPriceSaleMin, 0);
        vOffersSale1.push_back(GenerateOfferLine(_vSellers[0]->GetName(), dAmountSale, dPriceSaleMin));

        AddOffer(dAmountSale, dPriceSaleMax);
        CheckReplyOfferAdded(dAmountSale, dPriceSaleMax);
        CheckBalanceEqualToOriginal();
        CheckOffer(2, 0, dAmountSale, dPriceSaleMax, 1);
        vOffersSale1.push_back(GenerateOfferLine(_vSellers[0]->GetName(), dAmountSale, dPriceSaleMax));

        _reply = RequestHandler::ShowAllOffers(_pCurrentUser, true);
        CheckNoOffers();
        _reply = RequestHandler::ShowAllOffers(_pCurrentUser, false);
        CheckNoOffers();
    }

    std::vector<std::string> vOffersPurchase1;
    double dAmountPurchase = 5;
    double dPricePurchaseMin = 50;
    double dPricePurchaseMax = 60;
    double dPricePurchaseMean = (dPricePurchaseMin + dPricePurchaseMax) / 2;
    {
        SetCurrentUser(_vBuyers[0]);
        AddOffer(dAmountPurchase, dPricePurchaseMin);
        CheckReplyOfferAdded(dAmountPurchase, dPricePurchaseMin);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 1, dAmountPurchase, dPricePurchaseMin, 0);
        vOffersPurchase1.push_back(GenerateOfferLine(_vBuyers[0]->GetName(), dAmountPurchase, dPricePurchaseMin));

        AddOffer(dAmountPurchase, dPricePurchaseMax);
        CheckReplyOfferAdded(dAmountPurchase, dPricePurchaseMax);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 2, dAmountPurchase, dPricePurchaseMax, 1);
        vOffersPurchase1.push_back(GenerateOfferLine(_vBuyers[0]->GetName(), dAmountPurchase, dPricePurchaseMax));

        _reply = RequestHandler::ShowAllOffers(_pCurrentUser, true);
        CheckAllOffers(true, dPriceSaleMax, dPriceSaleMin, dPriceSaleMean, &vOffersSale1);
        _reply = RequestHandler::ShowAllOffers(_pCurrentUser, false);
        CheckNoOffers();
    }

    std::vector<std::string> vOffersSale2;
    {
        SetCurrentUser(_vSellers[1]);
        AddOffer(dAmountSale, dPriceSaleMin);
        CheckReplyOfferAdded(dAmountSale, dPriceSaleMin);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmountSale, dPriceSaleMin, 0);
        vOffersSale2.push_back(GenerateOfferLine(_vSellers[1]->GetName(), dAmountSale, dPriceSaleMin));

        AddOffer(dAmountSale, dPriceSaleMax);
        CheckReplyOfferAdded(dAmountSale, dPriceSaleMax);
        CheckBalanceEqualToOriginal();
        CheckOffer(2, 0, dAmountSale, dPriceSaleMax, 1);
        vOffersSale2.push_back(GenerateOfferLine(_vSellers[1]->GetName(), dAmountSale, dPriceSaleMax));

        _reply = RequestHandler::ShowAllOffers(_pCurrentUser, true);
        CheckAllOffers(true, dPriceSaleMax, dPriceSaleMin, dPriceSaleMean, &vOffersSale1);
        _reply = RequestHandler::ShowAllOffers(_pCurrentUser, false);
        CheckAllOffers(false, dPricePurchaseMax, dPricePurchaseMin, dPricePurchaseMean, &vOffersPurchase1);
    }

    std::vector<std::string> vOffersPurchase2;
    {
        SetCurrentUser(_vBuyers[1]);
        AddOffer(dAmountPurchase, dPricePurchaseMin);
        CheckReplyOfferAdded(dAmountPurchase, dPricePurchaseMin);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 1, dAmountPurchase, dPricePurchaseMin, 0);
        vOffersPurchase2.push_back(GenerateOfferLine(_vBuyers[1]->GetName(), dAmountPurchase, dPricePurchaseMin));

        AddOffer(dAmountPurchase, dPricePurchaseMax);
        CheckReplyOfferAdded(dAmountPurchase, dPricePurchaseMax);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 2, dAmountPurchase, dPricePurchaseMax, 1);
        vOffersPurchase2.push_back(GenerateOfferLine(_vBuyers[1]->GetName(), dAmountPurchase, dPricePurchaseMax));

        _reply = RequestHandler::ShowAllOffers(_pCurrentUser, true);
        CheckAllOffers(true, dPriceSaleMax, dPriceSaleMin, dPriceSaleMean, &vOffersSale1, &vOffersSale2);
        _reply = RequestHandler::ShowAllOffers(_pCurrentUser, false);
        CheckAllOffers(false, dPricePurchaseMax, dPricePurchaseMin, dPricePurchaseMean, &vOffersPurchase1);
    }
}
