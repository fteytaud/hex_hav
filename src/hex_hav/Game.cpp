
#include <hex_hav/Game.hpp>
#include <hex_hav/Player.hpp>

#include <cassert>
#include <iostream>

Game::Game() : _ptrWhitePlayer(nullptr), _ptrBlackPlayer(nullptr) 
{
    // TODO refactor havannah stats
    _stats.fill(0);
}

void Game::reset(int size, Player * ptrWhitePlayer, Player * ptrBlackPlayer) 
{
    std::clog << std::endl;
    std::clog << "new game\n";

    _ptrWhitePlayer = ptrWhitePlayer;
    _ptrBlackPlayer = ptrBlackPlayer;

    _board.reset(size);

    _chrono.start();

    Player * ptrPlayer = getPtrCurrentPlayer();
    ptrPlayer->newTurn(_board);
}

void Game::clickIndex(int index) 
{
    if (_board.isValidIndex(index)) 
    {
        Player * ptrCurrentPlayer = getPtrCurrentPlayer();
        if (ptrCurrentPlayer)
            ptrCurrentPlayer->clickIndex(index);
    }
}

bool Game::hasTurnToPlay() const 
{
    if (_board.getWinnerCellt() != CELL_EMPTY) 
        return false;
    Player * ptrCurrentPlayer = getPtrCurrentPlayer();
    return ptrCurrentPlayer and ptrCurrentPlayer->hasIndexToPlay();
}

void Game::playTurn() 
{
    assert(hasTurnToPlay());

    // play current turn
    Player * ptrCurrentPlayer = getPtrCurrentPlayer();
    int indexToPlay = ptrCurrentPlayer->getIndexToPlay();
    assert(_board.isValidIndex(indexToPlay));
    _board.play(indexToPlay);

    // TODO refactor havannah stats
    if (_board.isGameFinished()) 
    {
        assert(0 <= _board._stats and _board._stats < 7);
        _stats[_board._stats]++;
    }

    // display turn time
    _chrono.stop();
    cell_t currentCellt = _board.getCurrentCellt();
    std::string currentCelltString = 
        Board<BOARD_TYPE>::convertCelltToString(currentCellt);
    std::clog << currentCelltString << " played in " << _chrono.elapsedStopped() 
        << " s (cell " << indexToPlay << ")" << std::endl;

    // start new turn
    if (not _board.isGameFinished()) 
    {
        //if (_board.getWinnerCellt() == CELL_EMPTY) 
        ptrCurrentPlayer->resetTurn();
        _chrono.start();
        Player * ptrNewPlayer = getPtrCurrentPlayer();
        ptrNewPlayer->newTurn(_board);
    }
}

const std::string Game::getBoardType() const 
{
    return _board.getBoardType();
}

int Game::getSize() const 
{
    return _board.getSize();
}

int Game::getLastCellIndex() const 
{
    return _board.getLastIndex();
}

cell_t Game::getWinnerCellt() const 
{
    return _board.getWinnerCellt();
}

cell_t Game::getCurrentCellt() const 
{
    return _board.getCurrentCellt();
}

const BOARD_TYPE & Game::getRefBoard() const 
{
    return _board;
}

Player * Game::getPtrCurrentPlayer() const 
{
    cell_t currentCellt = _board.getCurrentCellt();
    if (currentCellt == CELL_WHITE) return _ptrWhitePlayer;
    if (currentCellt == CELL_BLACK) return _ptrBlackPlayer;
    return 0; 
}

