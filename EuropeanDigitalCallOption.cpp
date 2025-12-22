#include "EuropeanDigitalCallOption.h"

EuropeanDigitalCallOption::EuropeanDigitalCallOption(double T, double K)
    : EuropeanDigitalOption(T, K) {}

EuropeanDigitalOption::optionType
EuropeanDigitalCallOption::GetOptionType() const {
    return call;
}

double EuropeanDigitalCallOption::payoff(double S) const {
    return (S >= getStrike()) ? 1.0 : 0.0;
}