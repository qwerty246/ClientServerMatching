#include "TestOfferCreation.h"

std::string TestOfferCreation::GenerateOfferId(size_t uOfferId)
{
    return RequestHandler::GenerateOfferId(_pCurrentUser->GetId(),
                                           _bIsCurrentOfferSale ? "_s_" : "_p_",
                                           uOfferId);
}

void TestOfferCreation::RemoveOffer(const std::string& sOfferId)
{
    _reply = RequestHandler::RemoveOffer(_pCurrentUser, sOfferId);
}

void TestOfferCreation::CheckReplyOfferRemoved(std::string_view sOfferId) const
{
    nlohmann::json expectedReply;
    expectedReply["OfferId"] = sOfferId;
    ASSERT_EQ(_reply, expectedReply);
}

void TestOfferCreation::CheckReplyOfferNotExist() const
{
    nlohmann::json expectedReply;
    expectedReply["OfferId"] = "0";
    ASSERT_EQ(_reply, expectedReply);
}

void TestOfferCreation::CheckReplyOfferIncorrectId() const
{
    nlohmann::json expectedReply;
    expectedReply["OfferId"] = "-1";
    ASSERT_EQ(_reply, expectedReply);
}



TEST_F(TestOfferCreation, SuccessfullAddingOffer_Sale)
{
    {
        SetCurrentUser(_vSellers[0]);
        double dAmount = 15;
        double dPrice = 60;
        AddOffer(dAmount, dPrice);
        CheckReplyOfferAdded(dAmount, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmount, dPrice, 0);
    }

    {
        SetCurrentUser(_vSellers[1]);
        double dAmount1 = 2;
        double dPrice = 61;
        AddOffer(dAmount1, dPrice);
        CheckReplyOfferAdded(dAmount1, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmount1, dPrice, 0);

        double dAmount2 = 10;
        dPrice = 59;
        AddOffer(dAmount2, dPrice);
        CheckReplyOfferAdded(dAmount2, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(2, 0, dAmount2, dPrice, 1);
    }
}

TEST_F(TestOfferCreation, SuccessfullAddingOffer_Purchase)
{
    {
        SetCurrentUser(_vBuyers[0]);
        double dAmount = 15;
        double dPrice = 60;
        AddOffer(dAmount, dPrice);
        CheckReplyOfferAdded(dAmount, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 1, dAmount, dPrice, 0);
    }

    {
        SetCurrentUser(_vBuyers[1]);
        double dAmount1 = 2;
        double dPrice1 = 61;
        AddOffer(dAmount1, dPrice1);
        CheckReplyOfferAdded(dAmount1, dPrice1);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 1, dAmount1, dPrice1, 0);

        double dAmount2 = 10;
        double dPrice2 = 59;
        AddOffer(dAmount2, dPrice2);
        CheckReplyOfferAdded(dAmount2, dPrice2);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 2, dAmount2, dPrice2, 1);
    }
}

TEST_F(TestOfferCreation, SuccessfullAddingOffer_SaleTypeDouble)
{
    {
        SetCurrentUser(_vSellers[0]);
        double dAmount = 15.01;
        double dPrice = 60.78;
        AddOffer(dAmount, dPrice);
        CheckReplyOfferAdded(dAmount, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmount, dPrice, 0);
    }

    {
        SetCurrentUser(_vSellers[1]);
        double dAmount1 = 2.6;
        double dPrice = 61.98;
        AddOffer(dAmount1, dPrice);
        CheckReplyOfferAdded(dAmount1, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmount1, dPrice, 0);
    }
}

TEST_F(TestOfferCreation, SuccessfullAddingOffer_PurchaseTypeDouble)
{
    {
        SetCurrentUser(_vBuyers[0]);
        double dAmount = 15.07;
        double dPrice = 60.12;
        AddOffer(dAmount, dPrice);
        CheckReplyOfferAdded(dAmount, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 1, dAmount, dPrice, 0);
    }

    {
        SetCurrentUser(_vBuyers[1]);
        double dAmount1 = 2.96;
        double dPrice1 = 61.34;
        AddOffer(dAmount1, dPrice1);
        CheckReplyOfferAdded(dAmount1, dPrice1);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 1, dAmount1, dPrice1, 0);
    }
}

TEST_F(TestOfferCreation, SuccessfullAddingOffers_FromOneUser)
{
    SetCurrentUser(_vSellers[0]);
    double dAmountUSD1 = 5;
    double dPriceUSD1 = 62;
    AddOffer(dAmountUSD1, dPriceUSD1);
    CheckReplyOfferAdded(dAmountUSD1, dPriceUSD1);
    CheckBalanceEqualToOriginal();
    CheckOffer(1, 0, dAmountUSD1, dPriceUSD1, 0);

    _bIsCurrentOfferSale = false;
    double dAmountRUB1 = 10;
    double dPriceRUB1 = 58;
    AddOffer(dAmountRUB1, dPriceRUB1);
    CheckReplyOfferAdded(dAmountRUB1, dPriceRUB1);
    CheckBalanceEqualToOriginal();
    CheckOffer(1, 1, dAmountRUB1, dPriceRUB1, 0);

    _bIsCurrentOfferSale = true;
    double dAmountUSD2 = 4;
    double dPriceUSD2 = 64;
    AddOffer(dAmountUSD2, dPriceUSD2);
    CheckReplyOfferAdded(dAmountUSD2, dPriceUSD2);
    CheckBalanceEqualToOriginal();
    CheckOffer(2, 1, dAmountUSD2, dPriceUSD2, 1);

    _bIsCurrentOfferSale = false;
    double dAmountRUB2 = 7;
    double dPriceRUB2 = 56;
    AddOffer(dAmountRUB2, dPriceRUB2);
    CheckReplyOfferAdded(dAmountRUB2, dPriceRUB2);
    CheckBalanceEqualToOriginal();
    CheckOffer(2, 2, dAmountRUB2, dPriceRUB2, 1);
}

TEST_F(TestOfferCreation, SuccessfullAddingOffer_RemoveOffers)
{
    {
        SetCurrentUser(_vSellers[0]);
        double dAmount1 = 10;
        double dPrice1 = 60;
        AddOffer(dAmount1, dPrice1);
        CheckReplyOfferAdded(dAmount1, dPrice1);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmount1, dPrice1, 0);

        double dAmount2 = 20;
        double dPrice2 = 70;
        AddOffer(dAmount2, dPrice2);
        CheckReplyOfferAdded(dAmount2, dPrice2);
        CheckBalanceEqualToOriginal();
        CheckOffer(2, 0, dAmount2, dPrice2, 1);
    }

    {
        SetCurrentUser(_vSellers[1]);
        double dAmount1 = 11;
        double dPrice1 = 61;
        AddOffer(dAmount1, dPrice1);
        CheckReplyOfferAdded(dAmount1, dPrice1);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmount1, dPrice1, 0);

        double dAmount2 = 21;
        double dPrice2 = 71;
        AddOffer(dAmount2, dPrice2);
        CheckReplyOfferAdded(dAmount2, dPrice2);
        CheckBalanceEqualToOriginal();
        CheckOffer(2, 0, dAmount2, dPrice2, 1);
    }

    SetCurrentUser(_vSellers[0]);
    std::string sOfferId = GenerateOfferId(1);
    RemoveOffer(sOfferId);
    CheckReplyOfferRemoved(sOfferId);
    CheckListsOffers(1, 0);

    SetCurrentUser(_vSellers[1]);
    sOfferId = GenerateOfferId(0);
    RemoveOffer(sOfferId);
    CheckReplyOfferRemoved(sOfferId);
    CheckListsOffers(1, 0);

    SetCurrentUser(_vSellers[1]);
    sOfferId = GenerateOfferId(1);
    RemoveOffer(sOfferId);
    CheckReplyOfferRemoved(sOfferId);
    CheckListsOffersEmpty();

    {
        SetCurrentUser(_vSellers[0]);
        double dAmount = 30;
        double dPrice = 80;
        AddOffer(dAmount, dPrice);
        CheckReplyOfferAdded(dAmount, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(2, 0, dAmount, dPrice, 2);
    }

    {
        SetCurrentUser(_vSellers[1]);
        double dAmount = 31;
        double dPrice = 81;
        AddOffer(dAmount, dPrice);
        CheckReplyOfferAdded(dAmount, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmount, dPrice, 2);
    }

    SetCurrentUser(_vSellers[0]);
    sOfferId = GenerateOfferId(0);
    RemoveOffer(sOfferId);
    CheckReplyOfferRemoved(sOfferId);
    CheckListsOffers(1, 0);

    SetCurrentUser(_vSellers[1]);
    sOfferId = GenerateOfferId(2);
    RemoveOffer(sOfferId);
    CheckReplyOfferRemoved(sOfferId);
    CheckListsOffersEmpty();

    SetCurrentUser(_vSellers[0]);
    sOfferId = GenerateOfferId(2);
    RemoveOffer(sOfferId);
    CheckReplyOfferRemoved(sOfferId);
    CheckListsOffersEmpty();
}

TEST_F(TestOfferCreation, SuccessfullAddingOffer_RemoveNonExistentOffers)
{
    SetCurrentUser(_vSellers[0]);
    double dAmount = 10;
    double dPrice = 60;
    AddOffer(dAmount, dPrice);
    CheckReplyOfferAdded(dAmount, dPrice);
    CheckBalanceEqualToOriginal();
    CheckOffer(1, 0, dAmount, dPrice, 0);

    std::string sOfferId = GenerateOfferId(1);
    RemoveOffer(sOfferId);
    CheckReplyOfferNotExist();
    CheckListsOffers(1, 0);

    sOfferId = GenerateOfferId(0);
    RemoveOffer(sOfferId);
    CheckReplyOfferRemoved(sOfferId);
    CheckListsOffersEmpty();

    RemoveOffer(sOfferId);
    CheckReplyOfferNotExist();
    CheckListsOffersEmpty();

    RemoveOffer("0_s_100");
    CheckReplyOfferNotExist();
    CheckListsOffersEmpty();

    RemoveOffer("100_s_0");
    CheckReplyOfferNotExist();
    CheckListsOffersEmpty();
}

TEST_F(TestOfferCreation, SuccessfullAddingOffer_RemoveOffersIncorrectId)
{
    SetCurrentUser(_vSellers[0]);
    double dAmount = 10;
    double dPrice = 60;
    AddOffer(dAmount, dPrice);
    CheckReplyOfferAdded(dAmount, dPrice);
    CheckBalanceEqualToOriginal();
    CheckOffer(1, 0, dAmount, dPrice, 0);

    RemoveOffer("0_0");
    CheckReplyOfferIncorrectId();
    CheckListsOffers(1, 0);

    RemoveOffer("_0_s_0");
    CheckReplyOfferIncorrectId();
    CheckListsOffers(1, 0);

    RemoveOffer("0_s_0_");
    CheckReplyOfferIncorrectId();
    CheckListsOffers(1, 0);

    RemoveOffer("0_s_");
    CheckReplyOfferIncorrectId();
    CheckListsOffers(1, 0);

    RemoveOffer("_s_0");
    CheckReplyOfferIncorrectId();
    CheckListsOffers(1, 0);

    RemoveOffer("0_a_0");
    CheckReplyOfferIncorrectId();
    CheckListsOffers(1, 0);

    RemoveOffer("abc");
    CheckReplyOfferIncorrectId();
    CheckListsOffers(1, 0);

    RemoveOffer("0");
    CheckReplyOfferIncorrectId();
    CheckListsOffers(1, 0);
}
