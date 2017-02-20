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

#ifndef BOARD_HEX_TEST_HPP_
#define BOARD_HEX_TEST_HPP_

#include <hex_hav/BoardHex.hpp>
#include <cxxtest/TestSuite.h>
#include <algorithm>

#define CHECKPATHINFO(p, c, b1, b2, m) \
{ \
    const PathInfoHex & rp = p; \
    TS_ASSERT_EQUALS(rp._cellt, c); \
    TS_ASSERT_EQUALS(rp._isConnectedBorder1, b1); \
    TS_ASSERT_EQUALS(rp._isConnectedBorder2, b2); \
    TS_ASSERT_EQUALS(rp._mainPathIndex, m); \
}

class BoardHex_test : public CxxTest::TestSuite, public BoardHex 
{
    public:
        void test_reset_someAttributes() 
        {
            BoardHex_test b;
            b.reset(7);
            TS_ASSERT_EQUALS(b._size, 7);
            TS_ASSERT_EQUALS(b._nbIndices, 49);
            TS_ASSERT_EQUALS(b._currentCellt, CELL_WHITE);
            TS_ASSERT_EQUALS(b._winnerCellt, CELL_EMPTY);
            TS_ASSERT_EQUALS(b._lastIndex, -1);
            TS_ASSERT_EQUALS(b._nbEmptyIndices, 49);
        }

        void test_copyConstructor() 
        {
            BoardHex_test b0;
            b0.reset(7);
            BoardHex_test b(b0);
            TS_ASSERT_EQUALS(b._size, 7);
            TS_ASSERT_EQUALS(b._nbIndices, 49);
            TS_ASSERT_EQUALS(b._currentCellt, CELL_WHITE);
            TS_ASSERT_EQUALS(b._winnerCellt, CELL_EMPTY);
            TS_ASSERT_EQUALS(b._lastIndex, -1);
            TS_ASSERT_EQUALS(b._nbEmptyIndices, 49);
        }

        void test_reset_neighboursBoard_topLeft() 
        {
            BoardHex_test b;
            b.reset(7);
            int index = b.convertCellToIndex(Cell(0, 0));
            const std::array<int, 7> refNeighbourIndices = b._neighboursBoard[index];
            TS_ASSERT_EQUALS(refNeighbourIndices[0], b.convertCellToIndex(Cell(0,1)));
            TS_ASSERT_EQUALS(refNeighbourIndices[1], b.convertCellToIndex(Cell(1,0)));
            TS_ASSERT_EQUALS(refNeighbourIndices[2], -1);
        }

        void test_reset_neighboursBoard_topCenter() 
        {
            BoardHex_test b;
            b.reset(7);
            int index = b.convertCellToIndex(Cell(0, 3));
            const std::array<int, 7> refNeighbourIndices = b._neighboursBoard[index];
            TS_ASSERT_EQUALS(refNeighbourIndices[0], b.convertCellToIndex(Cell(0,2)));
            TS_ASSERT_EQUALS(refNeighbourIndices[1], b.convertCellToIndex(Cell(0,4)));
            TS_ASSERT_EQUALS(refNeighbourIndices[2], b.convertCellToIndex(Cell(1,2)));
            TS_ASSERT_EQUALS(refNeighbourIndices[3], b.convertCellToIndex(Cell(1,3)));
            TS_ASSERT_EQUALS(refNeighbourIndices[4], -1);
        }

        void test_reset_neighboursBoard_topRight() 
        {
            BoardHex_test b;
            b.reset(7);
            int index = b.convertCellToIndex(Cell(0, 6));
            const std::array<int, 7> refNeighbourIndices = b._neighboursBoard[index];
            TS_ASSERT_EQUALS(refNeighbourIndices[0], b.convertCellToIndex(Cell(0,5)));
            TS_ASSERT_EQUALS(refNeighbourIndices[1], b.convertCellToIndex(Cell(1,5)));
            TS_ASSERT_EQUALS(refNeighbourIndices[2], b.convertCellToIndex(Cell(1,6)));
            TS_ASSERT_EQUALS(refNeighbourIndices[3], -1);
        }

        void test_reset_neighboursBoard_middleRight() 
        {
            BoardHex_test b;
            b.reset(7);
            int index = b.convertCellToIndex(Cell(3, 6));
            const std::array<int, 7> refNeighbourIndices = b._neighboursBoard[index];
            TS_ASSERT_EQUALS(refNeighbourIndices[0], b.convertCellToIndex(Cell(2,6)));
            TS_ASSERT_EQUALS(refNeighbourIndices[1], b.convertCellToIndex(Cell(3,5)));
            TS_ASSERT_EQUALS(refNeighbourIndices[2], b.convertCellToIndex(Cell(4,5)));
            TS_ASSERT_EQUALS(refNeighbourIndices[3], b.convertCellToIndex(Cell(4,6)));
            TS_ASSERT_EQUALS(refNeighbourIndices[4], -1);
        }

        void test_reset_neighboursBoard_bottomRight() 
        {
            BoardHex_test b;
            b.reset(7);
            int index = b.convertCellToIndex(Cell(6, 6));
            const std::array<int, 7> refNeighbourIndices = b._neighboursBoard[index];
            TS_ASSERT_EQUALS(refNeighbourIndices[0], b.convertCellToIndex(Cell(5,6)));
            TS_ASSERT_EQUALS(refNeighbourIndices[1], b.convertCellToIndex(Cell(6,5)));
            TS_ASSERT_EQUALS(refNeighbourIndices[2], -1);
        }

        void test_reset_neighboursBoard_bottomCenter() 
        {
            BoardHex_test b;
            b.reset(7);
            int index = b.convertCellToIndex(Cell(6, 3));
            const std::array<int, 7> refNeighbourIndices = b._neighboursBoard[index];
            TS_ASSERT_EQUALS(refNeighbourIndices[0], b.convertCellToIndex(Cell(5,3)));
            TS_ASSERT_EQUALS(refNeighbourIndices[1], b.convertCellToIndex(Cell(5,4)));
            TS_ASSERT_EQUALS(refNeighbourIndices[2], b.convertCellToIndex(Cell(6,2)));
            TS_ASSERT_EQUALS(refNeighbourIndices[3], b.convertCellToIndex(Cell(6,4)));
            TS_ASSERT_EQUALS(refNeighbourIndices[4], -1);
        }

        void test_reset_neighboursBoard_bottomLeft() 
        {
            BoardHex_test b;
            b.reset(7);
            int index = b.convertCellToIndex(Cell(6, 0));
            const std::array<int, 7> refNeighbourIndices = b._neighboursBoard[index];
            TS_ASSERT_EQUALS(refNeighbourIndices[0], b.convertCellToIndex(Cell(5,0)));
            TS_ASSERT_EQUALS(refNeighbourIndices[1], b.convertCellToIndex(Cell(5,1)));
            TS_ASSERT_EQUALS(refNeighbourIndices[2], b.convertCellToIndex(Cell(6,1)));
            TS_ASSERT_EQUALS(refNeighbourIndices[3], -1);
        }

        void test_reset_neighboursBoard_middleLeft() 
        {
            BoardHex_test b;
            b.reset(7);
            int index = b.convertCellToIndex(Cell(3, 0));
            const std::array<int, 7> refNeighbourIndices = b._neighboursBoard[index];
            TS_ASSERT_EQUALS(refNeighbourIndices[0], b.convertCellToIndex(Cell(2,0)));
            TS_ASSERT_EQUALS(refNeighbourIndices[1], b.convertCellToIndex(Cell(2,1)));
            TS_ASSERT_EQUALS(refNeighbourIndices[2], b.convertCellToIndex(Cell(3,1)));
            TS_ASSERT_EQUALS(refNeighbourIndices[3], b.convertCellToIndex(Cell(4,0)));
            TS_ASSERT_EQUALS(refNeighbourIndices[4], -1);
        }

        void test_reset_neighboursBoard_middleCenter() 
        {
            BoardHex_test b;
            b.reset(7);
            int index = b.convertCellToIndex(Cell(3, 3));
            const std::array<int, 7> refNeighbourIndices = b._neighboursBoard[index];
            TS_ASSERT_EQUALS(refNeighbourIndices[0], b.convertCellToIndex(Cell(2,3)));
            TS_ASSERT_EQUALS(refNeighbourIndices[1], b.convertCellToIndex(Cell(2,4)));
            TS_ASSERT_EQUALS(refNeighbourIndices[2], b.convertCellToIndex(Cell(3,2)));
            TS_ASSERT_EQUALS(refNeighbourIndices[3], b.convertCellToIndex(Cell(3,4)));
            TS_ASSERT_EQUALS(refNeighbourIndices[4], b.convertCellToIndex(Cell(4,2)));
            TS_ASSERT_EQUALS(refNeighbourIndices[5], b.convertCellToIndex(Cell(4,3)));
            TS_ASSERT_EQUALS(refNeighbourIndices[6], -1);
        }

        void test_reset_paths() 
        {
            BoardHex_test b;
            b.reset(5);
            TS_ASSERT_EQUALS(b._pathsEnd, 1);
            const PathInfoHex & p0 = b._paths[0];
            TS_ASSERT_EQUALS(p0._cellt, CELL_EMPTY);
            TS_ASSERT_EQUALS(p0._isConnectedBorder1, false);
            TS_ASSERT_EQUALS(p0._isConnectedBorder2, false);
            TS_ASSERT_EQUALS(p0._mainPathIndex, 0);
        }

        void test_reset_pathBoard() 
        {
            BoardHex_test b;
            b.reset(5);
            for (int i=0; i<25; i++) 
            {
                TS_ASSERT_EQUALS(b._pathBoard[i], 0); 
                TS_ASSERT_EQUALS(b.getCelltAtIndex(i), CELL_EMPTY);
            }
        }

        void test_reset_someGetters() 
        {
            BoardHex_test b;
            b.reset(11);
            TS_ASSERT_EQUALS(b.getSize(), 11);
            TS_ASSERT_EQUALS(b.getNbIndices(), 121);
            TS_ASSERT_EQUALS(b.getNbEmptyIndices(), 121);
            TS_ASSERT_EQUALS(b.getLastIndex(), -1);
            TS_ASSERT_EQUALS(b.getCurrentCellt(), CELL_WHITE);
            TS_ASSERT_EQUALS(b.getWinnerCellt(), CELL_EMPTY);
        }


        void test_isValid() 
        {
            BoardHex_test b;
            b.reset(8);

            TS_ASSERT_EQUALS(b.isValidIndex(12), true);
            TS_ASSERT_EQUALS(b.isValidIndex(0), true);
            TS_ASSERT_EQUALS(b.isValidIndex(-1), false);
            TS_ASSERT_EQUALS(b.isValidIndex(63), true);
            TS_ASSERT_EQUALS(b.isValidIndex(64), false);

            TS_ASSERT_EQUALS(b.isValidCell(Cell(3, 4)), true);
            TS_ASSERT_EQUALS(b.isValidCell(Cell(0, 2)), true);
            TS_ASSERT_EQUALS(b.isValidCell(Cell(-1, 1)), false);
            TS_ASSERT_EQUALS(b.isValidCell(Cell(2, 0)), true);
            TS_ASSERT_EQUALS(b.isValidCell(Cell(2, -1)), false);
            TS_ASSERT_EQUALS(b.isValidCell(Cell(3, 7)), true);
            TS_ASSERT_EQUALS(b.isValidCell(Cell(3, 8)), false);
            TS_ASSERT_EQUALS(b.isValidCell(Cell(7, 2)), true);
            TS_ASSERT_EQUALS(b.isValidCell(Cell(8, 1)), false);
        }

        void test_convertIndexToCell() 
        {
            BoardHex_test b;
            b.reset(9);
            TS_ASSERT_EQUALS(b.convertIndexToCell(3), Cell(0, 3));
            TS_ASSERT_EQUALS(b.convertIndexToCell(13), Cell(1, 4));
            TS_ASSERT_EQUALS(b.convertIndexToCell(37), Cell(4, 1));
        }

        void test_convertCellToIndex() 
        {
            BoardHex_test b;
            b.reset(10);
            TS_ASSERT_EQUALS(b.convertCellToIndex(Cell(0, 2)), 2);
            TS_ASSERT_EQUALS(b.convertCellToIndex(Cell(1, 0)), 10);
            TS_ASSERT_EQUALS(b.convertCellToIndex(Cell(7, 5)), 75);
        }

        void test_convertCelltToString() 
        {
            BoardHex_test b;
            b.reset(8);
            TS_ASSERT_EQUALS(b.convertCelltToString(CELL_EMPTY), "none");
            TS_ASSERT_EQUALS(b.convertCelltToString(CELL_WHITE), "white");
            TS_ASSERT_EQUALS(b.convertCelltToString(CELL_BLACK), "black");
        }

        void test_computeBorderConnection_white() 
        {
            BoardHex_test b;
            b.reset(12);

            int index;
            bool isConnectedBorder1;
            bool isConnectedBorder2;

            // left
            index = b.convertCellToIndex(Cell(7, 0));
            b.computeBorderConnection(index, CELL_WHITE, 
                    isConnectedBorder1, isConnectedBorder2);
            TS_ASSERT_EQUALS(isConnectedBorder1, true);
            TS_ASSERT_EQUALS(isConnectedBorder2, false);

            // right
            index = b.convertCellToIndex(Cell(7, 11));
            b.computeBorderConnection(index, CELL_WHITE, 
                    isConnectedBorder1, isConnectedBorder2);
            TS_ASSERT_EQUALS(isConnectedBorder1, false);
            TS_ASSERT_EQUALS(isConnectedBorder2, true);

            // top
            index = b.convertCellToIndex(Cell(0, 7));
            b.computeBorderConnection(index, CELL_WHITE, 
                    isConnectedBorder1, isConnectedBorder2);
            TS_ASSERT_EQUALS(isConnectedBorder1, false);
            TS_ASSERT_EQUALS(isConnectedBorder2, false);

            // bottom
            index = b.convertCellToIndex(Cell(11, 7));
            b.computeBorderConnection(index, CELL_WHITE, 
                    isConnectedBorder1, isConnectedBorder2);
            TS_ASSERT_EQUALS(isConnectedBorder1, false);
            TS_ASSERT_EQUALS(isConnectedBorder2, false);
        }

        void test_computeBorderConnection_black() 
        {
            BoardHex_test b;
            b.reset(12);

            int index;
            bool isConnectedBorder1;
            bool isConnectedBorder2;

            // left
            index = b.convertCellToIndex(Cell(7, 0));
            b.computeBorderConnection(index, CELL_BLACK, 
                    isConnectedBorder1, isConnectedBorder2);
            TS_ASSERT_EQUALS(isConnectedBorder1, false);
            TS_ASSERT_EQUALS(isConnectedBorder2, false);

            // right
            index = b.convertCellToIndex(Cell(7, 11));
            b.computeBorderConnection(index, CELL_BLACK, 
                    isConnectedBorder1, isConnectedBorder2);
            TS_ASSERT_EQUALS(isConnectedBorder1, false);
            TS_ASSERT_EQUALS(isConnectedBorder2, false);

            // top
            index = b.convertCellToIndex(Cell(0, 7));
            b.computeBorderConnection(index, CELL_BLACK, 
                    isConnectedBorder1, isConnectedBorder2);
            TS_ASSERT_EQUALS(isConnectedBorder1, true);
            TS_ASSERT_EQUALS(isConnectedBorder2, false);

            // bottom
            index = b.convertCellToIndex(Cell(11, 7));
            b.computeBorderConnection(index, CELL_BLACK, 
                    isConnectedBorder1, isConnectedBorder2);
            TS_ASSERT_EQUALS(isConnectedBorder1, false);
            TS_ASSERT_EQUALS(isConnectedBorder2, true);
        }

        void test_computeBorderConnection_empty() 
        {
            BoardHex_test b;
            b.reset(12);

            int index;
            bool isConnectedBorder1;
            bool isConnectedBorder2;

            // left
            index = b.convertCellToIndex(Cell(7, 0));
            b.computeBorderConnection(index, CELL_EMPTY, 
                    isConnectedBorder1, isConnectedBorder2);
            TS_ASSERT_EQUALS(isConnectedBorder1, false);
            TS_ASSERT_EQUALS(isConnectedBorder2, false);

            // right
            index = b.convertCellToIndex(Cell(7, 11));
            b.computeBorderConnection(index, CELL_EMPTY, 
                    isConnectedBorder1, isConnectedBorder2);
            TS_ASSERT_EQUALS(isConnectedBorder1, false);
            TS_ASSERT_EQUALS(isConnectedBorder2, false);

            // top
            index = b.convertCellToIndex(Cell(0, 7));
            b.computeBorderConnection(index, CELL_EMPTY, 
                    isConnectedBorder1, isConnectedBorder2);
            TS_ASSERT_EQUALS(isConnectedBorder1, false);
            TS_ASSERT_EQUALS(isConnectedBorder2, false);

            // bottom
            index = b.convertCellToIndex(Cell(11, 7));
            b.computeBorderConnection(index, CELL_EMPTY, 
                    isConnectedBorder1, isConnectedBorder2);
            TS_ASSERT_EQUALS(isConnectedBorder1, false);
            TS_ASSERT_EQUALS(isConnectedBorder2, false);
        }

        void test_getPathIndexAndCelltAtIndex() 
        {
            BoardHex_test b;
            b.reset(7);
            b._pathsEnd = 3;
            b._paths[1] = PathInfoHex(1, CELL_WHITE, false, false);
            b._paths[2] = PathInfoHex(2, CELL_BLACK, false, false);

            int index_1_1 = b.convertCellToIndex(Cell(1, 1));
            int index_3_1 = b.convertCellToIndex(Cell(3, 1));
            int index_2_3 = b.convertCellToIndex(Cell(2, 3));

            b._pathBoard[index_3_1] = 1;
            b._pathBoard[index_2_3] = 2;

            int pathIndex;
            cell_t cellt;

            b.getPathIndexAndCelltAtIndex(index_1_1, pathIndex, cellt);
            TS_ASSERT_EQUALS(pathIndex, 0);
            TS_ASSERT_EQUALS(cellt, CELL_EMPTY);

            b.getPathIndexAndCelltAtIndex(index_3_1, pathIndex, cellt);
            TS_ASSERT_EQUALS(pathIndex, 1);
            TS_ASSERT_EQUALS(cellt, CELL_WHITE);

            b.getPathIndexAndCelltAtIndex(index_2_3, pathIndex, cellt);
            TS_ASSERT_EQUALS(pathIndex, 2);
            TS_ASSERT_EQUALS(cellt, CELL_BLACK);
        }

        void test_play0() 
        {
            BoardHex_test b;
            b.reset(5);
            int index;

            index = b.convertCellToIndex(Cell(0, 0));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 2);
            TS_ASSERT_EQUALS(b._pathBoard[index], 1);
            CHECKPATHINFO(b._paths[1], CELL_WHITE, true, false, 1);

            index = b.convertCellToIndex(Cell(2, 0));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 3);
            TS_ASSERT_EQUALS(b._pathBoard[index], 2);
            CHECKPATHINFO(b._paths[2], CELL_BLACK, false, false, 2);

            index = b.convertCellToIndex(Cell(0, 3));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 4);
            TS_ASSERT_EQUALS(b._pathBoard[index], 3);
            CHECKPATHINFO(b._paths[3], CELL_WHITE, false, false, 3);

            index = b.convertCellToIndex(Cell(1, 2));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 5);
            TS_ASSERT_EQUALS(b._pathBoard[index], 4);
            CHECKPATHINFO(b._paths[4], CELL_BLACK, false, false, 4);

            index = b.convertCellToIndex(Cell(1, 0));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 5);
            TS_ASSERT_EQUALS(b._pathBoard[index], 1);
            CHECKPATHINFO(b._paths[1], CELL_WHITE, true, false, 1);

            index = b.convertCellToIndex(Cell(2, 2));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 5);
            TS_ASSERT_EQUALS(b._pathBoard[index], 4);
            CHECKPATHINFO(b._paths[4], CELL_BLACK, false, false, 4);

            index = b.convertCellToIndex(Cell(4, 2));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 6);
            TS_ASSERT_EQUALS(b._pathBoard[index], 5);
            CHECKPATHINFO(b._paths[5], CELL_WHITE, false, false, 5);

            index = b.convertCellToIndex(Cell(2, 4));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 7);
            TS_ASSERT_EQUALS(b._pathBoard[index], 6);
            CHECKPATHINFO(b._paths[6], CELL_BLACK, false, false, 6);

            index = b.convertCellToIndex(Cell(0, 1));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 7);
            TS_ASSERT_EQUALS(b._pathBoard[index], 1);
            CHECKPATHINFO(b._paths[1], CELL_WHITE, true, false, 1);

            index = b.convertCellToIndex(Cell(0, 4));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 8);
            TS_ASSERT_EQUALS(b._pathBoard[index], 7);
            CHECKPATHINFO(b._paths[7], CELL_BLACK, true, false, 7);

            index = b.convertCellToIndex(Cell(3, 1));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 9);
            TS_ASSERT_EQUALS(b._pathBoard[index], 8);
            CHECKPATHINFO(b._paths[8], CELL_WHITE, false, false, 8);

            index = b.convertCellToIndex(Cell(0, 2));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 9);
            TS_ASSERT_EQUALS(b._pathBoard[index], 4);
            CHECKPATHINFO(b._paths[4], CELL_BLACK, true, false, 4);

            index = b.convertCellToIndex(Cell(2, 3));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 10);
            TS_ASSERT_EQUALS(b._pathBoard[index], 9);
            CHECKPATHINFO(b._paths[9], CELL_WHITE, false, false, 9);

            index = b.convertCellToIndex(Cell(4, 4));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 11);
            TS_ASSERT_EQUALS(b._pathBoard[index], 10);
            CHECKPATHINFO(b._paths[10], CELL_BLACK, false, true, 10);

            index = b.convertCellToIndex(Cell(3, 3));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 11);
            TS_ASSERT_EQUALS(b._pathBoard[index], 5);
            CHECKPATHINFO(b._paths[5], CELL_WHITE, false, false, 5);
            TS_ASSERT_EQUALS(b._paths[9]._mainPathIndex, 5);

            index = b.convertCellToIndex(Cell(4, 1));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 12);
            TS_ASSERT_EQUALS(b._pathBoard[index], 11);
            CHECKPATHINFO(b._paths[11], CELL_BLACK, false, true, 11);

            index = b.convertCellToIndex(Cell(3, 2));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 12);
            TS_ASSERT_EQUALS(b._pathBoard[index], 5);
            CHECKPATHINFO(b._paths[5], CELL_WHITE, false, false, 5);
            TS_ASSERT_EQUALS(b._paths[8]._mainPathIndex, 5);
            TS_ASSERT_EQUALS(b._paths[9]._mainPathIndex, 5);

            index = b.convertCellToIndex(Cell(4, 0));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 12);
            TS_ASSERT_EQUALS(b._pathBoard[index], 11);
            CHECKPATHINFO(b._paths[11], CELL_BLACK, false, true, 11);

            index = b.convertCellToIndex(Cell(1, 1));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 12);
            TS_ASSERT_EQUALS(b._pathBoard[index], 1);
            CHECKPATHINFO(b._paths[1], CELL_WHITE, true, false, 1);

            index = b.convertCellToIndex(Cell(4, 3));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 12);
            TS_ASSERT_EQUALS(b._pathBoard[index], 10);
            CHECKPATHINFO(b._paths[10], CELL_BLACK, false, true, 10);

            index = b.convertCellToIndex(Cell(3, 4));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 12);
            TS_ASSERT_EQUALS(b._pathBoard[index], 5);
            CHECKPATHINFO(b._paths[5], CELL_WHITE, false, true, 5);

            index = b.convertCellToIndex(Cell(3, 0));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 12);
            TS_ASSERT_EQUALS(b._pathBoard[index], 2);
            CHECKPATHINFO(b._paths[2], CELL_BLACK, false, true, 2);
            TS_ASSERT_EQUALS(b._paths[11]._mainPathIndex, 2);

            index = b.convertCellToIndex(Cell(1, 3));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 12);
            TS_ASSERT_EQUALS(b._pathBoard[index], 3);
            CHECKPATHINFO(b._paths[3], CELL_WHITE, false, true, 3);
            TS_ASSERT_EQUALS(b._paths[5]._mainPathIndex, 3);
            TS_ASSERT_EQUALS(b._paths[8]._mainPathIndex, 3);
            TS_ASSERT_EQUALS(b._paths[9]._mainPathIndex, 3);

            index = b.convertCellToIndex(Cell(1, 4));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 12);
            TS_ASSERT_EQUALS(b._pathBoard[index], 6);
            CHECKPATHINFO(b._paths[6], CELL_BLACK, true, false, 6);
            TS_ASSERT_EQUALS(b._paths[7]._mainPathIndex, 6);

            index = b.convertCellToIndex(Cell(2, 1));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 12);
            TS_ASSERT_EQUALS(b._pathBoard[index], 1);
            CHECKPATHINFO(b._paths[1], CELL_WHITE, true, true, 1);
            TS_ASSERT_EQUALS(b._paths[3]._mainPathIndex, 1);
            TS_ASSERT_EQUALS(b._paths[5]._mainPathIndex, 1);
            TS_ASSERT_EQUALS(b._paths[8]._mainPathIndex, 1);
            TS_ASSERT_EQUALS(b._paths[9]._mainPathIndex, 1);
            TS_ASSERT_EQUALS(b.getWinnerCellt(), CELL_WHITE);
        }

        void test_play1() 
        {
            BoardHex_test b;
            b.reset(5);
            int index;

            index = b.convertCellToIndex(Cell(2, 1));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 2);
            TS_ASSERT_EQUALS(b._pathBoard[index], 1);
            TS_ASSERT_EQUALS(b._paths[1]._cellt, CELL_WHITE);
            TS_ASSERT_EQUALS(b._paths[1]._isConnectedBorder1, false);
            TS_ASSERT_EQUALS(b._paths[1]._isConnectedBorder2, false);
            TS_ASSERT_EQUALS(b._paths[1]._mainPathIndex, 1);

            index = b.convertCellToIndex(Cell(3, 1));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 3);
            TS_ASSERT_EQUALS(b._pathBoard[index], 2);
            TS_ASSERT_EQUALS(b._paths[2]._cellt, CELL_BLACK);
            TS_ASSERT_EQUALS(b._paths[2]._isConnectedBorder1, false);
            TS_ASSERT_EQUALS(b._paths[2]._isConnectedBorder2, false);
            TS_ASSERT_EQUALS(b._paths[2]._mainPathIndex, 2);

            index = b.convertCellToIndex(Cell(2, 2));
            b.play(index);
            TS_ASSERT_EQUALS(b._pathsEnd, 3);
            TS_ASSERT_EQUALS(b._pathBoard[index], 1);
            TS_ASSERT_EQUALS(b._paths[1]._cellt, CELL_WHITE);
            TS_ASSERT_EQUALS(b._paths[1]._isConnectedBorder1, false);
            TS_ASSERT_EQUALS(b._paths[1]._isConnectedBorder2, false);
            TS_ASSERT_EQUALS(b._paths[1]._mainPathIndex, 1);
        }

        void test_play2() 
        {
            BoardHex_test b;
            b.reset(5);
            int index;

            index = b.convertCellToIndex(Cell(2, 1));
            b.play(index);
            index = b.convertCellToIndex(Cell(3, 1));
            b.play(index);
            index = b.convertCellToIndex(Cell(2, 2));
            b.play(index);

            TS_ASSERT_EQUALS(b._pathsEnd, 3);
            TS_ASSERT_EQUALS(b._pathBoard[index], 1);
            TS_ASSERT_EQUALS(b._paths[1]._cellt, CELL_WHITE);
            TS_ASSERT_EQUALS(b._paths[1]._isConnectedBorder1, false);
            TS_ASSERT_EQUALS(b._paths[1]._isConnectedBorder2, false);
            TS_ASSERT_EQUALS(b._paths[1]._mainPathIndex, 1);
        }

        void test_play3() 
        {
            BoardHex_test b;
            b.reset(5);
            int index;

            TS_ASSERT_EQUALS(b.getSize(), 5);
            TS_ASSERT_EQUALS(b.getNbIndices(), 25);

            TS_ASSERT_EQUALS(b.getNbEmptyIndices(), 25);
            TS_ASSERT_EQUALS(b.getLastIndex(), -1);
            TS_ASSERT_EQUALS(b.getCurrentCellt(), CELL_WHITE);
            TS_ASSERT_EQUALS(b.getWinnerCellt(), CELL_EMPTY);

            index = b.convertCellToIndex(Cell(2, 1));
            b.play(index);
            TS_ASSERT_EQUALS(b.getNbEmptyIndices(), 24);
            TS_ASSERT_EQUALS(b.getLastIndex(), index);
            TS_ASSERT_EQUALS(b.getCurrentCellt(), CELL_BLACK);
            TS_ASSERT_EQUALS(b.getWinnerCellt(), CELL_EMPTY);

            index = b.convertCellToIndex(Cell(3, 1));
            b.play(index);
            TS_ASSERT_EQUALS(b.getNbEmptyIndices(), 23);
            TS_ASSERT_EQUALS(b.getLastIndex(), index);
            TS_ASSERT_EQUALS(b.getCurrentCellt(), CELL_WHITE);
            TS_ASSERT_EQUALS(b.getWinnerCellt(), CELL_EMPTY);

            index = b.convertCellToIndex(Cell(2, 2));
            b.play(index);
            TS_ASSERT_EQUALS(b.getNbEmptyIndices(), 22);
            TS_ASSERT_EQUALS(b.getLastIndex(), index);
            TS_ASSERT_EQUALS(b.getCurrentCellt(), CELL_BLACK);
            TS_ASSERT_EQUALS(b.getWinnerCellt(), CELL_EMPTY);
        }

        void test_findWinnerPath() 
        {
            BoardHex_test b;
            b.reset(5);

            // play cells
            std::vector<Cell> gameCells = 
            {
                Cell(1, 1), Cell(2, 0), Cell(2, 3), Cell(3, 2), Cell(4, 2), 
                Cell(1, 2), Cell(0, 4), Cell(3, 0), Cell(1, 0), Cell(3, 3), 
                Cell(1, 3), Cell(0, 0), Cell(2, 1), Cell(1, 4), Cell(2, 2)
            };
            for (const Cell & c : gameCells) 
            {
                int index = b.convertCellToIndex(c);
                b.play(index);
            }
            TS_ASSERT_EQUALS(b.getWinnerCellt(), CELL_WHITE);

            // test cells in winner path
            std::vector<int> winIndices = b.findWinnerPath();
            TS_ASSERT_EQUALS(int(winIndices.size()), 7);
            std::vector<Cell> winCells { Cell(1, 1), Cell(2, 3), Cell(0, 4), 
                Cell(1, 0), Cell(1, 3), Cell(2, 1), Cell(2, 2) };
            for (const Cell & c : winCells)
            {
                int index = b.convertCellToIndex(c);
                TS_ASSERT_DIFFERS(winIndices.end(), std::find(winIndices.begin(), 
                            winIndices.end(), index));
            }
        }

        void test_findEmptyIndices() 
        {
            BoardHex_test b;
            b.reset(5);

            // play cells
            std::vector<Cell> gameCells = 
            {
                Cell(1, 1), Cell(2, 0), Cell(2, 3), Cell(3, 2), Cell(4, 2), 
                Cell(1, 2), Cell(0, 4), Cell(3, 0), Cell(1, 0), Cell(3, 3), 
                Cell(1, 3), Cell(0, 0), Cell(2, 1), Cell(1, 4), Cell(2, 2)
            };
            for (const Cell & c : gameCells) 
            {
                int index = b.convertCellToIndex(c);
                b.play(index);
            }

            // test cells in winner path
            std::vector<Cell> expectedEmptyCells = 
            { 
                Cell(0, 1), Cell(0, 2), Cell(0, 3), Cell(2, 4), Cell(3, 1), 
                Cell(3, 4), Cell(4, 0), Cell(4, 1), Cell(4, 3), Cell(4, 4) 
            };

            std::vector<int> emptyIndices = b.findEmptyIndices();
            TS_ASSERT_EQUALS(emptyIndices.size(), expectedEmptyCells.size());
            TS_ASSERT_EQUALS(unsigned(b.getNbEmptyIndices()), 
                    expectedEmptyCells.size());

            for (unsigned i=0; i<emptyIndices.size(); i++) 
            {
                int index = b.convertCellToIndex(expectedEmptyCells[i]);
                TS_ASSERT_EQUALS(index, emptyIndices[i]);
            }
        }

        void test_findNthEmptyIndex() 
        {
            BoardHex_test b;
            b.reset(5);

            // play cells
            std::vector<Cell> gameCells = 
            {
                Cell(1, 1), Cell(2, 0), Cell(2, 3), Cell(3, 2), Cell(4, 2), 
                Cell(1, 2), Cell(0, 4), Cell(3, 0), Cell(1, 0), Cell(3, 3), 
                Cell(1, 3), Cell(0, 0), Cell(2, 1), Cell(1, 4), Cell(2, 2)
            };
            for (const Cell & c : gameCells) 
            {
                int index = b.convertCellToIndex(c);
                b.play(index);
            }

            // test cells in winner path
            std::vector<Cell> expectedEmptyCells = 
            { 
                Cell(0, 1), Cell(0, 2), Cell(0, 3), Cell(2, 4), Cell(3, 1), 
                Cell(3, 4), Cell(4, 0), Cell(4, 1), Cell(4, 3), Cell(4, 4) 
            };

            TS_ASSERT_EQUALS(unsigned(b.getNbEmptyIndices()), 
                    expectedEmptyCells.size());

            for (unsigned i=0; i<expectedEmptyCells.size(); i++) 
            {
                int indexExpected = b.convertCellToIndex(expectedEmptyCells[i]);
                int indexFound = b.findNthEmptyIndex(i);
                TS_ASSERT_EQUALS(indexFound, indexExpected);
            }
        }
};

#endif  // BOARD_HEX_TEST_HPP_

