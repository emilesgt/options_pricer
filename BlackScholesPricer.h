
#pragma once
#include "Option.h"
#include "EuropeanDigitalOption.h"   
#include <cmath>
#include <stdexcept>

class BlackScholesPricer {
private:
    // On peut pricer soit une vanilla, soit une digitale
    EuropeanVanillaOption* _opt;     // option vanilla (call / put classique)
    EuropeanDigitalOption* _dopt;    // option digitale (call / put digital)

    double _s;   // asset price
    double _r;   // interest rate
    double _vol; // volatility

    static double norm_cdf(double x);  // N(x)
    static double norm_pdf(double x);  // φ(x)

public:

    BlackScholesPricer(EuropeanVanillaOption* option,
        double asset_price,
        double interest_rate,
        double volatility);


    BlackScholesPricer(EuropeanDigitalOption* option,
        double asset_price,
        double interest_rate,
        double volatility);

    double operator()() const;  // price computation
    double delta() const;       // delta computation
};
