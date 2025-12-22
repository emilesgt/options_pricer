#include "AsianPutOption.h"
#include <algorithm>   
#include <stdexcept>
AsianPutOption::AsianPutOption(const std::vector<double>& timeSteps, double K)
    : AsianOption(timeSteps), _strike(K)
{
    if (K <= 0.0) {
        throw std::invalid_argument("Strike price must be positive");
    }
}

double AsianPutOption::payoff(double S) const {
    return std::max(_strike - S, 0.0);
}
