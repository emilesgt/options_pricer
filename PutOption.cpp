#include "PutOption.h"
#include <algorithm>   

PutOption::PutOption(double T, double K)
    : EuropeanVanillaOption(T, K)
{
}

double PutOption::payoff(double S) const {
    return std::max(getStrike() - S, 0.0);
}

EuropeanVanillaOption::optionType PutOption::GetOptionType() const {
    return EuropeanVanillaOption::put;
}
