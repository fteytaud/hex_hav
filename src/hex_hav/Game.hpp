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

