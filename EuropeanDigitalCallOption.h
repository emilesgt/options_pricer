#pragma once
#include "EuropeanDigitalOption.h"
// Digital Call: payoff = 1_{S >= K}

class EuropeanDigitalCallOption : public EuropeanDigitalOption {
public:
    EuropeanDigitalCallOption(double T, double K);

    optionType GetOptionType() const override;
    double payoff(double S) const override;
};


