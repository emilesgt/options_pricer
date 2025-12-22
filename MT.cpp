#include "MT.h"
#include <chrono>

MT::MT() //Initializes the Mersenne Twister generator with a time-based seed
    : _gen(static_cast<unsigned int>(std::time(nullptr))), _unif(0.0, 1.0),_norm(0.0, 1.0)
{}

MT& MT::instance() {  
    static MT inst;
    return inst;
}
MT::~MT(){}

double MT::rand_unif() {
    return instance()._unif(instance()._gen);
}

double MT::rand_norm() {
    return instance()._norm(instance()._gen);
}