#include <iostream>
#include <cmath>                //opérateurs mathématiques
#include <stdexcept>            //gérer exception comme strike négatif

using namespace std;

class Option 
{

    private:
    double _T;             // expiry date

    public:
    Option(double T)       //make sure that expiry is positive
    {
        if (T<0.0){
            throw invalid_argument("Expiry must not be negative");
        }
        _T = T;
    }

    double GetExpiry() const {return _T;} //get method

    virtual double payoff(double z) const=0;           //méthode virtual peut être surchargé dans une calsse dérivée
    virtual ~Option() = default;                       //destructeur

};

class EuropeanVanillaOption : public Option {
    
    public:
    enum optionType {call, put};            // type call or put

    private:
    double _K;                               // strike price

    public:
    EuropeanVanillaOption(double T, double K)
    : Option(T), _K(K){
        if(K<0.0)
        {
            throw invalid_argument("Strike price must be positive");
        }
    }
    double getstrike()const;

    virtual optionType GetOptionType() const = 0;

};