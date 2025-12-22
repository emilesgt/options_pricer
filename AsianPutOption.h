#pragma once

#include "Option.h"
#include <vector>

class AsianPutOption : public AsianOption {
private:
    double _strike;                                                     // strike price

public:
    AsianPutOption(const std::vector<double>& timeSteps, double K);

    double getStrike() const { return _strike; }                        // getter
    double payoff(double S) const override;                             // payoff based on the averaged price
};

