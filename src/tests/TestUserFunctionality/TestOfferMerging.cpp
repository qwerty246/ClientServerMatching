#include "TestOfferMerging.h"

void TestOfferMerging::CheckReplyOfferAddedAndPartiallyMerged(double dAmount, double dRemainder, double dPrice) const
{
    nlohmann::json expectedReply;
    expectedReply["Accept"] = "1";
    expectedReply["Completed"] = "0";
    expectedReply["NumOffers"] = "1";
    expectedReply["FinalAmount"] = std::to_string(dRemainder);
    expectedReply["FinalPrice"] = std::to_string(dPrice);
    expectedReply["Amount"] = std::to_string(dAmount) + "/";
    expectedReply["Price"] = std::to_string(dPrice) + "/";
    expectedReply["Type"] = std::string(_bIsCurrentOfferSale ? "Sale" : "Purchase");
    ASSERT_EQ(_reply, expectedReply);
}

void TestOfferMerging::CheckReplyOfferAddedAndPartiallyMerged(double dAmount1, double dAmount2, double dRemainder,
                                                              double dPrice1, double dPrice2, double dFinalPrice) const
{
    nlohmann::json expectedReply;
    expectedReply["Accept"] = "1";
    expectedReply["Completed"] = "0";
    expectedReply["NumOffers"] = "2";
    expectedReply["FinalAmount"] = std::to_string(dRemainder);
    expectedReply["FinalPrice"] = std::to_string(dFinalPrice);
    expectedReply["Amount"] = std::to_string(dAmount1) + "/" + std::to_string(dAmount2) + "/";
    expectedReply["Price"] = std::to_string(dPrice1) + "/" + std::to_string(dPrice2) + "/";
    expectedReply["Type"] = std::string(_bIsCurrentOfferSale ? "Sale" : "Purchase");
    ASSERT_EQ(_reply, expectedReply);
}



TEST_F(TestOfferMerging, SuccessfullMerging_EqualAmounts1)
{
    double dAmount = 10;
    double dPrice = 60;
    {
        SetCurrentUser(_vSellers[0]);
        AddOffer(dAmount, dPrice);
        CheckReplyOfferAdded(dAmount, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmount, dPrice, 0);
    }

    {
        SetCurrentUser(_vBuyers[0]);
        AddOffer(dAmount, dPrice);
        CheckReplyOfferCompleted(dAmount, dPrice);
        CheckBalance(_dInitialAmountUSD + dAmount,
                     _dInitialAmountRUB - dPrice * dAmount);
        CheckListsOffersEmpty();
    }

    {
        SetCurrentUser(_vSellers[0]);
        CheckBalance(_dInitialAmountUSD - dAmount,
                     _dInitialAmountRUB + dPrice * dAmount);
        CheckListsOffersEmpty();
    }
}

TEST_F(TestOfferMerging, SuccessfullMerging_EqualAmounts2)
{
    double dAmount = 10;
    double dPrice = 60;
    {
        SetCurrentUser(_vBuyers[0]);
        AddOffer(dAmount, dPrice);
        CheckReplyOfferAdded(dAmount, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 1, dAmount, dPrice, 0);
    }

    {
        SetCurrentUser(_vSellers[0]);
        AddOffer(dAmount, dPrice);
        CheckReplyOfferCompleted(dAmount, dPrice);
        CheckBalance(_dInitialAmountUSD - dAmount,
                     _dInitialAmountRUB + dPrice * dAmount);
        CheckListsOffersEmpty();
    }

    {
        SetCurrentUser(_vBuyers[0]);
        CheckBalance(_dInitialAmountUSD + dAmount,
                     _dInitialAmountRUB - dPrice * dAmount);
        CheckListsOffersEmpty();
    }
}

TEST_F(TestOfferMerging, SuccessfullMerging_SaleOfferAmountLarger1)
{
    double dAmountSale = 11;
    double dPrice = 60;
    OfferPtr pOffer;
    {
        SetCurrentUser(_vSellers[0]);
        AddOffer(dAmountSale, dPrice);
        pOffer = _pCurrentOffer;
        CheckReplyOfferAdded(dAmountSale, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmountSale, dPrice, 0);
    }

    double dAmountPurchase = 10;
    {
        SetCurrentUser(_vBuyers[0]);
        AddOffer(dAmountPurchase, dPrice);
        CheckReplyOfferCompleted(dAmountPurchase, dPrice);
        CheckBalance(_dInitialAmountUSD + dAmountPurchase,
                     _dInitialAmountRUB - dPrice * dAmountPurchase);
        CheckListsOffersEmpty();
    }

    {
        SetCurrentUser(_vSellers[0]);
        CheckBalance(_dInitialAmountUSD - dAmountPurchase,
                     _dInitialAmountRUB + dPrice * dAmountPurchase);
        CheckOffer(1, 0, dAmountSale - dAmountPurchase, dPrice, 0, pOffer);
    }
}

TEST_F(TestOfferMerging, SuccessfullMerging_SaleOfferAmountLarger2)
{
    double dAmountPurchase = 10;
    double dPrice = 60;
    {
        SetCurrentUser(_vBuyers[0]);
        AddOffer(dAmountPurchase, dPrice);
        CheckReplyOfferAdded(dAmountPurchase, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 1, dAmountPurchase, dPrice, 0);
    }

    double dAmountSale = 11;
    {
        SetCurrentUser(_vSellers[0]);
        AddOffer(dAmountSale, dPrice);
        CheckReplyOfferAddedAndPartiallyMerged(dAmountPurchase,
                                               dAmountSale - dAmountPurchase,
                                               dPrice);
        CheckBalance(_dInitialAmountUSD - dAmountPurchase,
                     _dInitialAmountRUB + dPrice * dAmountPurchase);
        CheckOffer(1, 0, dAmountSale - dAmountPurchase, dPrice, 0);
    }

    {
        SetCurrentUser(_vBuyers[0]);
        CheckBalance(_dInitialAmountUSD + dAmountPurchase,
                     _dInitialAmountRUB - dPrice * dAmountPurchase);
        CheckListsOffersEmpty();
    }
}

TEST_F(TestOfferMerging, SuccessfullMerging_SaleOfferPriceLarger1)
{
    double dAmount = 10;
    double dPriceSale = 61;
    OfferPtr pOffer;
    {
        SetCurrentUser(_vSellers[0]);
        AddOffer(dAmount, dPriceSale);
        pOffer = _pCurrentOffer;
        CheckReplyOfferAdded(dAmount, dPriceSale);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmount, dPriceSale, 0);
    }

    double dPricePurchase = 60;
    {
        SetCurrentUser(_vBuyers[0]);
        AddOffer(dAmount, dPricePurchase);
        CheckReplyOfferAdded(dAmount, dPricePurchase);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 1,
                   dAmount, dPricePurchase, 0);
    }

    {
        SetCurrentUser(_vSellers[0]);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmount, dPriceSale, 0, pOffer);
    }
}

TEST_F(TestOfferMerging, SuccessfullMerging_SaleOfferPriceLarger2)
{
    double dAmount = 10;
    double dPricePurchase = 60;
    OfferPtr pOffer;
    {
        SetCurrentUser(_vBuyers[0]);
        AddOffer(dAmount, dPricePurchase);
        pOffer = _pCurrentOffer;
        CheckReplyOfferAdded(dAmount, dPricePurchase);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 1, dAmount, dPricePurchase, 0);
    }

    double dPriceSale = 61;
    {
        SetCurrentUser(_vSellers[0]);
        AddOffer(dAmount, dPriceSale);
        CheckReplyOfferAdded(dAmount, dPriceSale);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmount, dPriceSale, 0);
    }

    {
        SetCurrentUser(_vBuyers[0]);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 1, dAmount, dPricePurchase, 0, pOffer);
    }
}

TEST_F(TestOfferMerging, SuccessfullMerging_PurchaseOfferAmountLarger1)
{
    double dAmountSale = 10;
    double dPrice = 60;
    {
        SetCurrentUser(_vSellers[0]);
        AddOffer(dAmountSale, dPrice);
        CheckReplyOfferAdded(dAmountSale, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmountSale, dPrice, 0);
    }

    double dAmountPurchase = 11;
    {
        SetCurrentUser(_vBuyers[0]);
        AddOffer(dAmountPurchase, dPrice);
        CheckReplyOfferAddedAndPartiallyMerged(dAmountSale, dAmountPurchase - dAmountSale, dPrice);
        CheckBalance(_dInitialAmountUSD + dAmountSale,
                     _dInitialAmountRUB - dPrice * dAmountSale);
        CheckOffer(0, 1, dAmountPurchase - dAmountSale, dPrice, 0);
    }

    {
        SetCurrentUser(_vSellers[0]);
        CheckBalance(_dInitialAmountUSD - dAmountSale,
                     _dInitialAmountRUB + dPrice * dAmountSale);
        CheckListsOffersEmpty();
    }
}

TEST_F(TestOfferMerging, SuccessfullMerging_PurchaseOfferAmountLarger2)
{
    double dAmountPurchase = 11;
    double dPrice = 60;
    OfferPtr pOffer;
    {
        SetCurrentUser(_vBuyers[0]);
        AddOffer(dAmountPurchase, dPrice);
        pOffer = _pCurrentOffer;
        CheckReplyOfferAdded(dAmountPurchase, dPrice);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 1, dAmountPurchase, dPrice, 0);
    }

    double dAmountSale = 10;
    {
        SetCurrentUser(_vSellers[0]);
        AddOffer(dAmountSale, dPrice);
        CheckReplyOfferCompleted(dAmountSale, dPrice);
        CheckBalance(_dInitialAmountUSD - dAmountSale,
                     _dInitialAmountRUB + dPrice * dAmountSale);
        CheckListsOffersEmpty();
    }

    {
        SetCurrentUser(_vBuyers[0]);
        CheckBalance(_dInitialAmountUSD + dAmountSale,
                     _dInitialAmountRUB - dPrice * dAmountSale);
        CheckOffer(0, 1, dAmountPurchase - dAmountSale, dPrice, 0, pOffer);
    }
}

TEST_F(TestOfferMerging, SuccessfullMerging_PurchaseOfferPriceLarger1)
{
    double dAmount = 10;
    double dPriceSale = 60;
    OfferPtr pOffer;
    {
        SetCurrentUser(_vSellers[0]);
        AddOffer(dAmount, dPriceSale);
        pOffer = _pCurrentOffer;
        CheckReplyOfferAdded(dAmount, dPriceSale);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmount, dPriceSale, 0);
    }

    double dPricePurchase = 61;
    {
        SetCurrentUser(_vBuyers[0]);
        AddOffer(dAmount, dPricePurchase);
        CheckReplyOfferCompleted(dAmount, dPriceSale);
        CheckBalance(_dInitialAmountUSD + dAmount,
                     _dInitialAmountRUB - dPriceSale * dAmount);
        CheckListsOffersEmpty();
    }

    {
        SetCurrentUser(_vSellers[0]);
        CheckBalance(_dInitialAmountUSD - dAmount,
                     _dInitialAmountRUB + dPriceSale * dAmount);
        CheckOffer(0, 0, dAmount, dPriceSale, 1, pOffer);
    }
}

TEST_F(TestOfferMerging, SuccessfullMerging_PurchaseOfferPriceLarger2)
{
    double dAmount = 10;
    double dPricePurchase = 61;
    OfferPtr pOffer;
    {
        SetCurrentUser(_vBuyers[0]);
        AddOffer(dAmount, dPricePurchase);
        pOffer = _pCurrentOffer;
        CheckReplyOfferAdded(dAmount, dPricePurchase);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 1, dAmount, dPricePurchase, 0);
    }

    double dPriceSale = 60;
    {
        SetCurrentUser(_vSellers[0]);
        AddOffer(dAmount, dPriceSale);
        CheckReplyOfferCompleted(dAmount, dPricePurchase);
        CheckBalance(_dInitialAmountUSD - dAmount,
                     _dInitialAmountRUB + dPricePurchase * dAmount);
        CheckListsOffersEmpty();
    }

    {
        SetCurrentUser(_vBuyers[0]);
        CheckBalance(_dInitialAmountUSD + dAmount,
                     _dInitialAmountRUB - dPricePurchase * dAmount);
        CheckOffer(0, 0, dAmount, dPricePurchase, 1, pOffer);
    }
}

TEST_F(TestOfferMerging, SuccessfullMerging_MakeDialBetweenThreeUsers1)
{
    double dAmountPurchase1 = 10;
    double dPricePurchase1 = 62;
    {
        SetCurrentUser(_vBuyers[0]);
        AddOffer(dAmountPurchase1, dPricePurchase1);
        CheckReplyOfferAdded(dAmountPurchase1, dPricePurchase1);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 1, dAmountPurchase1, dPricePurchase1, 0);
    }

    double dAmountPurchase2 = 21;
    double dPricePurchase2 = 63;
    {
        SetCurrentUser(_vBuyers[1]);
        AddOffer(dAmountPurchase2, dPricePurchase2);
        CheckReplyOfferAdded(dAmountPurchase2, dPricePurchase2);
        CheckBalanceEqualToOriginal();
        CheckOffer(0, 1, dAmountPurchase2, dPricePurchase2, 0);
    }

    double dAmountSale = 50;
    double dPriceSale = 61;
    {
        SetCurrentUser(_vSellers[0]);
        AddOffer(dAmountSale, dPriceSale);
        CheckReplyOfferAddedAndPartiallyMerged(dAmountPurchase2,
                                               dAmountPurchase1,
                                               dAmountSale - dAmountPurchase2 - dAmountPurchase1,
                                               dPricePurchase2,
                                               dPricePurchase1,
                                               dPriceSale);
        CheckBalance(_dInitialAmountUSD - (dAmountPurchase1 + dAmountPurchase2),
                     _dInitialAmountRUB + dPricePurchase1 * dAmountPurchase1 + dPricePurchase2 * dAmountPurchase2);
        CheckOffer(1, 0, dAmountSale - dAmountPurchase1 - dAmountPurchase2, dPriceSale, 0);
    }

    {
        SetCurrentUser(_vBuyers[0]);
        CheckBalance(_dInitialAmountUSD + dAmountPurchase1,
                     _dInitialAmountRUB - dPricePurchase1 * dAmountPurchase1);
        CheckListsOffersEmpty();
    }

    {
        SetCurrentUser(_vBuyers[1]);
        CheckBalance(_dInitialAmountUSD + dAmountPurchase2,
                     _dInitialAmountRUB - dPricePurchase2 * dAmountPurchase2);
        CheckListsOffersEmpty();
    }
}

TEST_F(TestOfferMerging, SuccessfullMerging_MakeDialBetweenThreeUsers2)
{
    double dAmountSale1 = 10.23;
    double dPriceSale1 = 59.41;
    OfferPtr pOfferSale1;
    {
        SetCurrentUser(_vSellers[0]);
        AddOffer(dAmountSale1, dPriceSale1);
        pOfferSale1 = _pCurrentOffer;
        CheckReplyOfferAdded(dAmountSale1, dPriceSale1);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmountSale1, dPriceSale1, 0);
    }

    double dAmountSale2 = 20.58;
    double dPriceSale2 = 58.76;
    OfferPtr pOfferSale2;
    {
        SetCurrentUser(_vSellers[1]);
        AddOffer(dAmountSale2, dPriceSale2);
        pOfferSale2 = _pCurrentOffer;
        CheckReplyOfferAdded(dAmountSale2, dPriceSale2);
        CheckBalanceEqualToOriginal();
        CheckOffer(1, 0, dAmountSale2, dPriceSale2, 0);
    }

    double dAmountPurchase = 50.03;
    double dPricePurchase = 61.74;
    {
        SetCurrentUser(_vBuyers[0]);
        AddOffer(dAmountPurchase, dPricePurchase);
        CheckReplyOfferAddedAndPartiallyMerged(dAmountSale2,
                                               dAmountSale1,
                                               dAmountPurchase - dAmountSale2 - dAmountSale1,
                                               dPriceSale2,
                                               dPriceSale1,
                                               dPricePurchase);
        CheckBalance(_dInitialAmountUSD + dAmountSale1 + dAmountSale2,
                     _dInitialAmountRUB - dPriceSale1 * dAmountSale1 - dPriceSale2 * dAmountSale2);
        CheckOffer(0, 1, dAmountPurchase - (dAmountSale1 + dAmountSale2), dPricePurchase, 0);
    }

    {
        SetCurrentUser(_vSellers[0]);
        CheckBalance(_dInitialAmountUSD - dAmountSale1,
                     _dInitialAmountRUB + dPriceSale1 * dAmountSale1);
        CheckListsOffersEmpty();
    }

    {
        SetCurrentUser(_vSellers[1]);
        CheckBalance(_dInitialAmountUSD - dAmountSale2,
                     _dInitialAmountRUB + dPriceSale2 * dAmountSale2);
        CheckListsOffersEmpty();
    }
}
