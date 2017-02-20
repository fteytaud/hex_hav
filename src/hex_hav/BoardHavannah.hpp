
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

