#pragma once
#include "Option.h"
class CallOption : public EuropeanVanillaOption {
public:
    CallOption(double T, double K);      //constructor of EuVaOp

    //payoff attribute
    //payoff formula for a call option

    double payoff(double S) const override;
    optionType GetOptionType() const override;             //get method override for otion type => returns call
};

