#pragma once
#include "EuropeanDigitalOption.h"
// Digital Put: payoff = 1_{S <= K}

class EuropeanDigitalPutOption : public EuropeanDigitalOption {
public:
    EuropeanDigitalPutOption(double T, double K);

    optionType GetOptionType() const override;
    double payoff(double S) const override;
};


