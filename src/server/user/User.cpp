#include "User.h"

User::User(std::string_view sName,
           std::string_view sPassword,
           size_t uId) :
    _sName(sName),
    _sPassword(sPassword),
    _uId(uId),
    _pBalanceUSD(0),
    _pBalanceRUB(0),
    _uCounterOffersSale(0),
    _uCounterOffersPurchase(0)
{
}

const std::string& User::GetName() const
{
    return _sName;
}

const std::string& User::GetPassword() const
{
    return _sPassword;
}

size_t User::GetId() const
{
    return _uId;
}

const std::list<OfferPtr>& User::GetOffersSale() const
{
    return _lOffersSale;
}

const std::list<OfferPtr>& User::GetOffersPurchase() const
{
    return _lOffersPurchase;
}

double User::GetAmountUSD() const
{
    return _pBalanceUSD.GetAmount();
}

double User::GetAmountRUB() const
{
    return _pBalanceRUB.GetAmount();
}

const std::list<OfferPtr>& User::GetCompletedpOffers() const
{
    return _lCompletedOffers;
}

void User::AddOffer(const OfferPtr& pOffer)
{
    if (pOffer->IsSaleOffer())
    {
        _lOffersSale.push_back(pOffer);
    }
    else
    {
        _lOffersPurchase.push_back(pOffer);
    }
}

void User::RemoveOffer(const std::list<OfferPtr>::const_iterator& itOffer)
{
    if ((*itOffer)->IsSaleOffer())
    {
        _lOffersSale.erase(itOffer);
    }
    else
    {
        _lOffersPurchase.erase(itOffer);
    }
    _lCompletedOffers.push_back(*itOffer);
}

void User::AddCompletedOffer(const OfferPtr& pOffer)
{
    if (pOffer->IsSaleOffer())
    {
        _lCompletedOffers.push_back(pOffer);
    }
    else
    {
        _lCompletedOffers.push_back(pOffer);
    }
}

void User::ReplenishUSD(double dAmount)
{
    _pBalanceUSD.Replenish(dAmount);
}

void User::ReplenishRUB(double dAmount)
{
    _pBalanceRUB.Replenish(dAmount);
}

void User::RemoveUSD(double dAmount)
{
    _pBalanceUSD.Remove(dAmount);
}

void User::RemoveRUB(double dAmount)
{
    _pBalanceRUB.Remove(dAmount);
}

OfferPtr User::CreateOfferSale(double dAmount, double dPrice)
{
    return OfferPtr(new OfferSale(std::to_string(_uId),
                                  _uCounterOffersSale++,
                                  dAmount,
                                  dPrice));
}

OfferPtr User::CreateOfferPurchase(double dAmount, double dPrice)
{
    return OfferPtr(new OfferPurchase(std::to_string(_uId),
                                      _uCounterOffersPurchase++,
                                      dAmount,
                                      dPrice));
}
