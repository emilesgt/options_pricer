#include "EuropeanDigitalOption.h"

// EuropeanDigitalOption 

EuropeanDigitalOption::EuropeanDigitalOption(double T, double K)
    : Option(T), _strike(K)
{
    if (K <= 0.0) {
        throw std::invalid_argument("Strike price must be positive");
    }
}

double EuropeanDigitalOption::getStrike() const {
    return _strike;
}


