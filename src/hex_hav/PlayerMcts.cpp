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

#include <hex_hav/PlayerMcts.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// NodeMcts
///////////////////////////////////////////////////////////////////////////////

NodeMcts::NodeMcts(const BOARD_TYPE & b) : 
    _board(b), 
    _index(-1), 
    _nbWins(0), 
    _nbSims(0), 
    _ptrParentNode(nullptr) 
{
    _possibleIndices = _board.findEmptyIndices();
    _childNodes.reserve(_possibleIndices.size());
}

NodeMcts::NodeMcts(const BOARD_TYPE & b, int index, NodeMcts * ptrParentNode) :
    _board(b), 
    _index(index), 
    _nbWins(0),
    _nbSims(0), 
    _ptrParentNode(ptrParentNode) 
{
    _board.play(_index);
    _possibleIndices = _board.findEmptyIndices();
    // slower with _childNodes.reserve 
}

NodeMcts * NodeMcts::getBestNode() const 
{
    assert(not _childNodes.empty());
    auto cmpSims = [] (NodeMcts* ptrNode1, NodeMcts* ptrNode2) 
    { return ptrNode1->_nbSims < ptrNode2->_nbSims; };
    auto iter = std::max_element(_childNodes.begin(), 
            _childNodes.end(), cmpSims);
    return *iter;
}

double NodeMcts::getWinRatio() const 
{
    return _nbSims>0 ? _nbWins/double(_nbSims) : 0.0;
}

///////////////////////////////////////////////////////////////////////////////
// PlayerMcts
///////////////////////////////////////////////////////////////////////////////

PlayerMcts::PlayerMcts() 
{
    _parameters["KUCT"] = 0.25;
}

NodeMcts * PlayerMcts::computeSelection(NodeMcts * ptrNode) 
{
    assert(not ptrNode->_childNodes.empty());
    NodeMcts * ptrBestNode = nullptr;
    double bestScore = -1;
    for (NodeMcts * ptrChildNode : ptrNode->_childNodes) 
    {
        double score = computeScoreUct(ptrChildNode);
        assert(score >= 0);
        if (score > bestScore) 
        {
            ptrBestNode = ptrChildNode;
            bestScore = score;
        }
    }
    return ptrBestNode;
}

int PlayerMcts::computeExpansion(NodeMcts * ptrNode) 
{
    int index = ptrNode->_possibleIndices.back();
    ptrNode->_possibleIndices.pop_back();
    return index;
}

cell_t PlayerMcts::computeSimulation(NodeMcts * ptrNode) 
{
    BOARD_TYPE testBoard(ptrNode->_board);
    while (not testBoard.isGameFinished()) 
    {
        int nbEmptyIndices = testBoard.getNbEmptyIndices();
        int n = _prngMctsSim.generate() * nbEmptyIndices;
        int indexToPlay = testBoard.findNthEmptyIndex(n);
        testBoard.play(indexToPlay);
    }
    return testBoard.getWinnerCellt();
}

void PlayerMcts::computeBackpropagation(NodeMcts * ptrNode, cell_t winnerCellt) 
{
    while (ptrNode) 
    {
        cell_t nodeOpponentCellt = ptrNode->_board.getCurrentCellt();
        if (winnerCellt != nodeOpponentCellt) 
            ptrNode->_nbWins++;
        ptrNode->_nbSims++;
        ptrNode = ptrNode->_ptrParentNode;
    }
}

double PlayerMcts::computeScoreUct(NodeMcts * ptrNode) const 
{
    double KUCT = _parameters.at("KUCT");
    double parentSims = ptrNode->_ptrParentNode->_nbSims;
    double currentSims = ptrNode->_nbSims;
    double exploitation = ptrNode->getWinRatio();
    double exploration = std::sqrt(std::log(parentSims) / currentSims);
    return exploitation + KUCT * exploration;
}

