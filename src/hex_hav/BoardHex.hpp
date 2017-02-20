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

#ifndef BOARD_HEX_HPP_
#define BOARD_HEX_HPP_

#include <hex_hav/Board.hpp>

struct PathInfoHex : public PathInfo 
{
    bool _isConnectedBorder1;
    bool _isConnectedBorder2;
    PathInfoHex();
    PathInfoHex(int index, cell_t cellt, bool border1, bool border2);
};

class BoardHex : public Board<PathInfoHex> 
{
    protected:
        void computeBorderConnection(int index, cell_t cellt, 
                bool & isConnectedBorder1, bool & isConnectedBorder2) const;

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
#endif // LOG
};

#endif // BOARD_HEX_HPP_

