// Copyright © 2014 Teytaud & Dehos <{teytaud,dehos}@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef PRNG_TEST_HPP_
#define PRNG_TEST_HPP_

#include "Prng.hpp"
#include <cxxtest/TestSuite.h>

class Prng_test : public CxxTest::TestSuite 
{
    public:
        void testAddition() 
        {
            const int nbSamples = 1e7;
            const int N = 100;
            Prng prng;
            unsigned int sum = 0;
            for (int i=0; i<nbSamples; i++) 
            {
                int s = prng.generate() * N;
                sum += s;
                TS_ASSERT(s >= 0);
                TS_ASSERT(s < N);
            }
            double mean = sum / (double)nbSamples;
            TS_ASSERT_DELTA(mean, N*0.5, 5);
        }

        void testAdditionDouble() 
        {
            const int nbSamples = 1e7;
            double sum = 0;
            Prng prng;
            for (int i=0; i<nbSamples; i++) 
            {
                double s = prng.generate();
                sum += s;
                TS_ASSERT(s >= 0);
                TS_ASSERT(s < 1);
            }
            double mean = sum / (double)nbSamples;
            TS_ASSERT_DELTA(mean, 0.5, 0.1);
        }
};

#endif

