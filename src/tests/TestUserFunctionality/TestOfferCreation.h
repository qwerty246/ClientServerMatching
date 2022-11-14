#pragma once

#include "TestBasic.h"

class TestOfferCreation : public TestBasic
{
public:
    TestOfferCreation() = default;
    ~TestOfferCreation() = default;

    std::string GenerateOfferId(size_t uOfferId);
    void RemoveOffer(const std::string& sOfferId);
    void CheckReplyOfferRemoved(std::string_view sOfferId) const;
    void CheckReplyOfferNotExist() const;
    void CheckReplyOfferIncorrectId() const;
};
