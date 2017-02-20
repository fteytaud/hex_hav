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

#ifndef PLAYER_RAVE_TEST_HPP_
#define PLAYER_RAVE_TEST_HPP_

#include <hex_hav/PlayerRave.hpp>
#include <cxxtest/TestSuite.h>

class PlayerRave_test : public CxxTest::TestSuite, public PlayerRave 
{ 
    public: 
        void testNewTurn1() 
        { 
            PlayerRave_test p; 
            p._parameters.at("nbSimulations") = 1000; 
            BOARD_TYPE b; 
            b.reset(5); 
            TS_ASSERT(not p.hasIndexToPlay()); 
            p.newTurn(b);
            TS_ASSERT(p.hasIndexToPlay()); 
        }

        void testNewTurn2() 
        { 
            PlayerRave_test p; 
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
            PlayerRave_test p;
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
            PlayerRave_test p; 
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
            PlayerRave_test p; 
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

            PlayerRave_test p; 
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
                TS_ASSERT_EQUALS(refNode._childNodes.size() + refNode._possibleIndices.size(), 120u);
                TS_ASSERT_EQUALS(refNode._ptrParentNode, ptrRootNode);
            }
        }

        void testGeneric_computeOneSimulation_2() 
        {
            // initialize board
            BOARD_TYPE b; 
            b.reset(7); 

            // initialize player
            PlayerRave_test p; 
            p._playerCellt = b.getCurrentCellt();
            p._nodes.clear();
            p._nodes.reserve(1001);
            p._nodes.emplace_back(b);
            for (int i=0; i<100; i++) 
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
            TS_ASSERT_EQUALS(nbSims, 100);
        }

        void test_rave0() 
        {
            // initialize board
            BOARD_TYPE b; 
            b.reset(5);

            // initialize player
            PlayerRave_test p; 
            p._playerCellt = b.getCurrentCellt();
            p._nodes.clear();
            p._nodes.reserve(1001);
            p._nodes.emplace_back(b);

            for (int k=0; k<100; k++) 
            {
                p.computeOneSimulation();
                int nbMoves = p._allMovesInSim.size();
                TS_ASSERT_LESS_THAN(5, nbMoves);
                TS_ASSERT_LESS_THAN(nbMoves, 26);
                for (int i=0; i<nbMoves; i++) 
                {
                    TS_ASSERT_LESS_THAN_EQUALS(0, p._allMovesInSim[i]);
                    TS_ASSERT_LESS_THAN_EQUALS(p._allMovesInSim[i], 24);
                }
            }
        }

        void test_rave1() 
        {
            // initialize board
            BOARD_TYPE b; 
            b.reset(5);

            // initialize player
            PlayerRave_test p; 
            p._playerCellt = b.getCurrentCellt();
            p._nodes.reserve(1001);

            for (int k=0; k<100; k++) 
            {
                p._nodes.clear();
                p._nodes.emplace_back(b);

                // do 1 simulation and check _raveChildWins, _raveChildSims...
                p.computeOneSimulation();
                int nbMoves = p._allMovesInSim.size();
                TS_ASSERT_LESS_THAN(5, nbMoves);
                TS_ASSERT_LESS_THAN(nbMoves, 26);
                int rootWin = nbMoves % 2 == 0;
                for (int i=0; i<nbMoves; i++) 
                {
                    int index = p._allMovesInSim[i];
                    if (i%2 == 0) 
                    {
                        // root
                        TS_ASSERT_EQUALS(p._nodes[0]._raveChildWins[index], 1-rootWin);
                        TS_ASSERT_EQUALS(p._nodes[0]._raveChildSims[index], 1);
                        // child 
                        TS_ASSERT_EQUALS(p._nodes[1]._raveChildWins[index], 0);
                        TS_ASSERT_EQUALS(p._nodes[1]._raveChildSims[index], 0);
                    }
                    else 
                    {
                        // root
                        TS_ASSERT_EQUALS(p._nodes[0]._raveChildWins[index], 0);
                        TS_ASSERT_EQUALS(p._nodes[0]._raveChildSims[index], 0);
                        // child 
                        TS_ASSERT_EQUALS(p._nodes[1]._raveChildWins[index], rootWin);
                        TS_ASSERT_EQUALS(p._nodes[1]._raveChildSims[index], 1);
                    }
                }
            }
        }

        void test_rave2() 
        {
            // initialize board
            BOARD_TYPE b; 
            b.reset(5);

            // initialize player
            PlayerRave_test p; 
            p._playerCellt = b.getCurrentCellt();
            p._nodes.clear();
            p._nodes.reserve(1001);
            p._nodes.emplace_back(b);


            // do 25 simulations and check rave update
            for (int iSim=0; iSim<25; iSim++) 
            {

                std::vector<double> oldRootWins = p._nodes[0]._raveChildWins;
                std::vector<double> oldRootSims = p._nodes[0]._raveChildSims;

                p.computeOneSimulation();
                int nbMoves = p._allMovesInSim.size();
                TS_ASSERT_LESS_THAN(5, nbMoves);
                TS_ASSERT_LESS_THAN(nbMoves, 26);
                int rootWin = nbMoves % 2 == 0;
                for (int i=0; i<nbMoves; i++) 
                {
                    int index = p._allMovesInSim[i];
                    if (i%2 == 0) 
                    {
                        // root
                        TS_ASSERT_EQUALS(p._nodes[0]._raveChildWins[index], 
                                oldRootWins[index]+1-rootWin);
                        TS_ASSERT_EQUALS(p._nodes[0]._raveChildSims[index], 
                                oldRootSims[index]+1);
                        // child 
                        TS_ASSERT_EQUALS(p._nodes.back()._raveChildWins[index], 0);
                        TS_ASSERT_EQUALS(p._nodes.back()._raveChildSims[index], 0);
                    }
                    else 
                    {
                        // root
                        TS_ASSERT_EQUALS(p._nodes[0]._raveChildWins[index], 
                                oldRootWins[index]);
                        TS_ASSERT_EQUALS(p._nodes[0]._raveChildSims[index], 
                                oldRootSims[index]);
                        // child 
                        TS_ASSERT_EQUALS(p._nodes.back()._raveChildWins[index], rootWin);
                        TS_ASSERT_EQUALS(p._nodes.back()._raveChildSims[index], 1);
                    }
                }
            }

            // check root rave
            for (int i=0; i<25; i++) 
            {
                TS_ASSERT_LESS_THAN_EQUALS(p._nodes[0]._raveChildSims[i], 25);
                TS_ASSERT_LESS_THAN_EQUALS(0, p._nodes[0]._raveChildSims[i]);
                TS_ASSERT_LESS_THAN_EQUALS(p._nodes[0]._raveChildWins[i], 
                        p._nodes[0]._raveChildSims[i]);
                TS_ASSERT_LESS_THAN_EQUALS(0, p._nodes[0]._raveChildWins[i]);
            }

            // check child rave
            for (int iNode=1; iNode<=25; iNode++) 
            {
                for (int i=0; i<25; i++) 
                {
                    TS_ASSERT_LESS_THAN_EQUALS(p._nodes[iNode]._raveChildSims[i], 1);
                    TS_ASSERT_LESS_THAN_EQUALS(0, p._nodes[iNode]._raveChildSims[i]);
                    TS_ASSERT_LESS_THAN_EQUALS(p._nodes[iNode]._raveChildWins[i], 1);
                    TS_ASSERT_LESS_THAN_EQUALS(0, p._nodes[iNode]._raveChildWins[i]);
                }
            }
        }

        void test_rave3() 
        {
            // initialize board
            BOARD_TYPE b; 
            b.reset(7);

            // initialize player
            PlayerRave_test p; 
            p._playerCellt = b.getCurrentCellt();
            p._nodes.clear();
            p._nodes.reserve(1001);
            p._nodes.emplace_back(b);

            for (int k=0; k<1000; k++) 
                p.computeOneSimulation();

            // for all node n check rave( n ) = sum( rave( child( child( n ) ) )
            auto fSumChild = [] (const NodeMcts * ptrNodeMcts, 
                    std::vector<double> & sumRaveWins, std::vector<double> & sumRaveSims) 
            {
                const NodeRave * ptrNode = (NodeRave*)ptrNodeMcts;
                assert(ptrNode);
                unsigned nbIndices = sumRaveSims.size();
                assert(nbIndices == sumRaveWins.size());
                for (const NodeMcts * ptrChildNodeMcts : ptrNode->_childNodes) 
                {
                    const NodeRave * ptrChildNode = (NodeRave*)(ptrChildNodeMcts);
                    assert(nbIndices == ptrChildNode->_raveChildWins.size());
                    assert(nbIndices == ptrChildNode->_raveChildSims.size());
                    for (unsigned i=0; i<nbIndices; i++) 
                    {
                        sumRaveWins[i] += ptrChildNode->_raveChildWins[i];
                        sumRaveSims[i] += ptrChildNode->_raveChildSims[i];
                    }
                }
            };

            for (const NodeRave & node : p._nodes) 
            {
                // compute rave sums (children of children of node)
                std::vector<double> sumRaveWins(49, 0);
                std::vector<double> sumRaveSims(49, 0);
                for (const NodeMcts * ptrChildNode : node._childNodes) 
                    fSumChild(ptrChildNode, sumRaveWins, sumRaveSims);

                int nbChildren = node._childNodes.size();

                // check root rave == sum rave (+/- nbChildren+1 because of 
                // first simulations of root and its children)
                for (int i=0; i<49; i++) 
                {
                    TS_ASSERT_LESS_THAN_EQUALS(sumRaveWins[i], node._raveChildWins[i]);
                    TS_ASSERT_LESS_THAN_EQUALS(node._raveChildWins[i], 
                            sumRaveWins[i]+nbChildren+1);
                    TS_ASSERT_LESS_THAN_EQUALS(sumRaveSims[i], node._raveChildSims[i]);
                    TS_ASSERT_LESS_THAN_EQUALS(node._raveChildSims[i], 
                            sumRaveSims[i]+nbChildren+1);
                }
            }
        }

}; 

#endif  // PLAYER_RAVE_TEST_HPP_

