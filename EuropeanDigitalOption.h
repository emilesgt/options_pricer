#pragma once

#include "Option.h"
#include <stdexcept>


// Base class for digital options

class EuropeanDigitalOption : public Option {
public:
    enum optionType { call, put };

protected:
    double _strike;

public:
    EuropeanDigitalOption(double T, double K);

    double getStrike() const;
    virtual optionType GetOptionType() const = 0;
    virtual double payoff(double S) const = 0;

    friend class BlackScholesPricer;
};





