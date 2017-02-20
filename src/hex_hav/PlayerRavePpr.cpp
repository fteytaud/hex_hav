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

#include <hex_hav/PlayerRavePpr.hpp>

#include <algorithm>

PlayerRavePpr::PlayerRavePpr() 
{
    _parameters["PERCENTAGE"] = 0.01;
    _parameters["POOLPROBA"] = 0.8;
    _parameters["POOLRATIO"] = 0.2;
    _allMovesInSim.reserve(MAX_BOARDSIZE * MAX_BOARDSIZE);
}

cell_t PlayerRavePpr::computeSimulation(NodeRave * ptrNode) 
{
    // get player parameters
    const double PERCENTAGE  = _parameters.at("PERCENTAGE");
    const double POOLPROBA = _parameters.at("POOLPROBA");
    const double POOLRATIO = _parameters.at("POOLRATIO");
    const double nbSimulations = _parameters.at("nbSimulations");

    // find a parent node having meaningful statistics for PPR
    NodeRave * ancestor = ptrNode;
    while (ancestor) 
    {
        if (ancestor->_nbSims/nbSimulations >= PERCENTAGE)
            break;
        ancestor = (NodeRave*)ancestor->_ptrParentNode;
    }

    // compute PPR moves
    std::vector<int> pprMoves = ptrNode->_board.findEmptyIndices();
    bool hasPprMoves = ancestor != nullptr;
    if (hasPprMoves) 
    {
        // pruning: select moves having a win ratio > POOLRATIO
        auto fRatio = [ancestor, POOLRATIO] (int i) 
        {
            double raveWins = ancestor->_raveChildWins[i];
            double raveSims = ancestor->_raveChildSims[i];
            return raveWins/raveSims > POOLRATIO;
        };
        auto iter = std::partition(pprMoves.begin(), pprMoves.end(), fRatio);
        pprMoves.erase(iter, pprMoves.end());
    }

    // playout 
    BOARD_TYPE testBoard(ptrNode->_board);
    while (not testBoard.isGameFinished()) 
    {
        int indexToPlay = -1;
        int nbPprMoves = pprMoves.size();
        double rndRoulette = _prngPprRoulette.generate();
        double rndMove = _prngRaveSim.generate();
        // original: win rate = 52%
        // modif 1: win rate = 57%
        // modif 2: win rate = 77%
        // modif 1 + modif 2: win rate = 75%

        if (rndRoulette < POOLPROBA and hasPprMoves and nbPprMoves != 0) 
        {
            // choose a PPR move
            int indexPprMoves = rndMove * nbPprMoves;    // original
            //indexPprMoves = rndRoulette * nbPprMoves;  // modif 1
            indexToPlay = pprMoves[indexPprMoves];
            // remove the move in pprMoves
            pprMoves[indexPprMoves] = pprMoves[nbPprMoves-1];
            pprMoves.pop_back();
        } 
        else 
        {
            // choose a random move
            int nbMoves = testBoard.getNbEmptyIndices();
            //int indexMove = rndMove * nbMoves;         // original
            int indexMove = rndRoulette * nbMoves;       // modif 2
            indexToPlay = testBoard.findNthEmptyIndex(indexMove);
            // remove the move in pprMoves if present 
            auto it = std::find(pprMoves.begin(), pprMoves.end(), indexToPlay);
            if (it != pprMoves.end())
                pprMoves.erase(it);
        }

        // play the move
        testBoard.play(indexToPlay);
        _allMovesInSim.push_back(indexToPlay);
    }

    return testBoard.getWinnerCellt();
}

