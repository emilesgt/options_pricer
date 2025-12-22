#pragma once
#include "Option.h"
class PutOption : public EuropeanVanillaOption {
public:
    PutOption(double T, double K);        //constructor of EuVaOp

    double payoff(double S) const override;                              //payoff attribute
    //payoff formula for a call option


    optionType GetOptionType() const override;             //get method override for otion type => returns call
};
