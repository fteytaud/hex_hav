
#include <hex_hav/Chrono.hpp>

double Chrono::elapsedRunning() 
{
    _t1 = std::chrono::system_clock::now();
    return 1e-3 * 
        std::chrono::duration_cast<std::chrono::milliseconds>(_t1-_t0).count();
}

double Chrono::elapsedStopped() const 
{
    return 1e-3 * 
        std::chrono::duration_cast<std::chrono::milliseconds>(_t1-_t0).count();
}

void Chrono::start() 
{
    _t1 = _t0 = std::chrono::system_clock::now();
}

void Chrono::stop() 
{
    _t1 = std::chrono::system_clock::now();
}

