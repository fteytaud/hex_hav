
#ifndef PLAYER_RAVE_LGRF1_HPP_
#define PLAYER_RAVE_LGRF1_HPP_

#include <hex_hav/PlayerRave.hpp>

class PlayerRaveLgrf1 : public PlayerRave 
{
    protected:
        std::vector<int> _lastGoodReplies;

    public:
        PlayerRaveLgrf1();
        void newTurn(const BOARD_TYPE & refBoard) override;

    protected:
        cell_t computeSimulation(NodeRave *ptrNode) override;
        void computeBackpropagation(NodeRave * ptrNode, 
                cell_t winCellt) override;
};

#endif  // PLAYER_RAVE_LGRF1_HPP_

