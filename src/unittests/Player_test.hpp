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

#ifndef PLAYER_TEST_HPP_
#define PLAYER_TEST_HPP_

#include <hex_hav/Player.hpp>
#include <cxxtest/TestSuite.h>

class Player_test : public CxxTest::TestSuite, public Player 
{
    public:
        // implement fake abstract methods
        void newTurn(const BOARD_TYPE & ) {}
        void clickIndex(int ) {}

        void testConstructor() 
        {
            Player_test p;
            TS_ASSERT(not p.hasIndexToPlay());
        }

        void testSetIndexToPlay() 
        {
            Player_test p;
            p.setIndexToPlay(42);
            TS_ASSERT(p.hasIndexToPlay());
        }

        void testGetIndexToPlay() 
        {
            Player_test p;
            p.setIndexToPlay(37);
            TS_ASSERT_EQUALS(p.getIndexToPlay(), 37);
        }

        void testResetTurn() 
        {
            Player_test p;
            p.setIndexToPlay(12);
            p.resetTurn();
            TS_ASSERT(not p.hasIndexToPlay());
        }
};

class PlayerHuman_test : public CxxTest::TestSuite, public PlayerHuman 
{
    public:
        void testNewTurn() 
        {
            BOARD_TYPE b;
            PlayerHuman_test p;
            TS_ASSERT(not p.hasIndexToPlay());
            p.newTurn(b);
            TS_ASSERT(not p.hasIndexToPlay());
            p.setIndexToPlay(37);
            TS_ASSERT(p.hasIndexToPlay());
            p.newTurn(b);
            TS_ASSERT(not p.hasIndexToPlay());
        }

        void testClickIndex() 
        {
            BOARD_TYPE b;
            PlayerHuman_test p;
            p.newTurn(b);
            p.clickIndex(23);
            TS_ASSERT(p.hasIndexToPlay());
            TS_ASSERT_EQUALS(p.getIndexToPlay(), 23);
            p.clickIndex(45);
            TS_ASSERT(p.hasIndexToPlay());
            TS_ASSERT_EQUALS(p.getIndexToPlay(), 45);
        }
};

class PlayerAi_test : public CxxTest::TestSuite, public PlayerAi 
{
    public:
        // implement fake abstract methods
        void newTurn(const BOARD_TYPE & ) {}

        void testClickIndex() 
        {
            PlayerAi_test p;
            p.clickIndex(3);
            TS_ASSERT(not p.hasIndexToPlay());
            p.setIndexToPlay(22);
            p.clickIndex(31);
            TS_ASSERT(p.hasIndexToPlay());
            TS_ASSERT_EQUALS(p.getIndexToPlay(), 22);
        }
};

class PlayerRandom_test : public CxxTest::TestSuite 
{ 
    public: 
        void testNewTurn() 
        { 
            PlayerRandom p; 
            BOARD_TYPE b; 
            b.reset(5); 
            TS_ASSERT(not p.hasIndexToPlay()); 
            p.newTurn(b);
            TS_ASSERT(p.hasIndexToPlay()); 
        } \
        void testTurnInBoard() 
        {
            PlayerRandom p;
            BOARD_TYPE b; 
            b.reset(6); 
            for (int i=0; i<100; i++) 
            { 
                p.newTurn(b); 
                int index = p.getIndexToPlay(); 
                Cell c = b.convertIndexToCell(index); 
                TS_ASSERT(c._i >= 0); 
                TS_ASSERT(c._i < 6); 
                TS_ASSERT(c._j >= 0); 
                TS_ASSERT(c._j < 6); 
            } 
        } 
        void testValidTurn() 
        { 
            PlayerRandom p; 
            BOARD_TYPE b; 
            b.reset(7); 
            b.play(13); 
            for (int i=0; i<100; i++) 
            { 
                p.newTurn(b); 
                TS_ASSERT_DIFFERS(p.getIndexToPlay(), 13); 
            } 
        } 
}; 

class PlayerMc_test : public CxxTest::TestSuite 
{ 
    public: 
        void testNewTurn() 
        { 
            PlayerMc p; 
            BOARD_TYPE b; 
            b.reset(5); 
            TS_ASSERT(not p.hasIndexToPlay()); 
            p.newTurn(b);
            TS_ASSERT(p.hasIndexToPlay()); 
        } 
        void testTurnInBoard() 
        {
            PlayerMc p;
            BOARD_TYPE b; 
            b.reset(6); 
            for (int i=0; i<100; i++) 
            { 
                p.newTurn(b); 
                int index = p.getIndexToPlay(); 
                Cell c = b.convertIndexToCell(index); 
                TS_ASSERT(c._i >= 0); 
                TS_ASSERT(c._i < 6); 
                TS_ASSERT(c._j >= 0); 
                TS_ASSERT(c._j < 6); 
            } 
        } 
        void testValidTurn() 
        { 
            PlayerMc p; 
            BOARD_TYPE b; 
            b.reset(7); 
            b.play(13); 
            for (int i=0; i<100; i++) 
            { 
                p.newTurn(b); 
                TS_ASSERT_DIFFERS(p.getIndexToPlay(), 13); 
            } 
        } 
}; 

#endif  // PLAYER_TEST_HPP_

