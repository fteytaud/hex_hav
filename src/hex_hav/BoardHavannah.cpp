
#include <hex_hav/BoardHavannah.hpp>
#include <iostream>
#include <set>
#include <cstring>

PathInfoHavannah::PathInfoHavannah() 
{}

PathInfoHavannah::PathInfoHavannah(int index, cell_t cellt, unsigned borders, 
        unsigned corners) :
    PathInfo(index, cellt), _borders(borders), _corners(corners) 
{}

const std::string BoardHavannah::getBoardType() const 
{
    return "BoardHavannah";
}

void BoardHavannah::reset(int size) 
{
    assert(size >= 2);
    assert(size <= MAX_BOARDSIZE);

    // TODO refactor havannah stats
    _stats = HAV_NONE;

    _size = size - 1 + (size%2);    // ensure _size is even
    _edgeSize = (_size + 1) / 2;

    _nbFullIndices = _size * _size;     // nb indices for full hex board
    _nbIndices = _nbFullIndices - (_edgeSize-1)*_edgeSize;
    _nbEmptyIndices = _nbIndices;   // nb indices for havannah board

    _winningCycle = 0;

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
    _paths[0] = PathInfoHavannah(0, CELL_EMPTY, 0, 0);
    _pathsEnd = 1;

    // _pathBoard
    // set all cells to 0 (empty path)
    _pathBoard.fill(0);
}

bool BoardHavannah::isWinningPath(const PathInfoHavannah & path, int pathIndex, 
        int cellIndex) 
{

#ifndef HAVANNAH_NO_FORK
    // test if path is connected to 3 borders 
    if (computeNbOnes(path._borders) >= 3) 
    {
        // TODO refactor havannah stats
        _stats = path._cellt == CELL_WHITE ? HAV_WHITE_FORK : HAV_BLACK_FORK;
        return true;
    }
#endif  // HAVANNAH_NO_FORK

#ifndef HAVANNAH_NO_BRIDGE
    // test if path is connected to 2 corners 
    if (computeNbOnes(path._corners) >= 2) 
    {
        _stats = path._cellt == CELL_WHITE ? HAV_WHITE_BRIDGE : HAV_BLACK_BRIDGE;
        return true;
    }
#endif  // HAVANNAH_NO_BRIDGE

#ifndef HAVANNAH_NO_CYCLE
    // test if path is a cycle
    if (isCycle(pathIndex, cellIndex)) 
    {
        _stats = path._cellt == CELL_WHITE ? HAV_WHITE_CYCLE : HAV_BLACK_CYCLE;
        _winningCycle = pathIndex;
        return true;
    }
#endif  // HAVANNAH_NO_CYCLE

    return false;
}

std::vector<int> BoardHavannah::findPathIndices(int pathIndex) const 
{
    std::vector<int> indices;
    indices.reserve(2*_size);
    for (int k=0; k<_nbFullIndices; k++) 
    {
        int pathIndexOfK = _pathBoard[k];
        int mainPathIndexOfK = _paths[pathIndexOfK]._mainPathIndex;
        if (mainPathIndexOfK == pathIndex)
            indices.push_back(k);
    }
    return indices;
}

int BoardHavannah::computeNbNeighbours(int cellIndex, cell_t cellt) const 
{
    int nbNeighbours = 0;
    for (int neighbourIndex : _neighboursBoard[cellIndex]) 
    {
        if (neighbourIndex == -1) 
            break;
        if (getCelltAtIndex(neighbourIndex) == cellt) 
            nbNeighbours++;
    }
    return nbNeighbours; 
}

bool BoardHavannah::detectHole(const std::vector<int> & indices) const 
{

    std::vector<Cell> cells;
    cells.reserve(indices.size());

    for (int i : indices)
        cells.emplace_back(convertIndexToCell(i));

    // find bounding box
    int imin = MAX_BOARDSIZE;
    int jmin = MAX_BOARDSIZE;
    int imax = 0;
    int jmax = 0;
    for (const Cell & c : cells) 
    {
        imin = std::min(imin, c._i);
        imax = std::max(imax, c._i);
        jmin = std::min(jmin, c._j);
        jmax = std::max(jmax, c._j);
    }

    // reset data
    int data[MAX_BOARDSIZE+2][MAX_BOARDSIZE+2];
    std::memset((void*)data,0,sizeof(int)*(MAX_BOARDSIZE+2)*(MAX_BOARDSIZE+2));
    int di = imax - imin + 3;
    int dj = jmax - jmin + 3;
    for (int i=0; i<di; i++) 
    {
        data[i][0] = 1;
        data[i][dj-1] = 1;
    }
    for (int j=0; j<dj; j++) 
    {
        data[0][j] = 1;
        data[di-1][j] = 1;
    }

    // write object
    for (const Cell & c : cells) 
    {
        int i = c._i - imin + 1;
        int j = c._j - jmin + 1;
        data[i][j] = -1;
    }

#ifdef LOG
    for (int i=0; i<di; i++) 
    { 
        for (int j=0; j<dj; j++) 
            std::clog << data[i][j] << '\t';
        std::clog << std::endl;
    }
#endif  // LOG

    // propagate background
    auto fMaxNeighbour = [&data] (int i, int j) 
    {
        int d = data[i][j];
        if (d >= 0) 
        {
            d = std::max(d, data[i-1][j]);
            d = std::max(d, data[i-1][j+1]);
            d = std::max(d, data[i][j-1]);
            d = std::max(d, data[i][j+1]);
            d = std::max(d, data[i+1][j-1]);
            d = std::max(d, data[i+1][j]);
        }
        return d;
    };

    bool hasChanged = true;
    while (hasChanged) 
    {
        hasChanged = false;

        for (int i=1; i<di-1; i++) 
        {
            for (int j=1; j<dj-1; j++) 
            {
                int d = fMaxNeighbour(i, j);
                if (data[i][j] != d) 
                {
                    data[i][j] = d;
                    hasChanged = true;
                }
            }
        }

        for (int i=di-2; i>0; i--) 
        {
            for (int j=dj-2; j>0; j--) 
            {
                int d = fMaxNeighbour(i, j);
                if (data[i][j] != d) 
                {
                    data[i][j] = d;
                    hasChanged = true;
                }
            }
        }
    }

#ifdef LOG
    for (int i=0; i<di; i++) 
    { 
        for (int j=0; j<dj; j++) 
            std::clog << data[i][j] << '\t';
        std::clog << std::endl;
    }
#endif  // LOG

    // check initial background
    for (int i=0; i<di; i++) 
        for (int j=0; j<dj; j++) 
            if (data[i][j] == 0)
                return true;

    return false;
}

bool BoardHavannah::isCycle(int pathIndex, int cellIndex) const 
{
    cell_t currentCellt = _paths[pathIndex]._cellt;

    // compute full path
    std::vector<int> indices = findPathIndices(pathIndex);

    // check if full path has 6 cells at least
    if (indices.size() < 6)
        return false;

    // check if cell is connected to two previous cells at least
    if (computeNbNeighbours(cellIndex, currentCellt) < 2)
        return false;

    // detect interior point
    for (int index : indices) 
        if (computeNbNeighbours(index, currentCellt) == 6)
            return true;

    // detect hole
    return detectHole(indices);
}

unsigned BoardHavannah::computeBorders(int index) const 
{
    unsigned borders = 0;
    Cell c = convertIndexToCell(index);
    if (isValidCell(c)) 
    {
        int i = c._i;
        int j = c._j;
        int e1 = _edgeSize - 1;
        int s1 = _size - 1;
        if (i==0 and e1<j and j<s1)         borders += 1;
        if (0<i and i<e1 and j==s1)         borders += 2;
        if (i+j==3*e1 and i<s1 and j<s1)    borders += 4;
        if (i==s1 and 0<j and j<e1)         borders += 8;
        if (e1<i and i<s1 and j==0)         borders += 16;
        if (i+j==e1 and i>0 and j>0)        borders += 32;
    }
    return borders;
}

unsigned BoardHavannah::computeCorners(int index) const 
{
    unsigned corners = 0;
    Cell c = convertIndexToCell(index);
    if (isValidCell(c)) 
    {
        int i = c._i;
        int j = c._j;
        int e1 = _edgeSize - 1;
        int s1 = _size - 1;
        if (i == 0 and j == e1)     corners += 1;
        if (i == 0 and j == s1)     corners += 2;
        if (i == e1 and j == s1)    corners += 4;
        if (i == s1 and j == e1)    corners += 8;
        if (i == s1 and j == 0)     corners += 16;
        if (i == e1 and j == 0)     corners += 32;
    }
    return corners;
}

int BoardHavannah::computeNbOnes(unsigned f) const 
{
    int n = f & 1u;
    f >>= 1;    n += f & 1u; 
    f >>= 1;    n += f & 1u; 
    f >>= 1;    n += f & 1u; 
    f >>= 1;    n += f & 1u; 
    f >>= 1;    n += f & 1u; 
    return n;
}

void BoardHavannah::play(int index) 
{
    assert(isValidIndex(index));
    assert(_pathBoard[index] == 0);

    if (not isGameFinished()) 
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
            int borders = computeBorders(index);
            int corners = computeCorners(index);

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
                    const PathInfoHavannah & neighbourPath = _paths[neighbourMain];
                    // add neigbour in set
                    neighbourMainPathIndices.insert(neighbourMain);
                    // update cell data
                    borders |= neighbourPath._borders;
                    corners |= neighbourPath._corners;
                    mainPathIndex = std::min(mainPathIndex, neighbourMain);
                }
            }

            // if the cell is not connected to any existing path, then create a
            // new path
            if (neighbourMainPathIndices.empty()) 
            {
                _paths[_pathsEnd] = 
                    PathInfoHavannah(_pathsEnd, _currentCellt, borders, corners);
                _pathsEnd++;
                _pathBoard[index] = mainPathIndex;
            }
            // if the cell is connected to an existing path, then update paths
            // and check end of game
            else 
            {
                // update main path
                PathInfoHavannah & mainPath = _paths[mainPathIndex];
                mainPath._borders |= borders;
                mainPath._corners |= corners;

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
                _pathBoard[index] = mainPathIndex;

                // update winner
                if (isWinningPath(mainPath, mainPathIndex, index))
                    _winnerCellt = _currentCellt;
            }

            // end turn and prepare for next one
            _nbEmptyIndices--;
            _lastIndex = index;
            _currentCellt = _currentCellt==CELL_WHITE ? CELL_BLACK : CELL_WHITE;
        }
    }

#ifdef LOG
    printLog();
#endif  // LOG
}

bool BoardHavannah::isValidCell(const Cell & refCell) const 
{
    int i = refCell._i;
    int j = refCell._j;
    return i>=0 and i<_size and j>=0 and j<_size and i+j>=_edgeSize-1 
        and i+j<=3*_edgeSize-3;
}

bool BoardHavannah::isValidIndex(int index) const 
{
    Cell c = convertIndexToCell(index);
    return isValidCell(c);
}

void BoardHavannah::guiCells(std::vector<Cell> & whiteCells, 
        std::vector<Cell> & blackCells, std::vector<Cell> & emptyCells) const 
{
    for (int i=0; i<_size; i++) 
    {
        for (int j=0; j<_size; j++) 
        {
            Cell cell(i, j);
            cell_t cellt = getCelltAtCell(cell);
            if (isValidCell(cell)) 
            {
                switch (cellt) 
                {
                    case CELL_WHITE: whiteCells.push_back(cell); break;
                    case CELL_BLACK: blackCells.push_back(cell); break;
                    default: emptyCells.push_back(cell); break;
                }
            }
        }
    }
}

int BoardHavannah::findNthEmptyIndex(int n) const 
{
    assert(n < _nbEmptyIndices);
    assert(n >= 0);

    int nbEmpty = 0;
    int i = 0;
    while (true) 
    {
        if (isValidIndex(i) and _pathBoard[i] == 0) 
        {
            if (nbEmpty == n) break;
            else nbEmpty++;
        }
        i++;
    }
    // yes, I can write ugly code too
    return i;
}

std::vector<int> BoardHavannah::findEmptyIndices() const 
{
    std::vector<int> emptyIndices;
    emptyIndices.reserve(_nbEmptyIndices);
    for (int k=0; k<_nbFullIndices; k++)
        if (isValidIndex(k) and _pathBoard[k] == 0)
            emptyIndices.push_back(k);
    return emptyIndices;
}

std::vector<int> BoardHavannah::findWinnerPath() const 
{

    assert(_winnerCellt != CELL_EMPTY);

    // find winning path
    int winPathIndex;
    if (_winningCycle != 0) 
        winPathIndex = _winningCycle;
    else 
    {
        winPathIndex = 1;
        while (true) 
        {
            const PathInfoHavannah & path = _paths[winPathIndex];
            if (computeNbOnes(path._borders) >= 3) break;
            if (computeNbOnes(path._corners) >= 2) break;
            winPathIndex++;
        }
    }

    assert(_paths[winPathIndex]._cellt == _winnerCellt);

    // find all indices connected to winning path
    return findPathIndices(winPathIndex);
}

#ifdef LOG
#include <iostream>
void BoardHavannah::printLog() const 
{

    std::clog << "\nboard = \n"; 

    for (int i=0; i<_size; i++) 
    {
        for (int k=0; k<i; k++)
            std::clog << "   ";
        for (int j=0; j<_size; j++) 
        {
            Cell cell(i, j);
            int index = convertCellToIndex(cell);
            cell_t cellt = getCelltAtIndex(index);
            if (not isValidCell(cell)) 
                std::clog << "      ";
            else if (cellt == CELL_EMPTY) 
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
        PathInfoHavannah p = _paths[k];
        std::clog << k << " = ";
        std::clog << p._mainPathIndex << " " << p._cellt << " " 
            << p._borders << " " << p._corners << std::endl;
    }

    std::clog << "\n";
}
#endif // LOG

