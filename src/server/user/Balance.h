#pragma once

#include <string>

class Balance
{
public:
    Balance(double dAmount);
    ~Balance() = default;

    double GetAmount() const;
    void Replenish(double dAmount);
    void Remove(double dAmount);
private:
    double _dAmount;
};
