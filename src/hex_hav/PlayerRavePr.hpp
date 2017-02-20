
#ifndef PLAYER_RAVE_PR_HPP_
#define PLAYER_RAVE_PR_HPP_

#include <hex_hav/PlayerRave.hpp>

class PlayerRavePr : public PlayerRave 
{
    private:
        Prng _prngPrRoulette;

    public:
        PlayerRavePr();

    protected:
        cell_t computeSimulation(NodeRave * ptrNode) override;
};

#endif  // PLAYER_RAVE_PR_HPP_

