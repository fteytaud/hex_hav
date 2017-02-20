
#include <hex_hav/PlayerRavePr.hpp>

#include<algorithm>

PlayerRavePr::PlayerRavePr() 
{
    _parameters["POOLSIM"] = 50;
    _parameters["POOLSIZE"] = 20;
    _parameters["POOLPROBA"] = 0.3;
    _allMovesInSim.reserve(MAX_BOARDSIZE * MAX_BOARDSIZE);
}

cell_t PlayerRavePr::computeSimulation(NodeRave *ptrNode) 
{
    double SIZE = _parameters.at("POOLSIZE");
    double NBSIMS  = _parameters.at("POOLSIM");
    double PROBA = _parameters.at("POOLPROBA");

    BOARD_TYPE testBoard(ptrNode->_board);
    std::vector<int> prMoves = testBoard.findEmptyIndices();

    NodeRave *ancestor = ptrNode;
    while (ancestor) 
    {
        if (ancestor->_nbSims > 50)
            break;
        ancestor = (NodeRave*)ancestor->_ptrParentNode;
    }
    bool playAPoolRaveMove = ancestor != nullptr;

    if (playAPoolRaveMove) 
    {
        auto f1 = [ancestor, NBSIMS](int i) 
        { return ancestor->_raveChildSims[i] >= NBSIMS; };
        auto iter = std::partition(prMoves.begin(), prMoves.end(), f1);
        prMoves.erase(iter, prMoves.end());

        auto f2 = [ancestor](int i, int j) 
        {
            double winsI = ancestor->_raveChildWins[i];
            double simsI = ancestor->_raveChildSims[i];
            double winsJ = ancestor->_raveChildWins[j];
            double simsJ = ancestor->_raveChildSims[j];
            return winsI/simsI > winsJ/simsJ;
        };
        std::sort(prMoves.begin(), prMoves.end(), f2); 

        if (prMoves.size() > SIZE)
            prMoves.resize(SIZE);
    }

    while (not testBoard.isGameFinished()) 
    {
        int indexToPlay = -1;

        double rndRoulette = _prngPrRoulette.generate();
        double rndMove = _prngRaveSim.generate();
        if (rndRoulette < PROBA and prMoves.size() and playAPoolRaveMove) 
        {
            int iPool = prMoves.size() * rndMove;
            indexToPlay = prMoves[iPool];
            prMoves[iPool] = prMoves[prMoves.size()-1];
            prMoves.pop_back();
        } 
        else 
        {
            int n = testBoard.getNbEmptyIndices() * rndMove;
            indexToPlay = testBoard.findNthEmptyIndex(n);
            auto iter = std::find(prMoves.begin(), prMoves.end(), indexToPlay);
            if (iter != prMoves.end())
                prMoves.erase(iter);
        }

        testBoard.play(indexToPlay);
        _allMovesInSim.push_back(indexToPlay);
    }

    return testBoard.getWinnerCellt();
}

