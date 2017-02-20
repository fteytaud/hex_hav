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

#include <hex_hav_gui1/Controller.hpp>
#include <hex_hav/PlayerMcts.hpp>
#include <hex_hav/PlayerRave.hpp>

#include <cassert>

Controller::Controller(int argc, char ** argv) : _view(*this, argc, argv) 
{
    _sizeParameter = 9;

    _whiteName = "Human";
    _whitePlayers["Human"] = std::unique_ptr<Player>(new PlayerHuman);
    _whitePlayers["Mcts"] = std::unique_ptr<Player>(new PlayerMcts);
    _whitePlayers["Rave"] = std::unique_ptr<Player>(new PlayerRave);

    _blackName = "Human";
    _blackPlayers["Human"] = std::unique_ptr<Player>(new PlayerHuman);
    _blackPlayers["Mcts"] = std::unique_ptr<Player>(new PlayerMcts);
    _blackPlayers["Rave"] = std::unique_ptr<Player>(new PlayerRave);

    newGame();
}

Controller::~Controller() 
{
    if (_computeThread.joinable()) 
        _computeThread.join();
}

void Controller::run() 
{
    _view.run();
}

void Controller::newGame() 
{
    if (_computeThread.joinable()) 
        _computeThread.join();

    // create players
    Player * whitePlayer = _whitePlayers[_whiteName].get();
    Player * blackPlayer = _blackPlayers[_blackName].get();

    if (whitePlayer and blackPlayer) 
    {
        // start new game
        _isComputing = true;
        std::function<void()> f = [this, whitePlayer, blackPlayer] () 
        {
            this->_game.reset(_sizeParameter, whitePlayer, blackPlayer);
            this->updateLocalData();
            this->_isComputing = false;
            this->_view.newGame();
        };
        _computeThread = std::thread(f);
    }
}

void Controller::clickCell(const Cell & cell) 
{
    if (not _isComputing) 
    {
        if (_computeThread.joinable()) 
            _computeThread.join();
        // if there is a winner start a new game
        if (isGameFinished()) 
            newGame();
        // if there is no winner take turn
        else 
        {
            int index = _localBoard.convertCellToIndex(cell);
            _game.clickIndex(index);
            _isComputing = true;
            std::function<void()> f = [this] () 
            {
                while (_game.hasTurnToPlay()) 
                { 
                    this->_game.playTurn();
                    this->updateLocalData();
                }
                this->_isComputing = false;
            };
            _computeThread = std::thread(f);
        }
    }
}

cell_t Controller::getCelltAtCell(const Cell & refCell) const 
{
    assert(_localBoard.isValidCell(refCell));
    return _localBoard.getCelltAtCell(refCell);
}

bool Controller::isComputing() const 
{
    return _isComputing;
}

bool Controller::isGameFinished() const 
{
    return _localBoard.isGameFinished();
}

cell_t Controller::getCurrentCellt() const 
{
    return _localBoard.getCurrentCellt();
}

cell_t Controller::getWinnerCellt() const 
{
    return _localBoard.getWinnerCellt();
}

int Controller::getSize() const 
{
    return _localBoard.getSize();
}

Cell Controller::getLastCell() const 
{
    int index = _localBoard.getLastIndex();
    if (_localBoard.isValidIndex(index))
        return _localBoard.convertIndexToCell(index);
    else
        return Cell(-1, -1);
}

void Controller::guiCells(std::vector<Cell> & whiteCells, 
        std::vector<Cell> & blackCells, std::vector<Cell> & emptyCells) const 
{
    _game.getRefBoard().guiCells(whiteCells, blackCells, emptyCells);
}

void Controller::updateLocalData() 
{
    _localBoard = _game.getRefBoard();
}

