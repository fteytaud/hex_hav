// Copyright © 2014 Teytaud & Dehos <{teytaud,dehos}@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include <hex_hav/Game.hpp>
#include <hex_hav/PlayerRave.hpp>
#include <hex_hav/PlayerRavePpr.hpp>

#include <hex_hav_gui2/View.hpp>

class Controller 
{
    private:
        Game _game;
        View _view;

        // local data for limiting concurrent accesses
        BOARD_TYPE _localBoard;
        //PlayerRavePpr _playerEval;
        PlayerRave _playerEval;
        BoardEval _boardEval;
        bool _isBoardEvalOk;

    public:
        const int _sizeParameter = 9;
        PlayerHuman _whitePlayer;
        PlayerHuman _blackPlayer;

    public:
        Controller(int argc, char ** argv);
        void run();

        void newGame();
        void clickCell(const Cell & refCell);
        int getSize() const;
        bool isGameFinished() const;

        cell_t getCurrentCellt() const;
        cell_t getWinnerCellt() const;
        cell_t getCelltAtCell(const Cell & refCell) const;

        const BoardEval & getBoardEval() const;
        void updateBoardEval(unsigned nbSims);
        bool isBoardEvalOk() const;

        void guiCells(std::vector<Cell> & whiteCells, 
                std::vector<Cell> & blackCells, 
                std::vector<Cell> & emptyCells) const;

    private:
        void updateLocalData();
};

#endif
