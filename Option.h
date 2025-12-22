#pragma once
#include <algorithm>   
#include <stdexcept>   
#include <cmath>       
#include <vector>

class BlackScholesPricer;



//abstract Class option, abstract = contains a pure virtual function
class Option {
private:
    double _expiry;                                                         //expiry date

public:
    explicit Option(double T);   //constructor  //make sure that expiry is positive

    double getExpiry() const { return _expiry; }                            //get method

    virtual double payoff(double S) const = 0;                              
    virtual double payoffPath(const std::vector<double>& path) const;

    virtual bool isAsianOption() const;
    virtual bool isAmericanOption() const;


    virtual ~Option() = default;                                            //destructor
};



//European Vanilla Option class derived from Option class
class EuropeanVanillaOption : public Option {
public:
    enum optionType { call, put };                                          //type call or put

private:
    double _strike;                                                         //strike price

public:
    EuropeanVanillaOption(double T, double K);      //constructor
                                                    //we have already ensured that T>0 in the option class
                                                    //an option cannot have a negative strike price

    double getStrike() const { return _strike; }                            //get method for strike

    virtual optionType GetOptionType() const = 0;                           //get method for option type

    friend class BlackScholesPricer;                                        //friend function to access the strike
};





class AsianOption : public Option {
protected:
    std::vector<double> _timeSteps;

    static double expiryFromTimeSteps(const std::vector<double>& timeSteps);
public:
    AsianOption(const std::vector<double>& timeSteps);

    const std::vector<double>& getTimeSteps() const { return _timeSteps; } // getter

    bool isAsianOption() const override;

    double payoffPath(const std::vector<double>& path) const override;
};





class AmericanOption : public Option {
public:
    AmericanOption(double T);

    bool isAmericanOption() const override;
};




