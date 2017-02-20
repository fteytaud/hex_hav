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

#include <hex_hav/Player.hpp>

#include <cassert>
#include <iostream>

////////////////////////////////////////////////////////////////////////
// Player
////////////////////////////////////////////////////////////////////////

Player::Player() 
{
    resetTurn();
}

bool Player::hasIndexToPlay() const 
{
    return _hasIndexToPlay;
}

int Player::getIndexToPlay() const 
{
    return _indexToPlay;
}

void Player::resetTurn() 
{
    _hasIndexToPlay = false;
}

void Player::setIndexToPlay(int index) 
{
    _indexToPlay = index;
    _hasIndexToPlay = true;
}

////////////////////////////////////////////////////////////////////////
// PlayerHuman
////////////////////////////////////////////////////////////////////////

void PlayerHuman::newTurn(const BOARD_TYPE & ) 
{
    resetTurn();
}

void PlayerHuman::clickIndex(int index) 
{
    setIndexToPlay(index);
}

////////////////////////////////////////////////////////////////////////
// PlayerAi
////////////////////////////////////////////////////////////////////////

void PlayerAi::clickIndex(int ) 
{}

////////////////////////////////////////////////////////////////////////
// PlayerRandom
////////////////////////////////////////////////////////////////////////

PlayerRandom::PlayerRandom() 
{
    _parameters["fake"] = 0;
#ifdef PRNG_SEED
    _prng.setSeed(PRNG_SEED);
#endif  // PRNG_SEED
}

void PlayerRandom::newTurn(const BOARD_TYPE & refBoard) 
{
    int nbEmptyIndices = refBoard.getNbEmptyIndices();
    int n = _prng.generate() * nbEmptyIndices;
    int index = refBoard.findNthEmptyIndex(n);
    setIndexToPlay(index);
}

////////////////////////////////////////////////////////////////////////
// PlayerMc
////////////////////////////////////////////////////////////////////////

PlayerMc::PlayerMc() 
{
    _parameters["nbSimulations"] = 10000;
#ifdef PRNG_SEED
    setSeed(PRNG_SEED);
#endif  // PRNG_SEED
}

void PlayerMc::newTurn(const BOARD_TYPE & refBoard) 
{
    cell_t currentCellt = refBoard.getCurrentCellt();
    unsigned nbSimulations = _parameters["nbSimulations"];
    unsigned nbEmptyIndices = refBoard.getNbEmptyIndices();
    assert(nbSimulations > 0);
    assert(nbEmptyIndices > 0);

    // vector containing number of wins for each index
    std::vector<int> nbWins(nbEmptyIndices, 0);
    std::vector<int> emptyIndices = refBoard.findEmptyIndices();

    // simulate all indices, in parallel
    for (unsigned iIndex=0; iIndex<nbEmptyIndices; iIndex++) 
    {
        for (unsigned iSimulation=0; iSimulation<nbSimulations; iSimulation++) 
        {
            cell_t simuWinnerCellt = 
                playRandomGameWithFirstIndex(refBoard, emptyIndices[iIndex]);
            if (simuWinnerCellt == currentCellt)
                nbWins[iIndex]++;
        }
    }

    // find best cell
    int bestNbWins = -1;
    int iBestIndex = -1;
    for (unsigned i=0; i<nbEmptyIndices; i++) 
    {
        if (nbWins[i] > bestNbWins) 
        {
            bestNbWins = nbWins[i];
            iBestIndex = i;
        }
    }
    setIndexToPlay(emptyIndices[iBestIndex]);

#ifdef LOG
    std::clog << BOARD_TYPE::convertCelltToString(currentCellt)
        << "\t(nbSimulations = " << nbSimulations 
        << ")\tbestWins = " << bestNbWins << std::endl;
#endif  // LOG

}

cell_t PlayerMc::playRandomGameWithFirstIndex(
        const BOARD_TYPE & refBoard, int firstIndex) 
{
    BOARD_TYPE testBoard(refBoard);
    testBoard.play(firstIndex);
    while (not testBoard.isGameFinished()) 
    {
        int nbEmptyIndices = testBoard.getNbEmptyIndices();
        int n = _prng.generate() * nbEmptyIndices;
        int indexToPlay = testBoard.findNthEmptyIndex(n);
        testBoard.play(indexToPlay);
    }
    return testBoard.getWinnerCellt();
}

