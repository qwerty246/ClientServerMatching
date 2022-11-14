#pragma once

#include <memory>
#include <string>

class Offer;

typedef std::shared_ptr<Offer> OfferPtr;

class Offer
{
public:
    Offer(std::string_view sUserId,
          std::string_view sOfferId,
          double dAmount,
          double dPrice,
          bool bIsSaleOffer);
    virtual ~Offer() = default;

    const std::string& GetUserId() const;
    const std::string& GetId() const;
    double GetAmount() const;
    double GetOriginalAmount() const;
    double GetPrice() const;
    bool IsSaleOffer() const;

    void RemoveAmount(double dAmount);
    bool IsMoreProfitable(const OfferPtr& pOffer) const;
    bool operator==(const Offer& offer);
private:
    const std::string _sUserId;
    const std::string _sId;
    double _dAmount;
    const double _dOriginalAmount;
    const double _dPrice;
    const bool _bIsSaleOffer;
};

class OfferSale : public Offer
{
public:
    OfferSale(const std::string& sUserId,
              size_t uOfferId,
              double dAmount,
              double dPrice);
    ~OfferSale() = default;
};

class OfferPurchase : public Offer
{
public:
    OfferPurchase(const std::string& sUserId,
                  size_t uOfferId,
                  double dAmount,
                  double dPrice);
    ~OfferPurchase() = default;
};

typedef std::shared_ptr<Offer> OfferPtr;
