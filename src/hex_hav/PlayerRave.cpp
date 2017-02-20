
#include <hex_hav/PlayerRave.hpp>

///////////////////////////////////////////////////////////////////////////////
// NodeRave
///////////////////////////////////////////////////////////////////////////////

NodeRave::NodeRave(const BOARD_TYPE & b) :
    NodeMcts(b), 
    _raveChildWins(_board.getNbFullIndices(), 0.0), 
    _raveChildSims(_board.getNbFullIndices(), 0.0) 
{}

NodeRave::NodeRave(const BOARD_TYPE & b, int index, NodeRave * ptrParentNode) :
    NodeMcts(b, index, ptrParentNode), 
    _raveChildWins(_board.getNbFullIndices(), 0.0), 
    _raveChildSims(_board.getNbFullIndices(), 0.0) 
{}

BoardEval NodeRave::computeBoardEval(unsigned nbSims) const
{
    BoardEval boardEval;
    boardEval._nbSims = nbSims;
    boardEval._nbSimsBest = 0;
    boardEval._scoreBest = -1.0;
    for (const NodeMcts * pNode : _childNodes) 
    {
        Cell cell = _board.convertIndexToCell(pNode->_index);
        if (pNode->_nbSims < BoardEval::SIMS_THRESHOLD)
        {
            boardEval._cellEvals.push_back(
                    {cell._i, cell._j, pNode->_nbSims, 0.0});
        }
        else
        {
            double score = pNode->_nbWins / double(pNode->_nbSims);
            boardEval._cellEvals.push_back(
                    {cell._i, cell._j, pNode->_nbSims, score});
            if (pNode->_nbSims > boardEval._nbSimsBest)
            {
                boardEval._scoreBest = score;
                boardEval._nbSimsBest = pNode->_nbSims;
                boardEval._iBest = cell._i;
                boardEval._jBest = cell._j;
            }
        }
    }
    return boardEval;
}

///////////////////////////////////////////////////////////////////////////////
// PlayerRave
///////////////////////////////////////////////////////////////////////////////

PlayerRave::PlayerRave() 
{
    _parameters["KRAVE"] = 130;
    _parameters["KUCT"] = 0;
    _allMovesInSim.reserve(MAX_BOARDSIZE * MAX_BOARDSIZE);
}

BoardEval PlayerRave::computeBoardEval() const
{
    unsigned nbSims = _parameters.at("nbSimulations");
    return _nodes.front().computeBoardEval(nbSims);
}

void PlayerRave::computeOneSimulationInit() 
{
    _allMovesInSim.clear();
}

NodeRave * PlayerRave::computeSelection(NodeRave * ptrNode) 
{
    assert(not ptrNode->_childNodes.empty());
    // find the best child node
    NodeRave * ptrBestNode = nullptr;
    double bestScore = -1.0;
    for (NodeMcts * p : ptrNode->_childNodes)
    {
        NodeRave * ptrChildNode = (NodeRave*)p;
        double score = computeScoreUctRave(ptrChildNode);
        if (score > bestScore) 
        {
            ptrBestNode = ptrChildNode;
            bestScore = score;
        }
    }
    // add the node in the vector of node to update
    _allMovesInSim.push_back(ptrBestNode->_board.getLastIndex());
    return ptrBestNode;
}

int PlayerRave::computeExpansion(NodeRave * ptrNode) 
{
    assert(not ptrNode->_possibleIndices.empty());
    // just get one of the possible nodes
    int index = ptrNode->_possibleIndices.back();
    ptrNode->_possibleIndices.pop_back();
    // add the node in the vector of node to update
    _allMovesInSim.push_back(index);
    return index;
}

cell_t PlayerRave::computeSimulation(NodeRave *ptrNode) 
{
    // playout
    BOARD_TYPE testBoard(ptrNode->_board);
    while (not testBoard.isGameFinished()) 
    {
        // compute move to play
        int nbEmptyIndices = testBoard.getNbEmptyIndices();
        int n = _prngRaveSim.generate() * nbEmptyIndices;
        int indexToPlay = testBoard.findNthEmptyIndex(n);
        // store player moves
        _allMovesInSim.push_back(indexToPlay);
        // play move
        testBoard.play(indexToPlay);
    }
    return testBoard.getWinnerCellt();
}

void PlayerRave::computeBackpropagation(NodeRave * ptrNode, cell_t winnerCellt) 
{
    // update all nodes, from leaf to root
    while (ptrNode) 
    {
        cell_t nodeOpponentCellt = ptrNode->_board.getCurrentCellt();
        assert(nodeOpponentCellt != CELL_EMPTY);

        // index of the first cell of the node player, in _allMovesInSim
        int i0Move = (nodeOpponentCellt != _playerCellt) ? 1 : 0;

        // update for a winning simulation
        if (winnerCellt != nodeOpponentCellt) 
        {
            // update MCTS
            ptrNode->_nbWins++;
            ptrNode->_nbSims++;
            // update RAVE
            for (unsigned i = i0Move; i < _allMovesInSim.size(); i+=2) 
            {
                unsigned moveIndex = _allMovesInSim[i];
                ptrNode->_raveChildSims[moveIndex]++;
            }
        }
        // update for a losing simulation
        else 
        { 
            // update MCTS
            ptrNode->_nbSims++;
            // update RAVE
            for (unsigned i = i0Move; i < _allMovesInSim.size(); i+=2) 
            {
                unsigned moveIndex = _allMovesInSim[i];
                ptrNode->_raveChildWins[moveIndex]++;
                ptrNode->_raveChildSims[moveIndex]++; 
            }
        }
        // next node (ie parent node)
        ptrNode = (NodeRave*)ptrNode->_ptrParentNode;
    }
}

double PlayerRave::computeScoreUctRave(NodeRave * ptrNode) const 
{
    int index = ptrNode->_index;
    NodeRave * ptrParentNode = (NodeRave*)(ptrNode->_ptrParentNode);
    double currentSims = ptrNode->_nbSims;
    double parentSims = ptrParentNode->_nbSims;

    // UCT score
    double KUCT = _parameters.at("KUCT");
    double exploitation = ptrNode->getWinRatio();
    double exploration = std::sqrt(std::log(parentSims) / currentSims);

    // RAVE score
    double KRAVE = _parameters.at("KRAVE");
    double raveWins = ptrParentNode->_raveChildWins[index];
    double raveSims = ptrParentNode->_raveChildSims[index];
    double scoreAmaf = (1.0 + raveWins) / (2.0 + raveSims);

    // final score
    double beta = sqrt(KRAVE / (KRAVE + 3.0 * currentSims));
    return (1.0 - beta) * exploitation + beta * scoreAmaf + KUCT * exploration;
}

#ifdef LOG
void PlayerRave::printLog() 
{
    const NodeRave &root = _nodes.front();
    std::vector<double> superTest(root._raveChildWins.size());
    for (unsigned t = 0; t < superTest.size(); t++) 
        superTest[t] = root._raveChildWins[t]/(double)root._raveChildSims[t];
    root._board.printLogWithData(superTest);
}
#endif  // LOG

