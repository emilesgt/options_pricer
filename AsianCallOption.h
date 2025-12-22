#pragma once

#include "Option.h"
#include <vector>
class AsianCallOption : public AsianOption {
private:
    double _strike;                                                     // strike price

public:
    AsianCallOption(const std::vector<double>& timeSteps, double K);

    double getStrike() const { return _strike; }                        // getter
    double payoff(double S) const override;                             // payoff based on the averaged price
};
