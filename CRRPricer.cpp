#include "CRRPricer.h"
#include <cmath>
#include <stdexcept>
#include <algorithm>


CRRPricer::CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate) :
    _option(option), _N(depth), _S0(asset_price), _U(up), _D(down), _R(interest_rate), _computed(false)
{
//Constructor


    if (!option)
    {
        throw std::invalid_argument("Invalid option pointer");
    }
    if (option->isAsianOption()) {
        throw std::invalid_argument("Error: The CRRPricer doesn't support an Asian option ");
    }
    if (_N < 0) { 
        throw std::invalid_argument("Depth must be >= 0"); 
    }

    if (_S0 <= 0.0) {
        throw std::invalid_argument("Asset price must be > 0"); 
    } 

    if (!(_D < _R && _R < _U)) //Arbitrage check
    {
        throw std::invalid_argument("Arbitrage detected: condition D < R < U must be verified");
    }

    _q = (_R - _D) / (_U - _D); //Risk neutral probability 
    _tree.setDepth(_N); // initialization of the tree strucutre, (N: depth)
    _exercise.setDepth(_N);
}

CRRPricer::CRRPricer(Option* option, int depth, double asset_price, double r, double volatility): _option(option), _N(depth), _S0(asset_price),_U(0.0), _D(0.0), _R(0.0),_q(0.0), _computed(false)
{// Constructor for Black-Scholes approximation 
    if (!_option) {
        throw std::invalid_argument("Invalid option pointer");
    }

    if (_option->isAsianOption()) {
        throw std::invalid_argument("Error: The CRRPricer doesn't support an Asian option");
    }

    if (_N <= 0) {
        throw std::invalid_argument("Depth must be > 0");
    }

    if (_S0 <= 0.0) {
        throw std::invalid_argument("Asset price must be > 0");
    }

    if (volatility < 0.0) {
        throw std::invalid_argument("Volatility must be >= 0");
    }

    const double T = _option->getExpiry();
    const double h = T / _N;

    //  the parameters of the binomial model:
    _U = std::exp((r + 0.5 * volatility * volatility) * h + volatility * std::sqrt(h)) - 1.0;
    _D = std::exp((r + 0.5 * volatility * volatility) * h - volatility * std::sqrt(h)) - 1.0;
    _R = std::exp(r * h) - 1.0;

    // No-arbitrage condition D < R < U 
    if (!(_D < _R && _R < _U)) {
        throw std::invalid_argument("Arbitrage detected: condition D < R < U must be verified");
    }

    _q = (_R - _D) / (_U - _D);

    _tree.setDepth(_N);
    _exercise.setDepth(_N);
}

void CRRPricer::compute() {  
    int N = _N;
    bool american = _option->isAmericanOption();
    for (int i = 0; i <= N; i++) { //Fill terminale nodes
        double S = _S0 * std::pow(1.0+_U, i) * std::pow(1.0+_D, N - i); //S(N,i) = S0 * (1+U)^i * (1+D)^(N - i)

        double payoff = _option->payoff(S);// Payoff depends on the option type (call or put)

        _tree.setNode(N, i, payoff); // Store in the tree
        _exercise.setNode(N, i, false);
    }



    for (int n = N - 1; n >= 0; n--) {
        for (int i = 0; i <= n; i++) {
            double expected = _q * _tree.getNode(n + 1, i + 1) + (1.0 - _q) * _tree.getNode(n + 1, i);
            double discounted = expected / (1 + _R);

            if (!american) {
                _tree.setNode(n, i, discounted);
                _exercise.setNode(n, i, false);
            }
            else {
                double S = _S0 * std::pow(1.0 + _U, i) * std::pow(1.0 + _D, n - i);
                double intrinsic = _option->payoff(S);

                double value = std::max(discounted, intrinsic);
                _tree.setNode(n, i, value); 
                _exercise.setNode(n, i, (intrinsic >= discounted));
            }

           
        }
    }

    _computed = true; // Mark as computed
}


double CRRPricer::get(int n, int i) const {// Returns the value H(n, i) from the computed tree.
    if (!_computed)
        throw std::logic_error("Tree not computed yet. Call compute() first.");
    return _tree.getNode(n, i);
}


double CRRPricer::comb(int n, int k) { // Computes C(n, k) = n! / (k!(n-k)!)
    if (k < 0 || k > n) return 0.0;
    if (k == 0 || k == n) return 1.0;

    double res = 1.0;
    for (int i = 1; i <= k; i++) {
        res *= (n - (k - i));
        res /= i;
    }
    return res;
}



double CRRPricer::operator()(bool closed_form) {// Return the option price (tree or closed-form)

    if (closed_form) {
        if (_option->isAmericanOption()) {
            throw std::invalid_argument("Closed-form pricing is not available for American options");
        }

        double sum = 0.0;
        for (int i = 0; i <= _N; i++) { // Closed-form expression : H(0,0) = (1 / (1+R)^N) * Σ [C(N,i) * q^i * (1-q)^(N-i) * payoff(S(N,i))]
            double S = _S0 * std::pow(1.0+_U, i) * std::pow(1.0+_D, _N - i);
            double payoff = _option->payoff(S);
            sum += comb(_N, i) * std::pow(_q, i) * std::pow(1 - _q, _N - i) * payoff;
        }
        return sum / std::pow(1 + _R, _N);
    }
    if (!_computed)
        compute();
    return _tree.getNode(0, 0);
}
bool CRRPricer::getExercise(int n, int i) const{ // Getter

    if (!_computed) {
        throw std::logic_error("Tree not computed yet. Call compute() first.");
    }
    return _exercise.getNode(n, i);
}