
#include <hex_hav/BoardHex.hpp>

#include <cassert>
#include <iostream>
#include <set>

PathInfoHex::PathInfoHex() 
{}

PathInfoHex::PathInfoHex(int index, cell_t cellt, bool border1, bool border2) :
    PathInfo(index, cellt), _isConnectedBorder1(border1), 
    _isConnectedBorder2(border2) 
{}

const std::string BoardHex::getBoardType() const 
{
    return "BoardHex";
}

void BoardHex::reset(int size) 
{
    assert(size >= 2);
    assert(size <= MAX_BOARDSIZE);

    _stats = 0;
    _size = size;
    _nbFullIndices = size * size;
    _nbIndices = _nbFullIndices;
    _nbEmptyIndices = _nbIndices;

    _currentCellt = CELL_WHITE;
    _winnerCellt = CELL_EMPTY;

    _lastIndex = -1;

    // _neighboursBoard
    // precompute all valid neighbours of each cell
    for (int i=0; i<_size; i++) 
    {
        for (int j=0; j<_size; j++) 
        {
            int index = convertCellToIndex(Cell(i, j));
            int k=0;
            std::array<Cell, 6> neighbours = 
            {{
                 Cell(i-1, j  ),
                 Cell(i-1, j+1),
                 Cell(i  , j-1),
                 Cell(i  , j+1),
                 Cell(i+1, j-1),
                 Cell(i+1, j  )
             }};
            for (const Cell & refCell : neighbours) 
            {
                if (isValidCell(refCell)) 
                {
                    _neighboursBoard[index][k] = convertCellToIndex(refCell);
                    k++;
                }
            }
            _neighboursBoard[index][k] = -1;
        }
    }

    // _paths
    // no initial path
    // path 0 for empty cells
    _paths[0] = PathInfoHex(0, CELL_EMPTY, false, false);
    _pathsEnd = 1;

    // _pathBoard
    // set all cells to 0 (empty path)
    _pathBoard.fill(0);
}

void BoardHex::play(int index) 
{
    assert(isValidIndex(index));
    assert(_pathBoard[index] == 0);

    if (_winnerCellt == CELL_EMPTY) 
    {

        // find previous path & cell at index
        int boardPathIndex;
        cell_t boardCellt;
        getPathIndexAndCelltAtIndex(index, boardPathIndex, boardCellt);

        // if board cell is empty, update board
        if (boardCellt == CELL_EMPTY) 
        {

            // cell data
            int mainPathIndex = _pathsEnd;
            bool isConnectedBorder1, isConnectedBorder2;
            computeBorderConnection(index, _currentCellt, isConnectedBorder1, 
                    isConnectedBorder2);

            // find all connected paths
            std::set<int> neighbourMainPathIndices;
            for (int neighbourIndex : _neighboursBoard[index]) 
            {
                if (neighbourIndex == -1) break;
                int neighbourPathIndex;
                cell_t neighbourCellt;
                getPathIndexAndCelltAtIndex(neighbourIndex, neighbourPathIndex, 
                        neighbourCellt);
                if (neighbourCellt == _currentCellt) 
                {
                    int neighbourMain = _paths[neighbourPathIndex]._mainPathIndex;
                    const PathInfoHex & neighbourPath = _paths[neighbourMain];
                    // add neigbour in set
                    neighbourMainPathIndices.insert(neighbourMain);
                    // update cell data
                    isConnectedBorder1 |= neighbourPath._isConnectedBorder1;
                    isConnectedBorder2 |= neighbourPath._isConnectedBorder2;
                    mainPathIndex = std::min(mainPathIndex, neighbourMain);
                }
            }

            // if the cell is not connected to any existing path, then create a
            // new path
            if (neighbourMainPathIndices.empty()) 
            {
                _paths[_pathsEnd] = PathInfoHex(_pathsEnd, _currentCellt, 
                        isConnectedBorder1, isConnectedBorder2);
                _pathsEnd++;
            }
            // if the cell is connected to an existing path, then update paths
            // and check end of game
            else 
            {
                // update main path
                PathInfoHex & mainPath = _paths[mainPathIndex];
                mainPath._isConnectedBorder1 |= isConnectedBorder1;
                mainPath._isConnectedBorder2 |= isConnectedBorder2;

                // update other paths
                neighbourMainPathIndices.erase(mainPathIndex);
                if (not neighbourMainPathIndices.empty()) 
                {
                    for (int k=mainPathIndex+1; k<_pathsEnd; k++) 
                    {
                        int mainK = _paths[k]._mainPathIndex;
                        auto iter = neighbourMainPathIndices.find(mainK);
                        if (iter != neighbourMainPathIndices.end()) 
                            _paths[k] = mainPath;
                    }
                }

                // update winner
                if (mainPath._isConnectedBorder1 and mainPath._isConnectedBorder2)
                    _winnerCellt = _currentCellt;
            }

            // end turn and prepare for next one
            _pathBoard[index] = mainPathIndex;
            _nbEmptyIndices--;
            _lastIndex = index;
            _currentCellt = _currentCellt==CELL_WHITE ? CELL_BLACK : CELL_WHITE;
        }
    }

#ifdef LOG
    printLog();
#endif  // LOG

}

bool BoardHex::isValidCell(const Cell & refCell) const 
{
    return refCell._i >= 0 and refCell._i < _size and refCell._j >= 0 
        and refCell._j < _size;
}

bool BoardHex::isValidIndex(int index) const 
{
    return index >= 0 and index < _nbFullIndices;
}

void BoardHex::guiCells(std::vector<Cell> & whiteCells, 
        std::vector<Cell> & blackCells, std::vector<Cell> & emptyCells) const 
{
    // board
    for (int i=0; i<_size; i++) 
    {
        for (int j=0; j<_size; j++) 
        {
            Cell cell(i, j);
            cell_t cellt = getCelltAtCell(cell);
            switch (cellt) 
            {
                case CELL_WHITE: whiteCells.push_back(cell); break;
                case CELL_BLACK: blackCells.push_back(cell); break;
                default: emptyCells.push_back(cell); break;
            }
        }
    }

    // borders
    for (int k=1; k<_size; k++) 
    {

        whiteCells.push_back(Cell(k, -1));
        whiteCells.push_back(Cell(k-1, _size));
        blackCells.push_back(Cell(-1, k));
        blackCells.push_back(Cell(_size, k-1));
    }
}

int BoardHex::findNthEmptyIndex(int n) const 
{
    assert(n < _nbEmptyIndices);
    assert(n >= 0);

    int nbEmpty = 0;
    int i = 0;
    while (true) 
    {
        if (_pathBoard[i] == 0) 
        {
            if (nbEmpty == n) break;
            else nbEmpty++;
        }
        i++;
    }
    // yes, I can write ugly code too
    return i;
}

std::vector<int> BoardHex::findEmptyIndices() const 
{
    std::vector<int> emptyIndices;
    emptyIndices.reserve(_nbEmptyIndices);
    for (int k=0; k<_nbFullIndices; k++)
        if (_pathBoard[k] == 0)
            emptyIndices.push_back(k);
    return emptyIndices;
}

std::vector<int> BoardHex::findWinnerPath() const 
{
    assert(_winnerCellt != CELL_EMPTY);

    // find winning main path index
    int winPathIndex = 1;
    while (not _paths[winPathIndex]._isConnectedBorder1 
            or not _paths[winPathIndex]._isConnectedBorder2)
        winPathIndex++;

    assert(_paths[winPathIndex]._cellt == _winnerCellt);

    // find all indices connected to main path
    std::vector<int> winIndices;
    winIndices.reserve(2*_size);
    for (int k=0; k<_nbFullIndices; k++) 
    {
        int pathIndexOfK = _pathBoard[k];
        int mainPathIndexOfK = _paths[pathIndexOfK]._mainPathIndex;
        if (mainPathIndexOfK == winPathIndex)
            winIndices.push_back(k);
    }

    return winIndices;
}

void BoardHex::computeBorderConnection(int index, cell_t cellt, 
        bool & isConnectedBorder1, bool & isConnectedBorder2) const 
{
    if (cellt == CELL_WHITE) 
    {
        int j = index % _size;
        isConnectedBorder1 = (j == 0);
        isConnectedBorder2 = (j == _size-1);

    }
    else if (cellt == CELL_BLACK) 
    {
        isConnectedBorder1 = (index < _size);
        isConnectedBorder2 = (index >= _nbFullIndices - _size);
    }
    else 
    {
        isConnectedBorder1 = false;
        isConnectedBorder2 = false;
    }
}

#ifdef LOG
#include <iostream>
void BoardHex::printLog() const 
{
    std::clog << "\nboard = \n"; 

    for (int i=0; i<_size; i++) 
    {
        for (int k=0; k<i; k++)
            std::clog << "   ";
        for (int j=0; j<_size; j++) 
        {
            int index = convertCellToIndex(Cell(i, j));
            cell_t cellt = getCelltAtIndex(index);
            if (cellt == CELL_EMPTY) 
                std::clog << "   ...";
            else 
            {
                std::string type = cellt == CELL_WHITE ? "C-" : "J-";
                std::clog << "   " << type << _pathBoard[index];
            }
        }
        std::clog << "\n\n";
    }

    std::clog << "\npaths = \n"; 
    for (int k=0; k<_pathsEnd; k++) 
    {
        PathInfoHex p = _paths[k];
        std::clog << k << " = ";
        std::clog << p._mainPathIndex << " " << p._cellt << " " 
            << p._isConnectedBorder1 << " " 
            << p._isConnectedBorder2 << std::endl;
    }
    std::clog << "\n";
}
#endif // LOG

