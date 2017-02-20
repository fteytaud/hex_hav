
#include <hex_hav/Prng.hpp>

#include <ctime>
#include <thread>
#include <iostream>

Prng::Prng() : 
    _engine(std::random_device{}()),  
    _distribution(0, 1) 
{
#ifdef PRNG_SEED
    setSeed(PRNG_SEED);
#endif  // PRNG_SEED
}

double Prng::generate() 
{
    return _distribution(_engine);
}

void Prng::setSeed(unsigned seed) 
{
    _engine.seed(seed);
#ifdef LOG
    std::clog << "Prng::setSeed(" << seed << ")" << std::endl;
#endif  // LOG
}

