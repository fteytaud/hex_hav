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

#include <hex_hav/PlayerRavePr.hpp>

#include<algorithm>

PlayerRavePr::PlayerRavePr() 
{
    _parameters["POOLSIM"] = 50;
    _parameters["POOLSIZE"] = 20;
    _parameters["POOLPROBA"] = 0.3;
    _allMovesInSim.reserve(MAX_BOARDSIZE * MAX_BOARDSIZE);
}

cell_t PlayerRavePr::computeSimulation(NodeRave *ptrNode) 
{
    double SIZE = _parameters.at("POOLSIZE");
    double NBSIMS  = _parameters.at("POOLSIM");
    double PROBA = _parameters.at("POOLPROBA");

    BOARD_TYPE testBoard(ptrNode->_board);
    std::vector<int> prMoves = testBoard.findEmptyIndices();

    NodeRave *ancestor = ptrNode;
    while (ancestor) 
    {
        if (ancestor->_nbSims > 50)
            break;
        ancestor = (NodeRave*)ancestor->_ptrParentNode;
    }
    bool playAPoolRaveMove = ancestor != nullptr;

    if (playAPoolRaveMove) 
    {
        auto f1 = [ancestor, NBSIMS](int i) 
        { return ancestor->_raveChildSims[i] >= NBSIMS; };
        auto iter = std::partition(prMoves.begin(), prMoves.end(), f1);
        prMoves.erase(iter, prMoves.end());

        auto f2 = [ancestor](int i, int j) 
        {
            double winsI = ancestor->_raveChildWins[i];
            double simsI = ancestor->_raveChildSims[i];
            double winsJ = ancestor->_raveChildWins[j];
            double simsJ = ancestor->_raveChildSims[j];
            return winsI/simsI > winsJ/simsJ;
        };
        std::sort(prMoves.begin(), prMoves.end(), f2); 

        if (prMoves.size() > SIZE)
            prMoves.resize(SIZE);
    }

    while (not testBoard.isGameFinished()) 
    {
        int indexToPlay = -1;

        double rndRoulette = _prngPrRoulette.generate();
        double rndMove = _prngRaveSim.generate();
        if (rndRoulette < PROBA and prMoves.size() and playAPoolRaveMove) 
        {
            int iPool = prMoves.size() * rndMove;
            indexToPlay = prMoves[iPool];
            prMoves[iPool] = prMoves[prMoves.size()-1];
            prMoves.pop_back();
        } 
        else 
        {
            int n = testBoard.getNbEmptyIndices() * rndMove;
            indexToPlay = testBoard.findNthEmptyIndex(n);
            auto iter = std::find(prMoves.begin(), prMoves.end(), indexToPlay);
            if (iter != prMoves.end())
                prMoves.erase(iter);
        }

        testBoard.play(indexToPlay);
        _allMovesInSim.push_back(indexToPlay);
    }

    return testBoard.getWinnerCellt();
}

