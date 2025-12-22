#pragma once
#include <random>
class MT
{
private:
    std::mt19937 _gen;
    std::uniform_real_distribution<double> _unif;
    std::normal_distribution<double> _norm;

    MT();
    MT(const MT&) = delete;
    MT& operator=(const MT&) = delete;

public:
    static MT& instance();

    static double rand_unif();      // U(0,1)
    static double rand_norm();      // N(0,1)
    ~MT();
};

