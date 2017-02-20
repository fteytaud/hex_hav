// Copyright © 2014 Teytaud & Dehos <{teytaud,dehos}@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef PRNG_HPP_
#define PRNG_HPP_

#include <random>

// Pseudo-random number generator. Generate random number in [0, 1).
class Prng
{
    private:
        std::mt19937 _engine;
        std::uniform_real_distribution<double> _distribution;
    public:
        Prng();
        void setSeed(unsigned seed);
        double generate();
};

#endif  // PRNG_HPP_

