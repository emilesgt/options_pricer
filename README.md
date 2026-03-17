# Options Pricer

C++ project for pricing several classes of derivatives under standard option pricing frameworks.

The repository implements:

- **European vanilla options**: call and put
- **American options**: call and put
- **Asian options**: call and put
- **European digital options**: digital call and digital put

with the following pricing methods:

- **Black–Scholes closed form** for European vanilla and European digital options
- **CRR binomial tree** for vanilla and American-style pricing
- **Monte Carlo under Black–Scholes** for European and Asian options, with confidence intervals

The codebase is fully written in **C++** and organized with a clean object-oriented structure around an abstract `Option` base class. The GitHub repository description explicitly states support for American, European, Asian, and Digital option pricing with Monte Carlo, Black-Scholes, and Binomial approaches. citeturn934403view0turn480789view2turn480789view3turn480789view4

---

## Features

### 1. Option types

The class hierarchy is built around an abstract `Option` class with a pure virtual `payoff(double S)` method. The project also defines specialized derived abstractions for:

- `EuropeanVanillaOption`
- `AsianOption`
- `AmericanOption`
- `EuropeanDigitalOption` citeturn480789view1turn208320view0

Concrete instruments available in the repository include:

- `CallOption`
- `PutOption`
- `AmericanCallOption`
- `AmericanPutOption`
- `AsianCallOption`
- `AsianPutOption`
- `EuropeanDigitalCallOption`
- `EuropeanDigitalPutOption` citeturn934403view0turn208320view1turn208320view2turn208320view3turn208320view4turn372266view0turn372266view1turn372266view2turn372266view3

### 2. Pricing engines

#### Black–Scholes closed form

`BlackScholesPricer` supports:

- pricing European vanilla options
- pricing European digital options
- computing **Delta**

It has overloaded constructors for either a `EuropeanVanillaOption*` or a `EuropeanDigitalOption*`, and exposes `operator()()` for the price and `delta()` for sensitivity. citeturn480789view2

#### CRR binomial tree

`CRRPricer` supports tree-based pricing through:

- a constructor with explicit up/down factors and rate
- a constructor with rate and volatility
- backward induction via `compute()`
- optional closed-form evaluation through `operator()(bool closed_form = false)`
- exercise tracking through `getExercise(int n, int i)`

This makes it suitable for **European** and **American** contracts. citeturn480789view3

#### Monte Carlo under Black–Scholes

`BlackScholesMCPricer` supports:

- path generation with `generate(int nb_paths)`
- current estimator retrieval with `operator()()`
- a 95% confidence interval via `confidenceInterval()`
- pricing of options handled through the generic `Option*` interface

The header explicitly notes compatibility with **European** and **Asian** options. citeturn480789view4

---

## Repository structure

From the repository file tree, the project contains the following main components: citeturn934403view0

```text
Option abstraction and products
├── Option.h / Option.cpp
├── CallOption.h / CallOption.cpp
├── PutOption.h / PutOption.cpp
├── AmericanCallOption.h / .cpp
├── AmericanPutOption.h / .cpp
├── AsianCallOption.h / .cpp
├── AsianPutOption.h / .cpp
├── EuropeanDigitalOption.h / .cpp
├── EuropeanDigitalCallOption.h / .cpp
└── EuropeanDigitalPutOption.h / .cpp

Pricing engines
├── BlackScholesPricer.h / .cpp
├── BlackScholesMCPricer.h / .cpp
└── CRRPricer.h / .cpp

Utilities
├── BinaryTree.h / .cpp
├── MT.h / MT.cpp
└── main.cpp

Project files
├── Cpp_options_pricer.sln
└── ProjetA4.sln
```

`BinaryTree` is used by the CRR engine, while `MT` appears to provide random-number functionality for Monte Carlo simulation. The repository is entirely written in C++. citeturn934403view0

---

## Class design overview

The project follows an extensible design:

- `Option` is the abstract root class with a maturity and generic payoff interface.
- `EuropeanVanillaOption` stores the strike and option type abstraction for standard call/put contracts.
- `AsianOption` stores time steps and overrides `payoffPath(...)` for path-dependent pricing.
- `AmericanOption` marks contracts allowing early exercise.
- `EuropeanDigitalOption` provides a dedicated base for digital contracts. citeturn480789view1turn208320view0

This architecture makes it easy to add new products or pricing engines without changing the rest of the codebase.

---

## Example usage

Below are example snippets consistent with the class interfaces exposed in the headers.

### Black–Scholes pricing for a European call

```cpp
#include "CallOption.h"
#include "BlackScholesPricer.h"
#include <iostream>

int main() {
    CallOption call(1.0, 100.0);          // maturity T = 1 year, strike K = 100
    BlackScholesPricer pricer(&call, 100.0, 0.05, 0.20);

    std::cout << "Price: " << pricer() << std::endl;
    std::cout << "Delta: " << pricer.delta() << std::endl;
    return 0;
}
```

The constructor signature and available methods come directly from `BlackScholesPricer.h`. citeturn480789view2

### CRR binomial pricing for an American put

```cpp
#include "AmericanPutOption.h"
#include "CRRPricer.h"
#include <iostream>

int main() {
    AmericanPutOption put(1.0, 100.0);
    CRRPricer tree(&put, 100, 100.0, 0.05, 0.20);

    std::cout << "Price: " << tree() << std::endl;
    return 0;
}
```

`CRRPricer` exposes a constructor taking `(option, depth, asset_price, r, volatility)`, which is well suited to standard CRR setups. citeturn480789view3

### Monte Carlo pricing for an Asian call

```cpp
#include "AsianCallOption.h"
#include "BlackScholesMCPricer.h"
#include <iostream>
#include <vector>

int main() {
    std::vector<double> fixingDates = {0.25, 0.5, 0.75, 1.0};
    AsianCallOption option(fixingDates, 100.0);

    BlackScholesMCPricer mc(&option, 100.0, 0.05, 0.20);
    mc.generate(100000);

    std::cout << "MC price: " << mc() << std::endl;

    auto ci = mc.confidenceInterval();
    std::cout << "95% CI: [" << ci[0] << ", " << ci[1] << "]" << std::endl;
    return 0;
}
```

The `AsianCallOption` constructor takes a vector of time steps and a strike, while `BlackScholesMCPricer` provides `generate`, `operator()`, and `confidenceInterval`. citeturn208320view3turn480789view4

---

## Build instructions

The repository contains Visual Studio solution files:

- `Cpp_options_pricer.sln`
- `ProjetA4.sln` citeturn934403view0

### Option 1 — Visual Studio

1. Open the `.sln` file in Visual Studio.
2. Select the desired build configuration.
3. Build the project.
4. Add your own test code in `main.cpp` if needed.

### Option 2 — g++ / clang++

If you want to compile manually, you can compile all implementation files together, for example:

```bash
g++ -std=c++17 *.cpp -o options_pricer
```

Then run:

```bash
./options_pricer
```

Because `main.cpp` appears empty in the current repository snapshot, you will likely need to add your own example or test driver before producing an executable. citeturn666411view0

---

## What this project demonstrates

This repository is a strong academic / quantitative finance C++ project because it combines:

- **object-oriented financial product modelling**
- **closed-form pricing**
- **tree-based numerical methods**
- **Monte Carlo simulation**
- **greeks computation**
- **path-dependent derivatives**
- **early-exercise features**

It is therefore well suited for a portfolio, coursework submission, or interview discussion around derivatives pricing and numerical methods.

---

## Possible improvements

A few natural next steps for the repository would be:

- add a real `main.cpp` demo
- include benchmark cases and expected outputs
- add unit tests
- document mathematical formulas in more detail
- add implied volatility or additional greeks
- extend Monte Carlo to variance reduction techniques
- add barrier or lookback options

---

## Author

**Emile Sagot**

GitHub repository: `emilesgt/options_pricer` citeturn934403view0
