
#include <hex_hav/Board.hpp>

Cell::Cell() : _i(0), _j(0) 
{}

Cell::Cell(int i, int j) : _i(i), _j(j) 
{}

bool Cell::operator==(const Cell & refCell) const 
{
    return _i == refCell._i and _j == refCell._j; 
};

PathInfo::PathInfo(int index, cell_t cellt) :
    _mainPathIndex(index), _cellt(cellt) 
{}

