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

#include <hex_hav/PlayerRaveNast2.hpp>

#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
// Record
///////////////////////////////////////////////////////////////////////////////

void PlayerRaveNast2::Record::updateCdf(double & cdf) 
{
    double p = _sims==0 ? 1.0 : exp(_wins/double(_sims));
    cdf += p;
    _cdf = cdf;
}

///////////////////////////////////////////////////////////////////////////////
// PlayerRaveNast2
///////////////////////////////////////////////////////////////////////////////

void PlayerRaveNast2::newTurn(const BOARD_TYPE & refBoard) 
{
    PlayerRave::newTurn(refBoard);
    _nastRecords.clear();
}

cell_t PlayerRaveNast2::computeSimulation(NodeRave * ptrNode) 
{
    BOARD_TYPE testBoard(ptrNode->_board);
    while (not testBoard.isGameFinished()) 
    {

        //////////////////////////////////////////////////////////////////////
        // improve implementation -> win rate = 52%
        // consider all possible moves (even if a1/a2 not in nast)
        //////////////////////////////////////////////////////////////////////

        // compute CDF
        std::vector<int> moveIndices = testBoard.findEmptyIndices();
        std::vector<double> cdf(moveIndices.size());
        int action1 = testBoard.getLastIndex();
        double sumProba = 0.0;
        for (unsigned i=0; i<moveIndices.size(); i++)
        {
            double gibbs = 1.0;
            int action2 = moveIndices[i];
            int k = action1*testBoard.getNbFullIndices() + action2;
            auto iter = _nastRecords.find(k);
            if (iter != _nastRecords.end())
            {
                double wins = iter->second._wins;
                double sims = iter->second._sims;
                gibbs = exp(wins/sims);
            }
            sumProba += gibbs;
            cdf[i] = sumProba;
        }
        // choose one action
        double rndCdf = sumProba * _prngNast2Cdf.generate();
        int indexToPlay = moveIndices.front();
        for (unsigned i=1; i<moveIndices.size(); i++)
            if (cdf[i] >= rndCdf)
                break;
            else
                indexToPlay = moveIndices[i];

        /*
        //////////////////////////////////////////////////////////////////////
        // original implementation -> win rate = 46%
        // for each move, consider nast moves corresponding to action2
        //////////////////////////////////////////////////////////////////////

        // compute CDF
        std::vector<int> moveIndices = testBoard.findEmptyIndices();
        std::vector<double> cdf(moveIndices.size());
        double sumProba = 0.0;
        for (unsigned i=0; i<moveIndices.size(); i++)
        {
            double sims = 0.0;
            double wins = 0.0;
            int action2 = moveIndices[i];
            for (const auto & p : _nastRecords)
            {
                const auto & r = p.second;
                if (r._action2 == action2)
                {
                    sims += r._sims;
                    wins += r._wins;
                }
            }
            double gibbs = sims==0 ? 1.0 : exp(wins/sims);
            sumProba += gibbs;
            cdf[i] = sumProba;
        }
        // choose one action
        double rndCdf = sumProba * _prngNast2Cdf.generate();
        int indexToPlay = moveIndices.front();
        for (unsigned i=1; i<moveIndices.size(); i++)
            if (cdf[i] >= rndCdf)
                break;
            else
                indexToPlay = moveIndices[i];
        */

        // if the move invalid 
        if (not testBoard.isValidIndex(indexToPlay)
                or testBoard.getCelltAtIndex(indexToPlay) != CELL_EMPTY) 
        {
            // compute a random move
            double rndMove = _prngRaveSim.generate();
            int indexMove = testBoard.getNbEmptyIndices() * rndMove;
            indexToPlay = testBoard.findNthEmptyIndex(indexMove);
        }

        // store and play the move
        _allMovesInSim.push_back(indexToPlay);
        testBoard.play(indexToPlay);
    }

    return testBoard.getWinnerCellt();
}

void PlayerRaveNast2::computeBackpropagation(NodeRave *ptrNode, cell_t winCellt) 
{
    // update Rave and MCTS
    PlayerRave::computeBackpropagation(ptrNode, winCellt);

    // update Nast
    while (ptrNode) 
    {
        NodeRave * ptrParentNode = (NodeRave*) ptrNode->_ptrParentNode;

        int action1 = -1;
        if (ptrParentNode 
                and ptrParentNode->_board.getCurrentCellt() == _playerCellt)
            action1 = ptrParentNode->_index;

        int action2 = -1;
        if (ptrNode->_board.getCurrentCellt() != _playerCellt)
            action2 = ptrNode->_index;

        if (action1 != -1 and action2 != -1) 
        {
            // update record corresponding to (action1,action2)
            int k = action1*ptrParentNode->_board.getNbFullIndices() + action2;
            auto mapElt = std::make_pair(k,Record{action1,action2,0,0,0.0});
            auto mapPair = _nastRecords.insert(mapElt);
            Record & record = mapPair.first->second;
            record._sims++;
            if (winCellt == _playerCellt) 
                record._wins++;
        }

        ptrNode = ptrParentNode;
    }
}

#ifdef LOG
void PlayerRaveNast2::printLog() {
    const NodeRave &root = (_nodes.front());
    std::vector<double> superTest(root._raveChildWins.size());
    for (unsigned t = 0; t < superTest.size(); t++) 
        superTest[t] = root._raveChildWins[t]/(double)root._raveChildSims[t];
    root._board.printLogWithData(superTest);
}
#endif  // LOG

