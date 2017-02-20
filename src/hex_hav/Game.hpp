
#ifndef GAME_HPP_
#define GAME_HPP_

#include <hex_hav/Board.hpp>
#include <hex_hav/Chrono.hpp>
#include <hex_hav/Player.hpp>

#include <memory>

class Game 
{
    private:
        Chrono _chrono;

        BOARD_TYPE _board;

        Player * _ptrWhitePlayer;
        Player * _ptrBlackPlayer;

        // TODO refactor havannah stats
    public:
        std::array<int, 7> _stats;

    public:
        Game();

        void reset(int size, Player * ptrWhitePlayer, Player * ptrBlackPlayer);
        void clickIndex(int index);
        bool hasTurnToPlay() const;
        void playTurn();

        const std::string getBoardType() const;
        int getSize() const;
        int getLastCellIndex() const;
        cell_t getWinnerCellt() const;
        cell_t getCurrentCellt() const;

        // for Controller
        const BOARD_TYPE & getRefBoard() const;

        Player * getPtrCurrentPlayer() const;
};

#endif  // GAME_HPP_
