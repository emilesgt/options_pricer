#pragma once

#include "AmericanCallOption.h"
#include <algorithm>   
#include <stdexcept>

AmericanCallOption::AmericanCallOption(double T, double K) : AmericanOption(T), _strike(K) {

    if (K <= 0.0) {
        throw std::invalid_argument("Strike price must be positive");
    }
}

double AmericanCallOption::payoff(double S) const {
    return std::max(S - _strike, 0.0);
}