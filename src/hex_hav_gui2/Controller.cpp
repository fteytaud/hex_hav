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

#include <hex_hav_gui2/Controller.hpp>

#include <cassert>

Controller::Controller(int argc, char ** argv) : _view(*this, argc, argv) 
{
    newGame();
}

void Controller::run() 
{
    _view.run();
}

void Controller::newGame() 
{
    // start new game
    _game.reset(_sizeParameter, &_whitePlayer, &_blackPlayer);
    updateLocalData();
    _view.newGame();
}

void Controller::clickCell(const Cell & cell) 
{
    // if there is a winner start a new game
    if (isGameFinished()) 
        newGame();

    // if there is no winner take turn
    else 
    {
        int index = _localBoard.convertCellToIndex(cell);
        _game.clickIndex(index);

        while (_game.hasTurnToPlay()) 
        { 
            _game.playTurn();
            updateLocalData();
        }
        _view.update();
    }
}

cell_t Controller::getCelltAtCell(const Cell & refCell) const 
{
    assert(_localBoard.isValidCell(refCell));
    return _localBoard.getCelltAtCell(refCell);
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

const BoardEval & Controller::getBoardEval() const
{
    return _boardEval;
}

void Controller::updateBoardEval(unsigned nbSims)
{
    _playerEval._parameters["nbSimulations"] = nbSims;
    _playerEval.newTurn(_localBoard);
    _boardEval = _playerEval.computeBoardEval();
    _isBoardEvalOk = true;
    _view.update();
}

bool Controller::isBoardEvalOk() const
{
    return _isBoardEvalOk;
}

void Controller::guiCells(std::vector<Cell> & whiteCells, 
        std::vector<Cell> & blackCells, std::vector<Cell> & emptyCells) const 
{
    _localBoard.guiCells(whiteCells, blackCells, emptyCells);
}

void Controller::updateLocalData() 
{
    _localBoard = _game.getRefBoard();
    _isBoardEvalOk = false;
}

