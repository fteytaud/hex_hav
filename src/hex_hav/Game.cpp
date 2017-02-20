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

#include <hex_hav/Game.hpp>
#include <hex_hav/Player.hpp>

#include <cassert>
#include <iostream>

Game::Game() : _ptrWhitePlayer(nullptr), _ptrBlackPlayer(nullptr) 
{
    // TODO refactor havannah stats
    _stats.fill(0);
}

void Game::reset(int size, Player * ptrWhitePlayer, Player * ptrBlackPlayer) 
{
    std::clog << std::endl;
    std::clog << "new game\n";

    _ptrWhitePlayer = ptrWhitePlayer;
    _ptrBlackPlayer = ptrBlackPlayer;

    _board.reset(size);

    _chrono.start();

    Player * ptrPlayer = getPtrCurrentPlayer();
    ptrPlayer->newTurn(_board);
}

void Game::clickIndex(int index) 
{
    if (_board.isValidIndex(index)) 
    {
        Player * ptrCurrentPlayer = getPtrCurrentPlayer();
        if (ptrCurrentPlayer)
            ptrCurrentPlayer->clickIndex(index);
    }
}

bool Game::hasTurnToPlay() const 
{
    if (_board.getWinnerCellt() != CELL_EMPTY) 
        return false;
    Player * ptrCurrentPlayer = getPtrCurrentPlayer();
    return ptrCurrentPlayer and ptrCurrentPlayer->hasIndexToPlay();
}

void Game::playTurn() 
{
    assert(hasTurnToPlay());

    // play current turn
    Player * ptrCurrentPlayer = getPtrCurrentPlayer();
    int indexToPlay = ptrCurrentPlayer->getIndexToPlay();
    assert(_board.isValidIndex(indexToPlay));
    _board.play(indexToPlay);

    // TODO refactor havannah stats
    if (_board.isGameFinished()) 
    {
        assert(0 <= _board._stats and _board._stats < 7);
        _stats[_board._stats]++;
    }

    // display turn time
    _chrono.stop();
    cell_t currentCellt = _board.getCurrentCellt();
    std::string currentCelltString = 
        Board<BOARD_TYPE>::convertCelltToString(currentCellt);
    std::clog << currentCelltString << " played in " << _chrono.elapsedStopped() 
        << " s (cell " << indexToPlay << ")" << std::endl;

    // start new turn
    if (not _board.isGameFinished()) 
    {
        //if (_board.getWinnerCellt() == CELL_EMPTY) 
        ptrCurrentPlayer->resetTurn();
        _chrono.start();
        Player * ptrNewPlayer = getPtrCurrentPlayer();
        ptrNewPlayer->newTurn(_board);
    }
}

const std::string Game::getBoardType() const 
{
    return _board.getBoardType();
}

int Game::getSize() const 
{
    return _board.getSize();
}

int Game::getLastCellIndex() const 
{
    return _board.getLastIndex();
}

cell_t Game::getWinnerCellt() const 
{
    return _board.getWinnerCellt();
}

cell_t Game::getCurrentCellt() const 
{
    return _board.getCurrentCellt();
}

const BOARD_TYPE & Game::getRefBoard() const 
{
    return _board;
}

Player * Game::getPtrCurrentPlayer() const 
{
    cell_t currentCellt = _board.getCurrentCellt();
    if (currentCellt == CELL_WHITE) return _ptrWhitePlayer;
    if (currentCellt == CELL_BLACK) return _ptrBlackPlayer;
    return 0; 
}

