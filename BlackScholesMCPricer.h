#pragma once

#include "Option.h"
#include <vector>

// Monte Carlo pricer under the Black-Scholes model
class BlackScholesMCPricer {
private:
    Option* _option;                  // option to price (can be European or Asian)
    double _S0;                       // initial asset price
    double _r;                        // interest rate
    double _sigma;                    // volatility

    int _n;                           // total number of simulated paths
    double _sum;                      // sum of discounted payoffs
    double _sum2;                     // sum of squares of discounted payoffs

public:
    // Constructor required by the project
    BlackScholesMCPricer(Option* option,
        double initial_price,
        double interest_rate,
        double volatility);

    // Adds nb_paths new Monte Carlo samples and updates the running estimate
    void generate(int nb_paths);

    // Returns the current Monte Carlo estimate (throws if undefined)
    double operator()() const;

    // Returns the 95% confidence interval [low, high]
    std::vector<double> confidenceInterval() const;

    // Returns the number of simulated paths
    int getNbPaths() const { return _n; }
};
