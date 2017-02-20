// Copyright © 2014 Teytaud & Dehos <{teytaud,dehos}@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include <hex_hav/Game.hpp>
#include <hex_hav_gui1/View.hpp>

class Controller 
{
    private:
        Game _game;
        View _view;

        bool _isComputing;
        std::thread _computeThread;

        // local data for limiting concurrent accesses
        BOARD_TYPE _localBoard;

    public:
        int _sizeParameter;

        // TODO map de foncteur pour la factory
        std::string _whiteName;
        std::map<std::string, std::unique_ptr<Player>> _whitePlayers;

        std::string _blackName;
        std::map<std::string, std::unique_ptr<Player>> _blackPlayers;

    public:
        Controller(int argc, char ** argv);
        ~Controller();
        void run();

        void newGame();
        void clickCell(const Cell & refCell);
        bool isComputing() const;
        int getSize() const;
        bool isGameFinished() const;

        cell_t getCurrentCellt() const;
        cell_t getWinnerCellt() const;
        cell_t getCelltAtCell(const Cell & refCell) const;
        Cell getLastCell() const; // return (-1, -1) if no previous cell

        void guiCells(std::vector<Cell> & whiteCells, 
                std::vector<Cell> & blackCells, 
                std::vector<Cell> & emptyCells) const;

    private:
        void updateLocalData();
};

#endif
