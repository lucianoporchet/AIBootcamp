#include "A_star_to_goal.h"


status A_star_to_goal::run(std::list<SOrder>&, int index, const STurnData& turnInfo, Logger& l) {
	l.Log("A to goal");
	Tile pos{ turnInfo.npcInfoArray[index].q, turnInfo.npcInfoArray[index].r };
	Grid& grid = Grid::get();

	std::sort(begin(grid.goals), end(grid.goals), [&](const Tile& a, const Tile& b) { return heuristic(a, pos) < heuristic(b, pos); });

	for (auto g : grid.goals) {
		a_star_search(grid, pos, g, grid.came_from, grid.cost_so_far);
		if (grid.came_from.find(g) != grid.came_from.end()) {
			grid.List_of_paths[index] = reconstruct_path(pos, g, grid.came_from);
			grid.came_from.clear();
			grid.cost_so_far.clear();
			grid.reserveGoal(g);
			grid.goals.erase(std::remove(grid.goals.begin(), grid.goals.end(), g), grid.goals.end());
			return status::SUCCESS;
		}
	}
	return status::FAILURE;
}
