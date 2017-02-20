
#include <hex_hav/PlayerRaveNast2.hpp>

#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
// Record
///////////////////////////////////////////////////////////////////////////////

void PlayerRaveNast2::Record::updateCdf(double & cdf) 
{
    double p = _sims==0 ? 1.0 : exp(_wins/double(_sims));
    cdf += p;
    _cdf = cdf;
}

///////////////////////////////////////////////////////////////////////////////
// PlayerRaveNast2
///////////////////////////////////////////////////////////////////////////////

void PlayerRaveNast2::newTurn(const BOARD_TYPE & refBoard) 
{
    PlayerRave::newTurn(refBoard);
    _nastRecords.clear();
}

cell_t PlayerRaveNast2::computeSimulation(NodeRave * ptrNode) 
{
    BOARD_TYPE testBoard(ptrNode->_board);
    while (not testBoard.isGameFinished()) 
    {

        // bof (52%)
        // considère tous les coups possibles (même si a1/a2 pas dans nast)
        // compute CDF
        std::vector<int> moveIndices = testBoard.findEmptyIndices();
        std::vector<double> cdf(moveIndices.size());
        int action1 = testBoard.getLastIndex();
        double sumProba = 0.0;
        for (unsigned i=0; i<moveIndices.size(); i++)
        {
            double gibbs = 1.0;
            int action2 = moveIndices[i];
            int k = action1*testBoard.getNbFullIndices() + action2;
            auto iter = _nastRecords.find(k);
            if (iter != _nastRecords.end())
            {
                double wins = iter->second._wins;
                double sims = iter->second._sims;
                gibbs = exp(wins/sims);
            }
            sumProba += gibbs;
            cdf[i] = sumProba;
        }
        // choose one action
        double rndCdf = sumProba * _prngNast2Cdf.generate();
        int indexToPlay = moveIndices.front();
        for (unsigned i=1; i<moveIndices.size(); i++)
            if (cdf[i] >= rndCdf)
                break;
            else
                indexToPlay = moveIndices[i];

        /*
        // version initiale qui marche très bof (49%)
        // considère tous les coups nast
        // compute all CDF
        double sumProba = 0.0;
        for (auto & record : _nastRecords) 
            record.second.updateCdf(sumProba);
        // choose one action
        double rndCdf = sumProba * _prngNast2Cdf.generate();
        int indexToPlay = -1;
        for (auto & record : _nastRecords) 
            if (record.second._cdf >= rndCdf)
                break;
            else
                indexToPlay = record.second._action2;
        */

        /*
        // version qui devrait correspondre au papier mais qui marche pas (46%)
        // pour chaque coup possible, considère les coups nast dont le action2 correspond
        // compute CDF
        std::vector<int> moveIndices = testBoard.findEmptyIndices();
        std::vector<double> cdf(moveIndices.size());
        double sumProba = 0.0;
        for (unsigned i=0; i<moveIndices.size(); i++)
        {
            double sims = 0.0;
            double wins = 0.0;
            int action2 = moveIndices[i];
            for (const auto & p : _nastRecords)
            {
                const auto & r = p.second;
                if (r._action2 == action2)
                {
                    sims += r._sims;
                    wins += r._wins;
                }
            }
            double gibbs = sims==0 ? 1.0 : exp(wins/sims);
            sumProba += gibbs;
            cdf[i] = sumProba;
        }
        // choose one action
        double rndCdf = sumProba * _prngNast2Cdf.generate();
        int indexToPlay = moveIndices.front();
        for (unsigned i=1; i<moveIndices.size(); i++)
            if (cdf[i] >= rndCdf)
                break;
            else
                indexToPlay = moveIndices[i];
        */

        // if the move invalid 
        if (not testBoard.isValidIndex(indexToPlay)
                or testBoard.getCelltAtIndex(indexToPlay) != CELL_EMPTY) 
        {
            // compute a random move
            double rndMove = _prngRaveSim.generate();
            int indexMove = testBoard.getNbEmptyIndices() * rndMove;
            indexToPlay = testBoard.findNthEmptyIndex(indexMove);
        }

        // store and play the move
        _allMovesInSim.push_back(indexToPlay);
        testBoard.play(indexToPlay);
    }

    return testBoard.getWinnerCellt();
}

void PlayerRaveNast2::computeBackpropagation(NodeRave *ptrNode, cell_t winCellt) 
{
    // update Rave and MCTS
    PlayerRave::computeBackpropagation(ptrNode, winCellt);

    // update Nast
    while (ptrNode) 
    {
        NodeRave * ptrParentNode = (NodeRave*) ptrNode->_ptrParentNode;

        int action1 = -1;
        if (ptrParentNode 
                and ptrParentNode->_board.getCurrentCellt() == _playerCellt)
            action1 = ptrParentNode->_index;

        int action2 = -1;
        if (ptrNode->_board.getCurrentCellt() != _playerCellt)
            action2 = ptrNode->_index;

        if (action1 != -1 and action2 != -1) 
        {
            // update record corresponding to (action1,action2)
            int k = action1*ptrParentNode->_board.getNbFullIndices() + action2;
            auto mapElt = std::make_pair(k,Record{action1,action2,0,0,0.0});
            auto mapPair = _nastRecords.insert(mapElt);
            Record & record = mapPair.first->second;
            record._sims++;
            if (winCellt == _playerCellt) 
                record._wins++;
        }

        ptrNode = ptrParentNode;
    }
}

#ifdef LOG
void PlayerRaveNast2::printLog() {
    const NodeRave &root = (_nodes.front());
    std::vector<double> superTest(root._raveChildWins.size());
    for (unsigned t = 0; t < superTest.size(); t++) 
        superTest[t] = root._raveChildWins[t]/(double)root._raveChildSims[t];
    root._board.printLogWithData(superTest);
}
#endif  // LOG

