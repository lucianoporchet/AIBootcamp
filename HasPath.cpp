#include "HasPath.h"

status HasPath::run(std::list<SOrder>&, int index, const STurnData& _turnData, Logger& l)
{
	l.Log("Has path", true);
	Grid& grid = Grid::get();

	if (!grid.freeGoal(Tile{ _turnData.npcInfoArray[index].q, _turnData.npcInfoArray[index].r })) return status::SUCCESS;
	if (grid.List_of_paths.size() <= index) return status::FAILURE;
	if (!grid.List_of_paths[index].empty()) return status::SUCCESS;

	return status::FAILURE;
}
