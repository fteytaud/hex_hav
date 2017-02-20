
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

