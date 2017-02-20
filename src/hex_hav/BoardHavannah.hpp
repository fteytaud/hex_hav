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

#ifndef BOARD_HAVANNAH_HPP_
#define BOARD_HAVANNAH_HPP_

#include <hex_hav/Board.hpp>

struct PathInfoHavannah : public PathInfo 
{
    unsigned _borders;
    unsigned _corners;
    PathInfoHavannah();
    PathInfoHavannah(int index, cell_t cellt, unsigned borders, 
            unsigned corners);
};

class BoardHavannah : public Board<PathInfoHavannah> 
{
    private:
        int _edgeSize;
        int _winningCycle;

    public:
        enum HAV_STATS {
            HAV_NONE, HAV_WHITE_CYCLE, HAV_WHITE_BRIDGE, HAV_WHITE_FORK, 
            HAV_BLACK_CYCLE, HAV_BLACK_BRIDGE, HAV_BLACK_FORK
        };

    public:
        // implement abstract methods
        const std::string getBoardType() const override;
        void reset(int size);
        void play(int index);

        bool isValidCell(const Cell & refCell) const;
        bool isValidIndex(int index) const;

        int findNthEmptyIndex(int n) const;
        std::vector<int> findEmptyIndices() const;
        std::vector<int> findWinnerPath() const; 

        void guiCells(std::vector<Cell> & whiteCells, 
                std::vector<Cell> & blackCells, 
                std::vector<Cell> & emptyCells) const;

#ifdef LOG
        void printLog() const;
#endif  // LOG

    protected:
        bool isWinningPath(const PathInfoHavannah & path, int pathIndex, 
                int cellIndex);
        // isCycle: assume _paths[cellIndex] == pathIndex
        bool isCycle(int pathIndex, int cellIndex) const; 
        unsigned computeBorders(int index) const;
        unsigned computeCorners(int index) const;
        int computeNbOnes(unsigned f) const;
        std::vector<int> findPathIndices(int pathIndex) const;
        int computeNbNeighbours(int cellIndex, cell_t cellt) const;
        bool detectHole(const std::vector<int> & indices) const;
};

#endif // BOARD_HAVANNAH_HPP_

