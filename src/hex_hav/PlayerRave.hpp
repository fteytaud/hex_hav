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

#ifndef PLAYER_RAVE_HPP_
#define PLAYER_RAVE_HPP_

#include <hex_hav/PlayerMcts.hpp>

////////////////////////////////////////////////////////////////////////
// structures for evaluating cells (for debug purpose)
////////////////////////////////////////////////////////////////////////

struct CellEval {
    int _i;
    int _j;
    unsigned _nbSims;
    double _score;
};

struct BoardEval {
    static const int SIMS_THRESHOLD = 10;
    std::vector<CellEval> _cellEvals;
    unsigned _nbSims;
    unsigned _nbSimsBest;
    double _scoreBest;
    int _iBest;
    int _jBest;
};

////////////////////////////////////////////////////////////////////////
// Node struct for MCTS+RAVE tree
////////////////////////////////////////////////////////////////////////

struct NodeRave : NodeMcts 
{
    // a RAVE vector gives the number of time each cell has been played in the
    // sub tree 
    std::vector<double> _raveChildWins;  
    std::vector<double> _raveChildSims; 

    NodeRave(const BOARD_TYPE & b);
    NodeRave(const BOARD_TYPE & b, int index, NodeRave * ptrParentNode);

    BoardEval computeBoardEval(unsigned nbSims) const;
};

////////////////////////////////////////////////////////////////////////
// RAVE algorithm
////////////////////////////////////////////////////////////////////////

class PlayerRave : public PlayerMctsT<NodeRave> 
{
    protected:
        Prng _prngRaveSim;

    protected: 
        // board indices of the full game played by the last call to
        // computeSimulation, first move: root node, ...
        std::vector<int> _allMovesInSim;   

    public:
        PlayerRave();

        // TODO refactor board eval (PlayerMcts)
        BoardEval computeBoardEval() const;

    protected:
        double computeScoreUctRave(NodeRave * ptrNode) const;
        virtual void computeOneSimulationInit() override;
        virtual NodeRave * computeSelection(NodeRave * ptrNode) override;
        virtual int computeExpansion(NodeRave * ptrNode) override;
        virtual cell_t computeSimulation(NodeRave * ptrNode) override;
        virtual void computeBackpropagation(NodeRave * ptrNode, 
                cell_t winnerCellt) override;

#ifdef LOG
        virtual void printLog() override;
#endif  // LOG

};

#endif  // PLAYER_RAVE_HPP_

