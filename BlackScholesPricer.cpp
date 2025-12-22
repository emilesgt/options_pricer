#include "BlackScholesPricer.h"
#include <algorithm>
#include <cmath>


// Black Scholes Pricer class
double BlackScholesPricer::norm_cdf(double x) {                                             //normal cumulative distribution function                         
    return 0.5 * std::erfc(-x / std::sqrt(2));                                              //erfc(x) = 1-erf(x), N(x)=(erfc(-x/sqrt2))/2
}

double BlackScholesPricer::norm_pdf(double x) {
    static const double inv_sqrt_2pi = 0.39894228040143267794; // 1 / sqrt(2π)
    return inv_sqrt_2pi * std::exp(-0.5 * x * x);
}

//constructor
BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option,
    double asset_price, double interest_rate, double volatility)
    : _opt(option), _dopt(nullptr),
    _s(asset_price), _r(interest_rate), _vol(volatility)
{
    if (!_opt)
        throw std::invalid_argument("Not an option");
    if (_s <= 0)
        throw std::invalid_argument("Asset price must be > 0");
    if (_vol < 0)
        throw std::invalid_argument("Volatility must be >= 0");
}



BlackScholesPricer::BlackScholesPricer(EuropeanDigitalOption* option,
    double asset_price, double interest_rate, double volatility)
    : _opt(nullptr), _dopt(option),
    _s(asset_price), _r(interest_rate), _vol(volatility)
{
    if (!_dopt)
        throw std::invalid_argument("Not an option");
    if (_s < 0)
        throw std::invalid_argument("Asset price must be > 0");
    if (_vol < 0)
        throw std::invalid_argument("Volatility must be >= 0");
}


double BlackScholesPricer::operator()() const {
    
    if (_dopt) {
        double T = _dopt->getExpiry();
        double K = _dopt->getStrike();

        //  limits : T=0 or vol=0 => price = payoff
        if (T == 0 || _vol == 0)
            return _dopt->payoff(_s);

        double sqrtT = std::sqrt(T);
        double d1 = (std::log(_s / K) + (_r + 0.5 * _vol * _vol) * T) / (_vol * sqrtT);
        double d2 = d1 - _vol * sqrtT;

        if (_dopt->GetOptionType() == EuropeanDigitalOption::call) {
            // Prix digital call : e^{-rT} N(d2)
            return std::exp(-_r * T) * norm_cdf(d2);
        }

        if (_dopt->GetOptionType() == EuropeanDigitalOption::put) {
            // Prix digital put : e^{-rT} N(-d2)
            return std::exp(-_r * T) * norm_cdf(-d2);
        }

        throw std::logic_error("Unknown digital option type");
    }

    // option vanilla
    if (_opt) {
        double T = _opt->getExpiry();
        double K = _opt->getStrike();

        // limite : T=0 or vol=0 => price = payoff
        if (T == 0 || _vol == 0)
            return _opt->payoff(_s);

        double d1 = (std::log(_s / K) + (_r + 0.5 * _vol * _vol) * T) / (_vol * std::sqrt(T));
        double d2 = d1 - _vol * std::sqrt(T);

        if (_opt->GetOptionType() == EuropeanVanillaOption::call)
            return norm_cdf(d1) * _s - norm_cdf(d2) * K * std::exp(-_r * T);

        if (_opt->GetOptionType() == EuropeanVanillaOption::put)
            return norm_cdf(-d2) * K * std::exp(-_r * T) - norm_cdf(-d1) * _s;

        throw std::logic_error("Unknown vanilla option type");
    }

    throw std::logic_error("No option attached to BlackScholesPricer");
}



double BlackScholesPricer::delta() const {
    // digital option 
    if (_dopt) {
        double T = _dopt->getExpiry();
        double K = _dopt->getStrike();

        //  limite : avoid % 0
        if (T == 0 || _vol == 0)
            return 0.0;

        double sqrtT = std::sqrt(T);
        double d1 = (std::log(_s / K) + (_r + 0.5 * _vol * _vol) * T) / (_vol * sqrtT);
        double d2 = d1 - _vol * sqrtT;

        double factor = std::exp(-_r * T) * norm_pdf(d2) / (_s * _vol * sqrtT);

        if (_dopt->GetOptionType() == EuropeanDigitalOption::call) {
            // Δ digital call 
            return factor;
        }

        if (_dopt->GetOptionType() == EuropeanDigitalOption::put) {
            // Δ digital put
            return -factor;
        }

        throw std::logic_error("Unknown digital option type");
    }

    //  vanilla option 
    if (_opt) {
        double T = _opt->getExpiry();
        double K = _opt->getStrike();

        // limite 
        if (T == 0 || _vol == 0) {
            if (_opt->GetOptionType() == EuropeanVanillaOption::call)
                return (_s > K) ? 1.0 : 0.0;
            if (_opt->GetOptionType() == EuropeanVanillaOption::put)
                return (_s < K) ? -1.0 : 0.0;
        }

        double d1 = (std::log(_s / K) + (_r + 0.5 * _vol * _vol) * T) / (_vol * std::sqrt(T));

        if (_opt->GetOptionType() == EuropeanVanillaOption::call)
            return norm_cdf(d1);

        if (_opt->GetOptionType() == EuropeanVanillaOption::put)
            return norm_cdf(d1) - 1;

        throw std::logic_error("Unknown vanilla option type");
    }

    throw std::logic_error("No option attached to BlackScholesPricer");
}