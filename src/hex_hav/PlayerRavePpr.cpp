
#include <hex_hav/PlayerRavePpr.hpp>

#include <algorithm>

PlayerRavePpr::PlayerRavePpr() 
{
    _parameters["PERCENTAGE"] = 0.01;
    _parameters["POOLPROBA"] = 0.8;
    _parameters["POOLRATIO"] = 0.2;
    _allMovesInSim.reserve(MAX_BOARDSIZE * MAX_BOARDSIZE);
}

cell_t PlayerRavePpr::computeSimulation(NodeRave * ptrNode) 
{
    // get player parameters
    const double PERCENTAGE  = _parameters.at("PERCENTAGE");
    const double POOLPROBA = _parameters.at("POOLPROBA");
    const double POOLRATIO = _parameters.at("POOLRATIO");
    const double nbSimulations = _parameters.at("nbSimulations");

    // find a parent node having meaningful statistics for PPR
    NodeRave * ancestor = ptrNode;
    while (ancestor) 
    {
        if (ancestor->_nbSims/nbSimulations >= PERCENTAGE)
            break;
        ancestor = (NodeRave*)ancestor->_ptrParentNode;
    }

    // compute PPR moves
    std::vector<int> pprMoves = ptrNode->_board.findEmptyIndices();
    bool hasPprMoves = ancestor != nullptr;
    if (hasPprMoves) 
    {
        // pruning: select moves having a win ratio > POOLRATIO
        auto fRatio = [ancestor, POOLRATIO] (int i) 
        {
            double raveWins = ancestor->_raveChildWins[i];
            double raveSims = ancestor->_raveChildSims[i];
            return raveWins/raveSims > POOLRATIO;
        };
        auto iter = std::partition(pprMoves.begin(), pprMoves.end(), fRatio);
        pprMoves.erase(iter, pprMoves.end());
    }

    // playout 
    BOARD_TYPE testBoard(ptrNode->_board);
    while (not testBoard.isGameFinished()) 
    {
        int indexToPlay = -1;
        int nbPprMoves = pprMoves.size();
        double rndRoulette = _prngPprRoulette.generate();
        double rndMove = _prngRaveSim.generate();
        // cas nominal = 52%
        // modif 1 = 57%
        // modif 2 = 77%
        // modif 1 + modif 2 = 75%

        if (rndRoulette < POOLPROBA and hasPprMoves and nbPprMoves != 0) 
        {
            // choose a PPR move
            int indexPprMoves = rndMove * nbPprMoves;  // cas nominal 
            //indexPprMoves = rndRoulette * nbPprMoves;  // modif 1
            indexToPlay = pprMoves[indexPprMoves];
            // remove the move in pprMoves
            pprMoves[indexPprMoves] = pprMoves[nbPprMoves-1];
            pprMoves.pop_back();
        } 
        else 
        {
            // choose a random move
            int nbMoves = testBoard.getNbEmptyIndices();
            //int indexMove = rndMove * nbMoves; // cas nominal 
            int indexMove = rndRoulette * nbMoves;  // modif 2
            indexToPlay = testBoard.findNthEmptyIndex(indexMove);
            // remove the move in pprMoves if present 
            auto it = std::find(pprMoves.begin(), pprMoves.end(), indexToPlay);
            if (it != pprMoves.end())
                pprMoves.erase(it);
        }

        // play the move
        testBoard.play(indexToPlay);
        _allMovesInSim.push_back(indexToPlay);
    }

    return testBoard.getWinnerCellt();
}

