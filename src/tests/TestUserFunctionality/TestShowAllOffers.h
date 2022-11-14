#pragma once

#include "TestBasic.h"

class TestShowAllOffers : public TestBasic
{
public:
    TestShowAllOffers() = default;
    ~TestShowAllOffers() = default;

    void CheckNoOffers() const;
    void CheckAllOffers(bool bIsSaleOffer, double dPriceMax, double dPriceMin, double dPriceMean,
                        const std::vector<std::string>* vOffersSale1,
                        const std::vector<std::string>* vOffersSale2 = nullptr) const;
    std::string GenerateOfferLine(const std::string& sUsername, double dAmount, double dPrice) const;
};
