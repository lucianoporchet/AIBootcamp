#include "HasPath.h"

status HasPath::run(std::list<SOrder>&, int index, const STurnData&)
{
	Grid& grid = Grid::get();

	if (grid.List_of_paths.size() <= index) return status::FAILURE;

	if (!grid.List_of_paths[index].empty())
		return status::SUCCESS;
	return status::FAILURE;
}
