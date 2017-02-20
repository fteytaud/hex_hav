
#include <hex_hav/PlayerRaveMast.hpp>

#include <algorithm>

PlayerRaveMast::PlayerRaveMast() :
    _mastWins(MAX_BOARDSIZE * MAX_BOARDSIZE, 0), 
    _mastSims(MAX_BOARDSIZE * MAX_BOARDSIZE, 0)
{}

void PlayerRaveMast::newTurn(const BOARD_TYPE & refBoard) 
{
    PlayerRave::newTurn(refBoard);
    std::fill(std::begin(_mastWins), std::end(_mastWins), 0);
    std::fill(std::begin(_mastSims), std::end(_mastSims), 0);
}

cell_t PlayerRaveMast::computeSimulation(NodeRave * ptrNode) 
{
    // playout
    BOARD_TYPE testBoard(ptrNode->_board);
    while (not testBoard.isGameFinished()) 
    {
        // compute CDF for all possible moves
        std::vector<int> moveIndices = testBoard.findEmptyIndices();
        std::vector<double> mastCdf(moveIndices.size());
        double sumProba = 0.0;
        auto fCdf = [this,&sumProba](int moveIndex) 
        { 
            int mastSims = this->_mastSims[moveIndex];
            sumProba += mastSims==0 ? 1.0 :
                exp(this->_mastWins[moveIndex]/double(mastSims));
            return sumProba;
        };
        std::transform(moveIndices.begin(), moveIndices.end(), 
                mastCdf.begin(), fCdf);

        // choose a move according to CDF
        double rnd = sumProba * _prngRaveSim.generate();
        auto iter = std::find_if(mastCdf.begin(), mastCdf.end(), 
                [rnd] (double cdf) { return cdf >= rnd; });
        assert(iter != mastCdf.end());
        int cdfIndex = std::distance(mastCdf.begin(), iter);
        int indexToPlay = moveIndices[cdfIndex];

        // play the move
        testBoard.play(indexToPlay);
        _allMovesInSim.push_back(indexToPlay);
    }

    return testBoard.getWinnerCellt();
}

void PlayerRaveMast::computeBackpropagation(NodeRave* ptrNode, cell_t winCellt) 
{
    // update MCTS and Rave stats
    PlayerRave::computeBackpropagation(ptrNode, winCellt);

    // update MAST stats
    for (unsigned i = 0; i < _allMovesInSim.size(); i+=2) 
    {
        unsigned moveIndex = _allMovesInSim[i];
        if (winCellt == _playerCellt) 
        { 
            _mastWins[moveIndex]++;
            _mastSims[moveIndex]++;
        }
        else 
        { 
            _mastSims[moveIndex]++;
        }
    }
}

