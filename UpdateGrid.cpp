#include "UpdateGrid.h"


status UpdateGrid::run(std::list<SOrder>&, int, const STurnData& turnInfo, Logger& l) {
	l.Log("Update grid", true);
	Grid& grid = Grid::get();
	grid.updateGrid(turnInfo);
	return status::SUCCESS;

}