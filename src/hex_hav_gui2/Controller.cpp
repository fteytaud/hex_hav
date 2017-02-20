// Copyright © 2014 Teytaud & Dehos <{teytaud,dehos}@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

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

