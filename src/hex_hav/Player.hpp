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

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <hex_hav/BoardHex.hpp>
#include <hex_hav/BoardHavannah.hpp>
#include <hex_hav/Prng.hpp>

#include <list>
#include <map>
#include <memory>

// abstract class to implement players (human, AIs)
class Player 
{
    private:
        int _indexToPlay;
        bool _hasIndexToPlay;

    public:
        // TODO store player parameters as strings then convert them in player
        // implementation
        std::map<std::string, double> _parameters;

    public:
        Player();
        virtual ~Player() = default;

        virtual void newTurn(const BOARD_TYPE & refBoard) = 0;
        virtual void clickIndex(int index) = 0;

        void resetTurn();
        bool hasIndexToPlay() const;
        int getIndexToPlay() const;

        void setIndexToPlay(int index);
};

// human player : play the cells clicked by the user
class PlayerHuman : public Player 
{
    public:
        void newTurn(const BOARD_TYPE & refBoard) override ;
        void clickIndex(int index) override ;
};

// to implement an AI, derive from this class and define newTurn
class PlayerAi : public Player 
{
    public:
        void clickIndex(int index) override ;
};

// play randomly
class PlayerRandom : public PlayerAi 
{
    protected:
        Prng _prng;
    public:
        PlayerRandom();
        void newTurn(const BOARD_TYPE & refBoard) override ;
};

// basic Monte Carlo
class PlayerMc : public PlayerAi 
{
    protected:
        Prng _prng;
    public:
        PlayerMc();
        void newTurn(const BOARD_TYPE & refBoard) override ;
    private:
        cell_t playRandomGameWithFirstIndex(const BOARD_TYPE & refBoard, 
                int firstIndex);
};

#endif  // PLAYER_HPP_

