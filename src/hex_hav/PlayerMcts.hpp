/*
BSD 3-Clause License

Copyright (c) 2017, 
Fabien Teytaud, Julien Dehos, Joris Duguépéroux and Ahmad Mazyad
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef PLAYER_MCTS_HPP_
#define PLAYER_MCTS_HPP_

#include <hex_hav/PlayerMctsT.hpp>

////////////////////////////////////////////////////////////////////////
// Node struct for MTCS tree
// A NodeMcts implements a game state during MCTS search
////////////////////////////////////////////////////////////////////////

struct NodeMcts 
{
    // current board of the node
    BOARD_TYPE _board;   

    // index of the node, played in the board (so _board.getCurrentCellt() is
    // the opponent of _index cell_t)
    int _index;     

    // number of winning simulations (for root cell_t) which have used the node
    unsigned _nbWins;       

    // number of simulations which have used the node
    unsigned _nbSims;	

    // child nodes in MCTS tree, ie empty cells of _board which have been
    // considered as the following cell to play (by the opponent)
    std::vector<NodeMcts*> _childNodes;

    // empty indices of _board which have not been considered yet (no
    // corresponding child node) so "_board.getEmptyIndices() == _childNodes +
    // _possibleIndices"
    std::vector<int> _possibleIndices;

    // parent Mcts node (so "this" should be in _ptrParentNode->_childNodes)
    NodeMcts * _ptrParentNode;

    NodeMcts(const BOARD_TYPE & b);
    NodeMcts(const BOARD_TYPE & b, int index, NodeMcts * ptrParentNode);
    NodeMcts * getBestNode() const;
    double getWinRatio() const;
};

////////////////////////////////////////////////////////////////////////
// class for implementing the MCTS algorithm
////////////////////////////////////////////////////////////////////////

class PlayerMcts : public PlayerMctsT<NodeMcts> 
{
    protected:
        Prng _prngMctsSim;

    public:
        PlayerMcts();

    protected:
        NodeMcts * computeSelection(NodeMcts * ptrNode) override ;
        int computeExpansion(NodeMcts * ptrNode) override ;
        cell_t computeSimulation(NodeMcts * ptrNode) override ;
        void computeBackpropagation(NodeMcts * ptrNode, cell_t winnerCellt) 
            override ;

    private:
        double computeScoreUct(NodeMcts * ptrNode) const;
};

#endif  // PLAYER_MCTS_HPP_

