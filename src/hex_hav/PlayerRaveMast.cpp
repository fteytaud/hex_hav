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

#include <hex_hav/PlayerRaveMast.hpp>

#include <algorithm>

PlayerRaveMast::PlayerRaveMast() :
    _mastWins(MAX_BOARDSIZE * MAX_BOARDSIZE, 0), 
    _mastSims(MAX_BOARDSIZE * MAX_BOARDSIZE, 0)
{}

void PlayerRaveMast::newTurn(const BOARD_TYPE & refBoard) 
{
    PlayerRave::newTurn(refBoard);
    std::fill(std::begin(_mastWins), std::end(_mastWins), 0);
    std::fill(std::begin(_mastSims), std::end(_mastSims), 0);
}

cell_t PlayerRaveMast::computeSimulation(NodeRave * ptrNode) 
{
    // playout
    BOARD_TYPE testBoard(ptrNode->_board);
    while (not testBoard.isGameFinished()) 
    {
        // compute CDF for all possible moves
        std::vector<int> moveIndices = testBoard.findEmptyIndices();
        std::vector<double> mastCdf(moveIndices.size());
        double sumProba = 0.0;
        auto fCdf = [this,&sumProba](int moveIndex) 
        { 
            int mastSims = this->_mastSims[moveIndex];
            sumProba += mastSims==0 ? 1.0 :
                exp(this->_mastWins[moveIndex]/double(mastSims));
            return sumProba;
        };
        std::transform(moveIndices.begin(), moveIndices.end(), 
                mastCdf.begin(), fCdf);

        // choose a move according to CDF
        double rnd = sumProba * _prngRaveSim.generate();
        auto iter = std::find_if(mastCdf.begin(), mastCdf.end(), 
                [rnd] (double cdf) { return cdf >= rnd; });
        assert(iter != mastCdf.end());
        int cdfIndex = std::distance(mastCdf.begin(), iter);
        int indexToPlay = moveIndices[cdfIndex];

        // play the move
        testBoard.play(indexToPlay);
        _allMovesInSim.push_back(indexToPlay);
    }

    return testBoard.getWinnerCellt();
}

void PlayerRaveMast::computeBackpropagation(NodeRave* ptrNode, cell_t winCellt) 
{
    // update MCTS and Rave stats
    PlayerRave::computeBackpropagation(ptrNode, winCellt);

    // update MAST stats
    for (unsigned i = 0; i < _allMovesInSim.size(); i+=2) 
    {
        unsigned moveIndex = _allMovesInSim[i];
        if (winCellt == _playerCellt) 
        { 
            _mastWins[moveIndex]++;
            _mastSims[moveIndex]++;
        }
        else 
        { 
            _mastSims[moveIndex]++;
        }
    }
}

