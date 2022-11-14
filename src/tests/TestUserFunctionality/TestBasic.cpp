#include "TestBasic.h"
#include <hpp/Common.hpp>

TestBasic::TestBasic() :
    _sPassword("password"),
    _dInitialAmountUSD(100),
    _dInitialAmountRUB(6000)
{
}

void TestBasic::SetUp()
{
    nlohmann::json expectedReply;
    for (size_t i = 0; i < 2; i++)
    {
        ASSERT_EQ(RequestHandler::RegisterNewUser("Username" + std::to_string(i), _sPassword), std::to_string(i));
        _vSellers.push_back(Core::GetCore().GetUser("Username" + std::to_string(i)));
        expectedReply["Amount"] = std::to_string(_dInitialAmountUSD);
        ASSERT_EQ(RequestHandler::ReplenishUSD(_vSellers.back(), _dInitialAmountUSD), expectedReply);
        expectedReply["Amount"] = std::to_string(_dInitialAmountRUB);
        ASSERT_EQ(RequestHandler::ReplenishRUB(_vSellers.back(), _dInitialAmountRUB), expectedReply);
    }
    for (size_t i = 2; i < 4; i++)
    {
        ASSERT_EQ(RequestHandler::RegisterNewUser("Username" + std::to_string(i), _sPassword), std::to_string(i));
        _vBuyers.push_back(Core::GetCore().GetUser("Username" + std::to_string(i)));
        expectedReply["Amount"] = std::to_string(_dInitialAmountUSD);
        ASSERT_EQ(RequestHandler::ReplenishUSD(_vBuyers.back(), _dInitialAmountUSD), expectedReply);
        expectedReply["Amount"] = std::to_string(_dInitialAmountRUB);
        ASSERT_EQ(RequestHandler::ReplenishRUB(_vBuyers.back(), _dInitialAmountRUB), expectedReply);
    }
}

void TestBasic::TearDown()
{
    while (!_vSellers.empty())
    {
        RequestHandler::RemoveUser(_vSellers.back()->GetName(), _sPassword);
        _vSellers.pop_back();
    }
    while (!_vBuyers.empty())
    {
        RequestHandler::RemoveUser(_vBuyers.back()->GetName(), _sPassword);
        _vBuyers.pop_back();
    }
    _pCurrentOffer.reset();
    _pCurrentUser.reset();
}



void TestBasic::SetCurrentUser(const UserPtr& pUser)
{
    for (auto itUser : _vSellers)
    {
        if (itUser == pUser)
        {
            _bIsCurrentOfferSale = true;
            _pCurrentUser = pUser;
            return;
        }
    }
    _bIsCurrentOfferSale = false;
    _pCurrentUser = pUser;
}

void TestBasic::AddOffer(double dAmount, double dPrice)
{
    if (_bIsCurrentOfferSale)
    {
        _pCurrentOffer = _pCurrentUser->CreateOfferSale(dAmount, dPrice);
    }
    else
    {
        _pCurrentOffer = _pCurrentUser->CreateOfferPurchase(dAmount, dPrice);
    }
    _reply.clear();
    RequestHandler::AddOfferAndTryToMatch(_reply, _pCurrentUser, _pCurrentOffer);
}

void TestBasic::CheckReplyOfferAdded(double dAmount, double dPrice) const
{
    nlohmann::json expectedReply;
    expectedReply["Accept"] = "1";
    expectedReply["Completed"] = "0";
    expectedReply["NumOffers"] = "0";
    expectedReply["FinalAmount"] = std::to_string(dAmount);
    expectedReply["FinalPrice"] = std::to_string(dPrice);
    ASSERT_EQ(_reply, expectedReply);
}

void TestBasic::CheckReplyOfferCompleted(double dAmount, double dPrice) const
{
    nlohmann::json expectedReply;
    expectedReply["Accept"] = "0";
    expectedReply["Completed"] = "1";
    expectedReply["NumOffers"] = "0";
    expectedReply["Amount"] = std::to_string(dAmount) + "/";
    expectedReply["Price"] = std::to_string(dPrice) + "/";
    expectedReply["Type"] = std::string(_bIsCurrentOfferSale ? "Sale" : "Purchase");
    ASSERT_EQ(_reply, expectedReply);
}

void TestBasic::CheckBalance(double dAmountUSD, double dAmountRUB) const
{
    ASSERT_EQ(_pCurrentUser->GetAmountUSD(), dAmountUSD);
    ASSERT_EQ(_pCurrentUser->GetAmountRUB(), dAmountRUB);
}

void TestBasic::CheckBalanceEqualToOriginal() const
{
    ASSERT_EQ(_pCurrentUser->GetAmountUSD(), _dInitialAmountUSD);
    ASSERT_EQ(_pCurrentUser->GetAmountRUB(), _dInitialAmountRUB);
}

void TestBasic::CheckListsOffers(size_t uListSaleSize, size_t uListPurchaseSize) const
{
    ASSERT_EQ(std::size(_pCurrentUser->GetOffersSale()), uListSaleSize);
    ASSERT_EQ(std::size(_pCurrentUser->GetOffersPurchase()), uListPurchaseSize);
}

void TestBasic::CheckListsOffersEmpty() const
{
    ASSERT_TRUE(_pCurrentUser->GetOffersSale().empty());
    ASSERT_TRUE(_pCurrentUser->GetOffersPurchase().empty());
}

void TestBasic::CheckOffer(size_t uListSaleSize,
                           size_t uListPurchaseSize,
                           double dAmount,
                           double dPrice,
                           size_t uOfferId,
                           OfferPtr pOffer) const
{
    CheckListsOffers(uListSaleSize, uListPurchaseSize);

    if (uListSaleSize || uListPurchaseSize)
    {
        if (!pOffer)
        {
            pOffer = _pCurrentOffer;
        }

        if (_bIsCurrentOfferSale)
        {
            ASSERT_EQ(_pCurrentUser->GetOffersSale().back(), pOffer);
        }
        else
        {
            ASSERT_EQ(_pCurrentUser->GetOffersPurchase().back(), pOffer);
        }

        std::string sOfferId = std::to_string(_pCurrentUser->GetId()) +
                               (_bIsCurrentOfferSale ? "_s_" : "_p_") +
                               std::to_string(uOfferId);
        OfferPtr pExpectedOffer((new Offer(std::to_string(_pCurrentUser->GetId()),
                                           sOfferId,
                                           dAmount,
                                           dPrice,
                                           (_bIsCurrentOfferSale ? true : false))));
        ASSERT_TRUE(*pOffer == *pExpectedOffer);
    }
}
