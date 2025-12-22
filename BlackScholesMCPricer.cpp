#include "BlackScholesMCPricer.h"
#include "MT.h"

#include <cmath>        // std::exp, std::sqrt, std::log
#include <numeric>      // std::accumulate
#include <stdexcept>    // exceptions


BlackScholesMCPricer::BlackScholesMCPricer(Option* option, double initial_price, double interest_rate, double volatility) : _option(option), _S0(initial_price), _r(interest_rate), _sigma(volatility), _n(0), _sum(0.0), _sum2(0.0) {
	if (!_option) {
		throw std::invalid_argument("Not an option");
	}
	if (_S0 <= 0.0) {
		throw std::invalid_argument("Initial price must be > 0");
	}
	if (_sigma < 0.0) {
		throw std::invalid_argument("Volatility must be >= 0");
	}
}

void BlackScholesMCPricer::generate(int nb_paths)    //Handles both standard European options and path-dependent Asian options
{
    if (nb_paths <= 0) {
        throw std::invalid_argument("nb_paths must be > 0");
    }

    const double T = _option->getExpiry();

    for (int p = 0; p < nb_paths; ++p) {

        std::vector<double> path;

        // European option: m = 1
        if (!_option->isAsianOption()) {
            const double Z = MT::rand_norm();
            const double ST = _S0 * std::exp((_r - 0.5 * _sigma * _sigma) * T
                + _sigma * std::sqrt(T) * Z);
            path.push_back(ST);
        }
        // Asian option: m = timeSteps.size()
        else {
            const AsianOption* asian = static_cast<const AsianOption*>(_option);
            const std::vector<double>& ts = asian->getTimeSteps();
            path.reserve(ts.size());
            double S = _S0;
            double t_prev = 0.0;

            for (double t : ts) {
                const double dt = t - t_prev;                
                const double Z = MT::rand_norm();
                S = S * std::exp((_r - 0.5 * _sigma * _sigma) * dt + _sigma * std::sqrt(dt) * Z);
                path.push_back(S);
                t_prev = t;
            }
        }

        const double payoff = _option->payoffPath(path);
        const double sample = std::exp(-_r * T) * payoff;

        _sum += sample;
        _sum2 += sample * sample;
        _n += 1;
    }
}

double BlackScholesMCPricer::operator()() const    // Returns the current estimated price
{
    if (_n == 0) {
        throw std::logic_error("Estimator undefined");
    }
    double estimator = _sum / _n;
    return estimator;
}

std::vector<double> BlackScholesMCPricer::confidenceInterval() const
{
    if (_n < 2) {
        throw std::logic_error("Need at least 2 paths to compute a confidence interval");
    }

    const double mean = _sum / _n;
    const double mean2 = _sum2 / _n;

    // Unbiased sample variance: s^2 = (n/(n-1)) * (E[X^2] - (E[X])^2)
    const double var = (_n / (_n - 1.0)) * (mean2 - mean * mean);
    const double std_err = std::sqrt(var / _n);
    const double z = 1.96; // 95% CI (normal approximation)

    return { mean - z * std_err, mean + z * std_err };
}