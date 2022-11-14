#pragma once

#include "Balance.h"
#include "Offer.h"

#include <string>
#include <string_view>
#include <list>
#include <memory>

class User
{
public:
    User(std::string_view sName,
         std::string_view sPassword,
         size_t uId);
    ~User() = default;

    const std::string& GetName() const;
    const std::string& GetPassword() const;
    size_t GetId() const;
    const std::list<OfferPtr>& GetOffersSale() const;
    const std::list<OfferPtr>& GetOffersPurchase() const;
    double GetAmountUSD() const;
    double GetAmountRUB() const;
    const std::list<OfferPtr>& GetCompletedpOffers() const;

    void AddOffer(const OfferPtr& pOffer);
    void RemoveOffer(const std::list<OfferPtr>::const_iterator& itOffer);
    void AddCompletedOffer(const OfferPtr& pOffer);
    void ReplenishUSD(double dAmount);
    void ReplenishRUB(double dAmount);
    void RemoveUSD(double dAmount);
    void RemoveRUB(double dAmount);

    OfferPtr CreateOfferSale(double dAmount, double dPrice);
    OfferPtr CreateOfferPurchase(double dAmount, double dPrice);
private:
    const std::string _sName;
    const std::string _sPassword;
    const size_t _uId;
    Balance _pBalanceUSD;
    Balance _pBalanceRUB;
    std::list<OfferPtr> _lOffersSale;
    std::list<OfferPtr> _lOffersPurchase;
    std::list<OfferPtr> _lCompletedOffers;
    size_t _uCounterOffersSale;
    size_t _uCounterOffersPurchase;
};

typedef std::shared_ptr<User> UserPtr;
