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

#ifndef PLAYER_MCTS_T_HPP_
#define PLAYER_MCTS_T_HPP_

#include <hex_hav/Player.hpp>

#include <cassert>
#include <iostream>

////////////////////////////////////////////////////////////////////////
// template declaration
////////////////////////////////////////////////////////////////////////

template<typename T>
class PlayerMctsT : public PlayerAi 
{
    protected:
        std::vector<T> _nodes;
        cell_t _playerCellt;

    public:
        PlayerMctsT();
        virtual void newTurn(const BOARD_TYPE & refBoard) override;

    protected:
        virtual void computeOneSimulationInit() {}
        void computeOneSimulation();

    protected:
        virtual T * computeSelection(T * ptrNode) = 0;
        virtual int computeExpansion(T * ptrNode) = 0;
        virtual cell_t computeSimulation(T * ptrNode) = 0;
        virtual void computeBackpropagation(T * ptrNode, cell_t winCellt) = 0;

#ifdef LOG
        virtual void printLog() {}
#endif  // LOG

};

////////////////////////////////////////////////////////////////////////
// template implementation
////////////////////////////////////////////////////////////////////////

    template<typename T>
PlayerMctsT<T>::PlayerMctsT() 
{
    _parameters["nbSimulations"] = 10000;
}

    template<typename T>
void PlayerMctsT<T>::computeOneSimulation() 
{

    computeOneSimulationInit();

    // selection : select a leaf node
    T * ptrCurrentNode = std::addressof(_nodes.front());
    while (not ptrCurrentNode->_childNodes.empty() 
            and ptrCurrentNode->_possibleIndices.empty()) 
    {
        ptrCurrentNode = computeSelection(ptrCurrentNode);
    }

    // expansion : unless the current node ends the game, create a new node
    if (not ptrCurrentNode->_board.isGameFinished()) 
    {
        // select one cell index
        int index = computeExpansion(ptrCurrentNode);

        // create a node
        const BOARD_TYPE & board = ptrCurrentNode->_board;
        _nodes.emplace_back(board, index, ptrCurrentNode);

        // store this new child node in parent node
        T * ptrChildNode = std::addressof(_nodes.back());
        ptrCurrentNode->_childNodes.push_back(ptrChildNode);

        // select child node as current node
        ptrCurrentNode = ptrChildNode;
    }

    // simulation
    cell_t winnerCellt = computeSimulation(ptrCurrentNode);

    // backpropagation
    computeBackpropagation(ptrCurrentNode, winnerCellt);
}

    template<typename T>
void PlayerMctsT<T>::newTurn(const BOARD_TYPE & refBoard) 
{

    const unsigned int nbSimulations = _parameters["nbSimulations"];
    _playerCellt = refBoard.getCurrentCellt();

    // build search tree
    _nodes.clear();
    // warning: _nodes must be fully reserved !!!
    _nodes.reserve(nbSimulations+1);
    _nodes.emplace_back(refBoard);

    // compute simulations
    for (unsigned int i=0; i<nbSimulations; i++)
        computeOneSimulation();

    // get best cell to play
    T * ptrBestNode = (T *)_nodes.front().getBestNode();
    assert(ptrBestNode);
    setIndexToPlay(ptrBestNode->_index);

#ifdef LOG
    printLog();
    std::clog << BOARD_TYPE::convertCelltToString(refBoard.getCurrentCellt())
        << "\t(nbSimulations = " << nbSimulations 
        << ")\twinRatio = " << ptrBestNode->getWinRatio() << std::endl;
#endif  // LOG

}

#endif  // PLAYER_MCTS_T_HPP_

