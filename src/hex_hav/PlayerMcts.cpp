
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

