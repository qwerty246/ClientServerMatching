#pragma once

#include "TestBasic.h"

class TestOfferMerging : public TestBasic
{
public:
    TestOfferMerging() = default;
    ~TestOfferMerging() = default;

    void CheckReplyOfferAddedAndPartiallyMerged(double dAmount, double dRemainder, double dPrice) const;
    void CheckReplyOfferAddedAndPartiallyMerged(double dAmount1, double dAmount2, double dRemainder,
                                                double dPrice1, double dPrice2, double dFinalPrice) const;
};
