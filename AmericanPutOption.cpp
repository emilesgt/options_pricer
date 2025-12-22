#pragma once
#include "AmericanPutOption.h"
#include <algorithm>   
#include <stdexcept>

AmericanPutOption::AmericanPutOption(double T, double K) : AmericanOption(T), _strike(K)
{
    if (K <= 0.0) {
        throw std::invalid_argument("Strike price must be positive");
    }
}

double AmericanPutOption::payoff(double S) const {
    return std::max(_strike - S, 0.0);
}
