#include "Offer.h"

Offer::Offer(std::string_view sUserId,
             std::string_view sOfferId,
             double dAmount,
             double dPrice,
             bool bIsSaleOffer) :
    _sUserId(sUserId),
    _sId(sOfferId),
    _dAmount(dAmount),
    _dOriginalAmount(dAmount),
    _dPrice(dPrice),
    _bIsSaleOffer(bIsSaleOffer)
{
}

const std::string& Offer::GetUserId() const
{
    return _sUserId;
}

const std::string& Offer::GetId() const
{
    return _sId;
}

double Offer::GetAmount() const
{
    return _dAmount;
}

double Offer::GetOriginalAmount() const
{
    return _dOriginalAmount;
}

double Offer::GetPrice() const
{
    return _dPrice;
}

bool Offer::IsSaleOffer() const
{
    return _bIsSaleOffer;
}

void Offer::RemoveAmount(double dAmount)
{
    _dAmount -= dAmount;
}

bool Offer::IsMoreProfitable(const OfferPtr& pOffer) const
{
    if (_bIsSaleOffer)
    {
        if (_dPrice > pOffer->_dPrice)
        {
            return true;
        }
    }
    else
    {
        if (_dPrice < pOffer->_dPrice)
        {
            return true;
        }
    }
    return false;
}

bool Offer::operator==(const Offer& offer)
{
    if (this->_sUserId == offer._sUserId &&
       this->_dAmount == offer._dAmount &&
       this->_dPrice == offer._dPrice &&
       this->_sId == offer._sId &&
       this->_bIsSaleOffer == offer._bIsSaleOffer)
    {
        return true;
    }
    return false;
}



OfferSale::OfferSale(const std::string& sUserId,
                     size_t uOfferId,
                     double dAmount,
                     double dPrice) :
    Offer(sUserId,
          sUserId + "_s_" + std::to_string(uOfferId),
          dAmount, dPrice, true)
{
}

OfferPurchase::OfferPurchase(const std::string& sUserId,
                             size_t uOfferId,
                             double dAmount,
                             double dPrice) :
    Offer(sUserId,
          sUserId + "_p_" + std::to_string(uOfferId),
          dAmount, dPrice, false)
{
}
