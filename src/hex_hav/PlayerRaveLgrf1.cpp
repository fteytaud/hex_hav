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

#include <hex_hav/PlayerRaveLgrf1.hpp>

PlayerRaveLgrf1::PlayerRaveLgrf1() :
    _lastGoodReplies(MAX_BOARDSIZE*MAX_BOARDSIZE, -1)
{}

void PlayerRaveLgrf1::newTurn(const BOARD_TYPE & refBoard) 
{
    PlayerRave::newTurn(refBoard);
    std::fill(std::begin(_lastGoodReplies), std::end(_lastGoodReplies), -1);
}

cell_t PlayerRaveLgrf1::computeSimulation(NodeRave *ptrNode) 
{
    BOARD_TYPE testBoard(ptrNode->_board);

    while (not testBoard.isGameFinished()) 
    {
        // get last good reply 
        int indexToPlay = _lastGoodReplies[testBoard.getLastIndex()];
        if (testBoard.getCurrentCellt() == _playerCellt)
            indexToPlay = -1;

        // if no last good reply
        if (indexToPlay == -1 
                or not testBoard.isValidIndex(indexToPlay) 
                or testBoard.getCelltAtIndex(indexToPlay) != CELL_EMPTY) 
        {
            // chose a random move
            int nbEmptyIndices = testBoard.getNbEmptyIndices();
            int n = nbEmptyIndices * _prngRaveSim.generate();
            indexToPlay = testBoard.findNthEmptyIndex(n);
        }

        // play the move
        _allMovesInSim.push_back(indexToPlay);
        testBoard.play(indexToPlay);

    }

    return testBoard.getWinnerCellt();
}

void PlayerRaveLgrf1::computeBackpropagation(NodeRave* ptrNode, cell_t winCellt) 
{
    // update MCTS and Rave stats
    PlayerRave::computeBackpropagation(ptrNode, winCellt);

    // update last good replies
    // if we have a winning playout
    if (winCellt == _playerCellt)
        // store playout moves in the last good replies
        for (unsigned i=1; i<_allMovesInSim.size()-1; i+=2) 
            _lastGoodReplies[_allMovesInSim[i]] = _allMovesInSim[i+1];
    else
        // remove playout from the last good replies
        for (unsigned i=1; i<_allMovesInSim.size()-1; i+=2) 
            _lastGoodReplies[i] = -1;
}

