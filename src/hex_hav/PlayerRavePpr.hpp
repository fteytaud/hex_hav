
#ifndef PLAYER_RAVE_PPR_HPP_
#define PLAYER_RAVE_PPR_HPP_

#include <hex_hav/PlayerRave.hpp>

class PlayerRavePpr : public PlayerRave 
{
    private:
        Prng _prngPprRoulette;

    public:
        PlayerRavePpr();

    protected:
        cell_t computeSimulation(NodeRave * ptrNode) override;
};

#endif  // PLAYER_RAVE_PPR_HPP_

