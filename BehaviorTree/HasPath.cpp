#include "HasPath.h"

status HasPath::run(std::list<SOrder>&, int index, const STurnData& _turnData, Logger& l)
{
	Grid& grid = Grid::get();

	if (grid.npc_states[index] == State::ARRIVED) return status::SUCCESS;
	if (!grid.List_of_paths[index].empty()) return status::SUCCESS;

	return status::FAILURE;
}
