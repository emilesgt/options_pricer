#include "EuropeanDigitalPutOption.h"

EuropeanDigitalPutOption::EuropeanDigitalPutOption(double T, double K) 
    : EuropeanDigitalOption(T, K) {} 

EuropeanDigitalOption::optionType
EuropeanDigitalPutOption::GetOptionType() const {
    return put;
}

double EuropeanDigitalPutOption::payoff(double S) const {
    return (S <= getStrike()) ? 1.0 : 0.0;
}