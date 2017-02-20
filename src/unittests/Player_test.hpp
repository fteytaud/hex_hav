// Copyright © 2014 Teytaud & Dehos <{teytaud,dehos}@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef PLAYER_TEST_HPP_
#define PLAYER_TEST_HPP_

#include "Player.hpp"
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

#endif

