#include "AsianCallOption.h"
#include <algorithm>   
#include <stdexcept>


AsianCallOption::AsianCallOption(const std::vector<double>& timeSteps, double K)
    : AsianOption(timeSteps), _strike(K)
{
    if (K <= 0.0) {
        throw std::invalid_argument("Strike price must be positive");
    }
}

double AsianCallOption::payoff(double S) const {
    return std::max(S - _strike, 0.0);
}