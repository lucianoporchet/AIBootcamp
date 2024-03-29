#include "NewGoalVisible.h"

status NewGoalVisible::run(std::list<SOrder>&, int, const STurnData&, Logger& l) {

	l.Log("New goal visible", true);
	Grid& grid = Grid::get();
	if (grid.goals.size() != 0 && grid.newGoal) {
		return status::SUCCESS;
	}
	return status::FAILURE;
}