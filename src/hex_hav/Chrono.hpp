
#ifndef CHRONO_HPP_
#define CHRONO_HPP_

#include <chrono>

class Chrono 
{
    private:
        std::chrono::time_point<std::chrono::system_clock> _t0;
        std::chrono::time_point<std::chrono::system_clock> _t1;
    public:
        void start();
        void stop();
        double elapsedStopped() const;
        double elapsedRunning();
};

#endif  // CHRONO_HPP_

