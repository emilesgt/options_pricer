#include "Option.h"

#include <algorithm>   // std::max
#include <numeric>     // std::accumulate
#include <stdexcept>   // std::invalid_argument



Option::Option(double T) {
    
    if (T < 0.0) {
        throw std::invalid_argument("Expiry must not be negative");    //make sure that expiry is positive
    }
    _expiry = T;
}

double Option::payoffPath(const std::vector<double>& path) const {
    
    if (path.empty()) {
        throw std::invalid_argument("The path is empty");
    }
    return payoff(path.back());// European only use the final last path.
}

bool Option::isAsianOption() const {
    // Default: not an Asian option
    return false;
}

bool Option::isAmericanOption() const {              //The options are european by default
    return false;
}


EuropeanVanillaOption::EuropeanVanillaOption(double T, double K): Option(T), _strike(K)
{
    // Ensure strike is positive
    if (K <= 0.0) {
        throw std::invalid_argument("Strike price must be positive");
    }
}







// Asian

double AsianOption::expiryFromTimeSteps(const std::vector<double>& timeSteps)   // constructor, but first we test if timesteps is empty
{
    if (timeSteps.empty()) {
        throw std::invalid_argument("timeSteps must not be empty");
    }
    return timeSteps.back();
}

AsianOption::AsianOption(const std::vector<double>& timeSteps): Option(expiryFromTimeSteps(timeSteps)), _timeSteps(timeSteps)
{
   
    const double expiry = getExpiry();

    // Validates that time steps are positive and strictly increasing.
    for (std::size_t k = 0; k < _timeSteps.size(); ++k) {
        if (_timeSteps[k] <= 0.0 || _timeSteps[k] > expiry) {
            throw std::invalid_argument("timeSteps must be in (0, T]");
        }
        if (k > 0 && _timeSteps[k] <= _timeSteps[k - 1]) {
            throw std::invalid_argument("timeSteps must be strictly increasing");
        }
    }
}

bool AsianOption::isAsianOption() const {
    return true;
}

double AsianOption::payoffPath(const std::vector<double>& path) const {
    // For Asian options, the path should match the observation times
    if (path.empty()) {
        throw std::invalid_argument("The path is empty");
    }
    if (path.size() != _timeSteps.size()) {
        throw std::invalid_argument("Path size must match timeSteps size for an Asian option");
    }

    const double sum = std::accumulate(path.begin(), path.end(), 0.0);
    const double avg = sum / static_cast<double>(path.size());

   
    return payoff(avg);
}





//American
AmericanOption:: AmericanOption(double T): Option(T){}

bool AmericanOption::isAmericanOption() const {
    return true;
}



