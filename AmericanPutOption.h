#pragma once
#include "Option.h"


class AmericanPutOption : public AmericanOption {
private:
    double _strike;

public:
    AmericanPutOption(double T, double K);

    double getStrike() const { return _strike; }
    double payoff(double S) const override;
};

