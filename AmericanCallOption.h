#pragma once

#include "Option.h"
class AmericanCallOption : public AmericanOption {
private:
    double _strike;

public:
    AmericanCallOption(double T, double K);
    double getStrike() const { return _strike; }
    double payoff(double S) const override;

};
