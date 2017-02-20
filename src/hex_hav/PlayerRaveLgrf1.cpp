
#include <hex_hav/PlayerRaveLgrf1.hpp>

PlayerRaveLgrf1::PlayerRaveLgrf1() :
    _lastGoodReplies(MAX_BOARDSIZE*MAX_BOARDSIZE, -1)
{}

void PlayerRaveLgrf1::newTurn(const BOARD_TYPE & refBoard) 
{
    PlayerRave::newTurn(refBoard);
    std::fill(std::begin(_lastGoodReplies), std::end(_lastGoodReplies), -1);
}

cell_t PlayerRaveLgrf1::computeSimulation(NodeRave *ptrNode) 
{
    BOARD_TYPE testBoard(ptrNode->_board);

    while (not testBoard.isGameFinished()) 
    {
        // get last good reply 
        int indexToPlay = _lastGoodReplies[testBoard.getLastIndex()];
        if (testBoard.getCurrentCellt() == _playerCellt)
            indexToPlay = -1;

        // if no last good reply
        if (indexToPlay == -1 
                or not testBoard.isValidIndex(indexToPlay) 
                or testBoard.getCelltAtIndex(indexToPlay) != CELL_EMPTY) 
        {
            // chose a random move
            int nbEmptyIndices = testBoard.getNbEmptyIndices();
            int n = nbEmptyIndices * _prngRaveSim.generate();
            indexToPlay = testBoard.findNthEmptyIndex(n);
        }

        // play the move
        _allMovesInSim.push_back(indexToPlay);
        testBoard.play(indexToPlay);

    }

    return testBoard.getWinnerCellt();
}

void PlayerRaveLgrf1::computeBackpropagation(NodeRave* ptrNode, cell_t winCellt) 
{
    // update MCTS and Rave stats
    PlayerRave::computeBackpropagation(ptrNode, winCellt);

    // update last good replies
    // if we have a winning playout
    if (winCellt == _playerCellt)
        // store playout moves in the last good replies
        for (unsigned i=1; i<_allMovesInSim.size()-1; i+=2) 
            _lastGoodReplies[_allMovesInSim[i]] = _allMovesInSim[i+1];
    else
        // remove playout from the last good replies
        for (unsigned i=1; i<_allMovesInSim.size()-1; i+=2) 
            _lastGoodReplies[i] = -1;
}

