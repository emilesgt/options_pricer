#pragma once
#include "BinaryTree.h"
#include "Option.h"
#include <stdexcept>
#include <cmath>

class CRRPricer {
private:
    Option* _option;     // pointer to the option (base class)
    int _N;              // depth of the tree
    double _S0;          // initial asset price
    double _U, _D, _R;   // up, down, interest rate (R = simple rate per step)
    double _q;           // risk neutral probability
    bool _computed;      // flag to indicate if H(n, i) has already been computed
    BinaryTree<double> _tree;     // tree structure storing the option values H(n, i)

    BinaryTree<double> _exercise;
    // Utility function comb(N, K) used by the formula
    static double comb(int n, int k);

public:
    CRRPricer(Option* option, int depth, double asset_price, double up, double down, double interest_rate);  // Constructor

    CRRPricer(Option* option, int depth, double asset_price, double r, double volatility);
    void compute();

    double get(int n, int i) const;
    bool getExercise(int n, int i) const;
    // Returns the option price:
    // - if closed_form = false, uses the CRR tree
    // - if closed_form = true, uses the closed-form formula
    double operator()(bool closed_form = false);
};

