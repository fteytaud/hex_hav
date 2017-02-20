// Copyright © 2014 Teytaud & Dehos <{teytaud,dehos}@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef PLAYER_MCTS_TEST_HPP_
#define PLAYER_MCTS_TEST_HPP_

#include "PlayerMcts.hpp"
#include <cxxtest/TestSuite.h>

class PlayerMcts_test : public CxxTest::TestSuite, public PlayerMcts 
{ 
    public: 
        void testNewTurn1() 
        { 
            PlayerMcts_test p; 
            p._parameters.at("nbSimulations") = 1000; 
            BOARD_TYPE b; 
            b.reset(5); 
            TS_ASSERT(not p.hasIndexToPlay()); 
            p.newTurn(b);
            TS_ASSERT(p.hasIndexToPlay()); 
        }
        void testNewTurn2() 
        { 
            PlayerMcts_test p; 
            p._parameters.at("nbSimulations") = 1000; 
            BOARD_TYPE b; 
            b.reset(5); 

            p.newTurn(b);
            TS_ASSERT_EQUALS(p._playerCellt, CELL_WHITE); 
            b.play(p.getIndexToPlay());

            p.newTurn(b);
            TS_ASSERT_EQUALS(p._playerCellt, CELL_BLACK); 
        } 
        void testTurnInBoard() 
        {
            PlayerMcts p;
            p._parameters.at("nbSimulations") = 1000; 
            BOARD_TYPE b; 
            b.reset(6); 
            for (int i=0; i<100; i++) 
            { 
                p.newTurn(b); 
                int index = p.getIndexToPlay(); 
                Cell c = b.convertIndexToCell(index); 
                TS_ASSERT_LESS_THAN_EQUALS(0, c._i); 
                TS_ASSERT_LESS_THAN(c._i, 6); 
                TS_ASSERT_LESS_THAN_EQUALS(0, c._j); 
                TS_ASSERT_LESS_THAN(c._j, 6); 
            } 
        } 
        void testValidTurn() 
        { 
            PlayerMcts p; 
            p._parameters.at("nbSimulations") = 1000; 
            BOARD_TYPE b; 
            b.reset(7); 
            b.play(13); 
            for (int i=0; i<100; i++) 
            { 
                p.newTurn(b); TS_ASSERT_DIFFERS(p.getIndexToPlay(), 13); 
                TS_ASSERT_DIFFERS(p.getIndexToPlay(), 13); 
            } 
        } 
        void testGeneric_playerCellt() 
        {
            PlayerMcts_test p; 
            p._parameters.at("nbSimulations") = 1000; 
            BOARD_TYPE b; 
            b.reset(11); 
            cell_t opponent = CELL_BLACK;
            for (int i=0; i<11; i++) 
            {
                p.newTurn(b);
                TS_ASSERT_DIFFERS(opponent, p._playerCellt);
                int index = p.getIndexToPlay(); 
                b.play(index);
                opponent = p._playerCellt;
            }
        }
        void testGeneric_computeOneSimulation_1() 
        {
            BOARD_TYPE b; 
            b.reset(11); 

            PlayerMcts_test p; 
            p._playerCellt = b.getCurrentCellt();
            p._nodes.clear();
            p._nodes.reserve(1000);
            p._nodes.emplace_back(b);
            TS_ASSERT_EQUALS(p._nodes.size(), 1u);

            // root node before simulations
            NodeMcts * ptrRootNode = std::addressof(p._nodes.front());
            TS_ASSERT_EQUALS(ptrRootNode->_index, -1);
            TS_ASSERT_EQUALS(ptrRootNode->_nbWins, 0u);
            TS_ASSERT_EQUALS(ptrRootNode->_nbSims, 0u);
            TS_ASSERT_EQUALS(ptrRootNode->_ptrParentNode, nullptr);
            TS_ASSERT_EQUALS(ptrRootNode->_childNodes.size(), 0u);
            TS_ASSERT_EQUALS(ptrRootNode->_possibleIndices.size(), 121u);

            // first simulations: create child nodes
            for (unsigned i=1; i<=121; i++) 
            {
                p.computeOneSimulation();
                TS_ASSERT_EQUALS(ptrRootNode->_nbSims, i);
            }
            ptrRootNode = std::addressof(p._nodes.front());
            TS_ASSERT_EQUALS(ptrRootNode->_possibleIndices.size(), 0u);
            TS_ASSERT_EQUALS(ptrRootNode->_childNodes.size(), 121u);

            // next simulations: update children:
            for (unsigned i=1; i<=120; i++) 
            {
                p.computeOneSimulation();
                TS_ASSERT_EQUALS(ptrRootNode->_nbSims, 121+i);
            }
            TS_ASSERT_EQUALS(ptrRootNode->_childNodes.size(), 121u);
            for (unsigned i=1; i<_nodes.size(); i++) 
            {
                const NodeMcts & refNode = p._nodes[i];
                TS_ASSERT_EQUALS(refNode._nbSims, 1 + refNode._childNodes.size());
                TS_ASSERT_EQUALS(120u, 
                        refNode._childNodes.size() + refNode._possibleIndices.size());
                TS_ASSERT_EQUALS(refNode._ptrParentNode, ptrRootNode);
            }
        }
        void testGeneric_computeOneSimulation_2() 
        {
            // initialize board
            BOARD_TYPE b; 
            b.reset(7); 

            // initialize player
            PlayerMcts_test p; 
            p._playerCellt = b.getCurrentCellt();
            p._nodes.clear();
            p._nodes.reserve(1001);
            p._nodes.emplace_back(b);
            for (int i=0; i<1000; i++) 
                p.computeOneSimulation();

            // check wins/losses for a node and its children
            for (unsigned i=1; i<p._nodes.size(); i++) 
            {
                const NodeMcts & refNode = p._nodes[i];
                unsigned nbChildWins = 0;
                unsigned nbChildSims = 0;
                for (NodeMcts * ptrChildNode : refNode._childNodes) 
                {
                    TS_ASSERT_EQUALS(ptrChildNode->_ptrParentNode, 
                            std::addressof(refNode));
                    nbChildWins += ptrChildNode->_nbWins;
                    nbChildSims += ptrChildNode->_nbSims;
                }
                TS_ASSERT_EQUALS(refNode._nbSims, nbChildSims + 1);
                TS_ASSERT_LESS_THAN_EQUALS(nbChildWins, nbChildSims);
                unsigned nbChildLosses = nbChildSims - nbChildWins;
                TS_ASSERT(refNode._nbWins == nbChildLosses 
                        or refNode._nbWins == nbChildLosses+1);
            }

            // check nbSims for root node
            int nbSims=0;
            for (int i=1; i<=49; i++) 
                nbSims += p._nodes[i]._nbSims;            
            TS_ASSERT_EQUALS(nbSims, 1000);
        }
}; 

#endif

