#include "Move_to_goal.h"

status Move_to_goal::run(std::list<SOrder>& _orders, int i, const STurnData& _turnData, Logger& l)
{
	l.Log("Move to goal", true);
	Grid& grid = Grid::get();
	
	if (!grid.List_of_paths[i].empty()) {
		//redo A* to find new path if necessary
		if (grid.changePath) {
			Tile goal = grid.List_of_paths[i][0];
			a_star_search(grid, Tile{ _turnData.npcInfoArray[i].q, _turnData.npcInfoArray[i].r }, goal, grid.came_from, grid.cost_so_far);
			grid.List_of_paths[i].swap(reconstruct_path(Tile{ _turnData.npcInfoArray[i].q, _turnData.npcInfoArray[i].r }, goal, grid.came_from));
			grid.changePath = false;
			grid.came_from.clear();
			grid.cost_so_far.clear();
		}

		//choose npc state
		if (grid.List_of_paths[i].size() <= 1) {
			grid.npc_states[i] = State::ARRIVED;
		}
		else if (grid.isReserved(grid.List_of_paths[i].back())) {

			grid.npc_states[i] = State::BLOQUED;
		}
		else
			grid.npc_states[i] = State::MOVE;


		//final state machine
		switch (grid.npc_states[i])
		{
		case State::MOVE:
			grid.reserveNext(grid.List_of_paths[i].back());
			_orders.push_back(SOrder{ EOrderType::Move, _turnData.npcInfoArray[i].uid, chooseDirection(grid.List_of_paths[i], _turnData.npcInfoArray[i]) });
			break;
		case State::BLOQUED:
			_orders.push_back(SOrder{ EOrderType::Move, _turnData.npcInfoArray[i].uid, EHexCellDirection::CENTER });
			break;
		case State::ARRIVED:
			grid.reserveNext(grid.List_of_paths[i].back());
			if (goalInPath(grid.List_of_paths, grid.List_of_paths[i][0], i)) {
				grid.addForbidden(grid.List_of_paths[i][0]);
				grid.changePath = true;
			}
			_orders.push_back(SOrder{ EOrderType::Move, _turnData.npcInfoArray[i].uid, chooseDirection(grid.List_of_paths[i], _turnData.npcInfoArray[i]) });
			break;
		}
	}
	return status::SUCCESS;
}
