
#include <hex_hav/Player.hpp>

#include <cassert>
#include <iostream>

////////////////////////////////////////////////////////////////////////
// Player
////////////////////////////////////////////////////////////////////////

Player::Player() 
{
    resetTurn();
}

bool Player::hasIndexToPlay() const 
{
    return _hasIndexToPlay;
}

int Player::getIndexToPlay() const 
{
    return _indexToPlay;
}

void Player::resetTurn() 
{
    _hasIndexToPlay = false;
}

void Player::setIndexToPlay(int index) 
{
    _indexToPlay = index;
    _hasIndexToPlay = true;
}

////////////////////////////////////////////////////////////////////////
// PlayerHuman
////////////////////////////////////////////////////////////////////////

void PlayerHuman::newTurn(const BOARD_TYPE & ) 
{
    resetTurn();
}

void PlayerHuman::clickIndex(int index) 
{
    setIndexToPlay(index);
}

////////////////////////////////////////////////////////////////////////
// PlayerAi
////////////////////////////////////////////////////////////////////////

void PlayerAi::clickIndex(int ) 
{}

////////////////////////////////////////////////////////////////////////
// PlayerRandom
////////////////////////////////////////////////////////////////////////

PlayerRandom::PlayerRandom() 
{
    _parameters["fake"] = 0;
#ifdef PRNG_SEED
    _prng.setSeed(PRNG_SEED);
#endif  // PRNG_SEED
}

void PlayerRandom::newTurn(const BOARD_TYPE & refBoard) 
{
    int nbEmptyIndices = refBoard.getNbEmptyIndices();
    int n = _prng.generate() * nbEmptyIndices;
    int index = refBoard.findNthEmptyIndex(n);
    setIndexToPlay(index);
}

////////////////////////////////////////////////////////////////////////
// PlayerMc
////////////////////////////////////////////////////////////////////////

PlayerMc::PlayerMc() 
{
    _parameters["nbSimulations"] = 10000;
#ifdef PRNG_SEED
    setSeed(PRNG_SEED);
#endif  // PRNG_SEED
}

void PlayerMc::newTurn(const BOARD_TYPE & refBoard) 
{
    cell_t currentCellt = refBoard.getCurrentCellt();
    unsigned nbSimulations = _parameters["nbSimulations"];
    unsigned nbEmptyIndices = refBoard.getNbEmptyIndices();
    assert(nbSimulations > 0);
    assert(nbEmptyIndices > 0);

    // vector containing number of wins for each index
    std::vector<int> nbWins(nbEmptyIndices, 0);
    std::vector<int> emptyIndices = refBoard.findEmptyIndices();

    // simulate all indices, in parallel
    for (unsigned iIndex=0; iIndex<nbEmptyIndices; iIndex++) 
    {
        for (unsigned iSimulation=0; iSimulation<nbSimulations; iSimulation++) 
        {
            cell_t simuWinnerCellt = 
                playRandomGameWithFirstIndex(refBoard, emptyIndices[iIndex]);
            if (simuWinnerCellt == currentCellt)
                nbWins[iIndex]++;
        }
    }

    // find best cell
    int bestNbWins = -1;
    int iBestIndex = -1;
    for (unsigned i=0; i<nbEmptyIndices; i++) 
    {
        if (nbWins[i] > bestNbWins) 
        {
            bestNbWins = nbWins[i];
            iBestIndex = i;
        }
    }
    setIndexToPlay(emptyIndices[iBestIndex]);

#ifdef LOG
    std::clog << BOARD_TYPE::convertCelltToString(currentCellt)
        << "\t(nbSimulations = " << nbSimulations 
        << ")\tbestWins = " << bestNbWins << std::endl;
#endif  // LOG

}

cell_t PlayerMc::playRandomGameWithFirstIndex(
        const BOARD_TYPE & refBoard, int firstIndex) 
{
    BOARD_TYPE testBoard(refBoard);
    testBoard.play(firstIndex);
    while (not testBoard.isGameFinished()) 
    {
        int nbEmptyIndices = testBoard.getNbEmptyIndices();
        int n = _prng.generate() * nbEmptyIndices;
        int indexToPlay = testBoard.findNthEmptyIndex(n);
        testBoard.play(indexToPlay);
    }
    return testBoard.getWinnerCellt();
}

