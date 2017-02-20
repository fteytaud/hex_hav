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

#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <array>
#include <cassert>
#include <string>
#include <vector>

const int MAX_BOARDSIZE = 20;

#ifndef BOARD_TYPE
#error Please define BOARD_TYPE (BoardHex or BoardHavannah)
#endif  // BOARD_TYPE

enum cell_t { CELL_WHITE, CELL_BLACK, CELL_EMPTY };

struct Cell
{
    int _i;
    int _j;
    Cell();
    Cell(int i, int j);
    bool operator==(const Cell & refCell) const;
};

struct PathInfo 
{
    // minimal index, in a path array, to a connected path
    // can index itself
    // warning: tricky crap
    int _mainPathIndex;  

    cell_t _cellt;

    PathInfo() = default;
    PathInfo(int index, cell_t cellt);
};

//////////////////////////////////////////////////////////////////////
// template definition
//////////////////////////////////////////////////////////////////////

template <typename T>
class Board 
{
    // TODO refactor havannah stats
    public:
        int _stats;

    protected:
        int _size;
        int _nbFullIndices;
        int _nbIndices;

        cell_t _currentCellt;
        cell_t _winnerCellt;

        int _lastIndex;
        int _nbEmptyIndices;

        // neighbours of each cell (indices)
        // end value: -1
        std::array<std::array<int,7>,MAX_BOARDSIZE*MAX_BOARDSIZE> 
            _neighboursBoard;

        // PathInfo of the paths indexed from _pathBoard
        int _pathsEnd;
        std::array<T, MAX_BOARDSIZE*MAX_BOARDSIZE> _paths;

        // path of each cell (index in _paths) 
        std::array<int, MAX_BOARDSIZE*MAX_BOARDSIZE> _pathBoard;

    public:
        // TODO reimplementer avec methode non virtuelle + attribut 
        virtual const std::string getBoardType() const = 0;

        int getSize() const;
        int getNbFullIndices() const;   // nb indices of the full board
        int getNbIndices() const;       // nb indices of the playable board
        int getNbEmptyIndices() const;

        cell_t getCurrentCellt() const;
        cell_t getWinnerCellt() const;
        bool isGameFinished() const;

        int getLastIndex() const;
        cell_t getCelltAtIndex(int index) const;
        cell_t getCelltAtCell(const Cell & cell) const;

        Cell convertIndexToCell(int index) const;
        int convertCellToIndex(const Cell & refCell) const;
        static std::string convertCelltToString(cell_t cellt);

#ifdef LOG
        void printLogWithData(const std::vector<double>& dataToPrint) const;
#endif  // LOG

        // define abstract methods
        virtual void reset(int size) = 0;
        virtual void play(int index) = 0;

        virtual bool isValidCell(const Cell & refCell) const = 0;
        virtual bool isValidIndex(int index) const = 0;

        virtual int findNthEmptyIndex(int n) const = 0;
        virtual std::vector<int> findEmptyIndices() const = 0;
        virtual std::vector<int> findWinnerPath() const = 0; 

        virtual void guiCells(std::vector<Cell> & whiteCells, 
                std::vector<Cell> & blackCells, 
                std::vector<Cell> & emptyCells) const = 0;

    protected:
        void getPathIndexAndCelltAtIndex(int index, int & pathIndex, 
                cell_t & cellt) const;
};

//////////////////////////////////////////////////////////////////////
// template implementation
//////////////////////////////////////////////////////////////////////

template <typename T>
int Board<T>::getSize() const 
{
    return _size;
}

template <typename T>
int Board<T>::getNbFullIndices() const 
{
    return _nbFullIndices;
}

template <typename T>
int Board<T>::getNbIndices() const 
{
    return _nbIndices;
}

template <typename T>
int Board<T>::getNbEmptyIndices() const 
{
    return _nbEmptyIndices;
}

template <typename T>
cell_t Board<T>::getCurrentCellt() const 
{
    return _currentCellt;
}

template <typename T>
cell_t Board<T>::getWinnerCellt() const 
{
    return _winnerCellt;
}

template <typename T>
bool Board<T>::isGameFinished() const 
{
    return _nbEmptyIndices == 0 or _winnerCellt != CELL_EMPTY;
}

template <typename T>
int Board<T>::getLastIndex() const 
{
    return _lastIndex;
}

template <typename T>
cell_t Board<T>::getCelltAtIndex(int index) const 
{
    int pathIndex;
    cell_t cellt;
    getPathIndexAndCelltAtIndex(index, pathIndex, cellt);
    return cellt;
}

template <typename T>
cell_t Board<T>::getCelltAtCell(const Cell & cell) const 
{
    return getCelltAtIndex(convertCellToIndex(cell));
}


template <typename T>
Cell Board<T>::convertIndexToCell(int index) const 
{
    int i = index / _size;
    int j = index % _size;
    return Cell(i, j);
}

template <typename T>
int Board<T>::convertCellToIndex(const Cell & refCell) const 
{
    return refCell._i * _size + refCell._j;
}

    template <typename T>
std::string Board<T>::convertCelltToString(cell_t cellt) 
{
    if (cellt == CELL_WHITE) 
        return "white";
    if (cellt == CELL_BLACK) 
        return "black";
    return "none";
}

#ifdef LOG
#include <iostream>
template <typename T>
void Board<T>::printLogWithData(const std::vector<double>& dataToPrint) const 
{
    std::clog << "\nboard = \n"; 
    for (int i=0; i<_size; i++) 
    {
        for (int k=0; k<i; k++) 
            std::clog << "   ";
        for (int j=0; j<_size; j++) 
            std::clog << "   " << dataToPrint[i*_size+j];
        std::clog << "\n\n";
    }
}
#endif  // LOG

template <typename T>
void Board<T>::getPathIndexAndCelltAtIndex(int index, int & pathIndex, 
        cell_t & cellt) const 
{
    assert(index >= 0);
    assert(index < _nbFullIndices);

    // get path index from board
    pathIndex = _pathBoard[index];

    assert(pathIndex >= 0);
    assert(pathIndex < _pathsEnd);

    // get cellt from paths
    cellt = _paths[pathIndex]._cellt;
}

#endif  // BOARD_HPP_

