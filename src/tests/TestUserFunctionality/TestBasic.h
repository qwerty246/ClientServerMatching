#pragma once

#include <gtest/gtest.h>
#include <server/Core.h>
#include <server/RequestHandler.h>
#include <hpp/json.hpp>

class TestBasic : public testing::Test
{
public:
    TestBasic();
    virtual ~TestBasic() = default;
protected:
    void SetUp() override;
    void TearDown() override;

    void SetCurrentUser(const UserPtr& pUser);
    void AddOffer(double dAmount, double dPrice);
    void CheckReplyOfferAdded(double dAmount, double dPrice) const;
    void CheckReplyOfferCompleted(double dAmount, double dPrice) const;
    void CheckBalance(double dAmountUSD, double dAmountRUB) const;
    void CheckBalanceEqualToOriginal() const;
    void CheckListsOffers(size_t uListSaleSize, size_t uListPurchaseSize) const;
    void CheckListsOffersEmpty() const;
    void CheckOffer(size_t uListSaleSize,
                    size_t uListPurchaseSize,
                    double dAmount,
                    double dPrice,
                    size_t uOfferId,
                    OfferPtr pOffer = nullptr) const;
protected:
    const std::string _sPassword;
    const double _dInitialAmountUSD;
    const double _dInitialAmountRUB;

    std::vector<UserPtr> _vSellers;
    std::vector<UserPtr> _vBuyers;
    bool _bIsCurrentOfferSale;
    UserPtr _pCurrentUser;
    OfferPtr _pCurrentOffer;
    nlohmann::json _reply;
};
