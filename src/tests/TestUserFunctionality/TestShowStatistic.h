#pragma once

#include "TestBasic.h"

class TestShowStatistic : public TestBasic
{
public:
    TestShowStatistic() = default;
    ~TestShowStatistic() = default;

    void CheckActiveOffers(const std::vector<std::string>* vOffersSale, const std::vector<std::string>* vOffersPurchase) const;
    void CheckCompletedOffers(const std::vector<std::string>* vCompletedOffers) const;
    std::string GenerateActiveOfferLine(size_t uOfferId, double dAmount, double dPrice) const;
    std::string GenerateCompletedOfferLine(size_t uOfferId, double dAmount, double dPrice) const;
};
