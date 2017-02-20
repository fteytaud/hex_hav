// Copyright © 2014 Teytaud & Dehos <{teytaud,dehos}@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef PLAYER_RAVE_MAST_HPP_
#define PLAYER_RAVE_MAST_HPP_

#include <hex_hav/PlayerRave.hpp>

class PlayerRaveMast : public PlayerRave 
{
    protected:
        std::vector<int> _mastWins;
        std::vector<int> _mastSims;

    public:
        PlayerRaveMast();
        void newTurn(const BOARD_TYPE & refBoard) override;

    protected:
        cell_t computeSimulation(NodeRave *ptrNode) override;
        void computeBackpropagation(NodeRave * ptrNode,
                cell_t winnerCellt) override;
};

#endif  // PLAYER_RAVE_MAST_HPP_

