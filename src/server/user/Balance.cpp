#include "Balance.h"

Balance::Balance(double dAmount) :
    _dAmount(dAmount)
{
}

double Balance::GetAmount() const
{
    return _dAmount;
}

void Balance::Replenish(double dAmount)
{
    _dAmount += dAmount;
}

void Balance::Remove(double dAmount)
{
    _dAmount -= dAmount;
}
