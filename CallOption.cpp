#include "CallOption.h"
#include <algorithm> 

CallOption::CallOption(double T, double K)
    : EuropeanVanillaOption(T, K)
{
}

double CallOption::payoff(double S) const {
    return std::max(S - getStrike(), 0.0);
}

EuropeanVanillaOption::optionType CallOption::GetOptionType() const {
    return EuropeanVanillaOption::call;
}