#include "Move_to_next.h"

status Move_to_next::run(std::list<SOrder>& _orders, int index, const STurnData& _turnData, Logger& l)
{
	l.Log("Move to next", true);
	Grid& grid = Grid::get();

	Tile pos = Tile{ _turnData.npcInfoArray[index].q, _turnData.npcInfoArray[index].r };
	Tile dir{ 0,0 };
	Tile next{ 0,0 };
	Tile next_goal{ 0,0 };

	std::vector<Tile> neighbors = grid.neighbors(pos);
	int size = static_cast<int>(neighbors.size());
	//int c = 0;
	if (size == 1) grid.addDeadEnd(pos);
	if (!grid.List_of_intermediate_paths[index].empty()) {
		next = grid.List_of_intermediate_paths[index].back();
	}
	else {
		std::vector<Tile> nevervisited{};
		for (auto n : neighbors) {
			if (!grid.wasVisitedAllNPC(n)) {
				nevervisited.push_back(n);
			}
			else if (!grid.wasVisited(n, index)) {
				grid.to_visit[index].push(n);
			}
		}

		for (auto nv : nevervisited) {
			grid.to_visit[index].push(nv);
		}

		next_goal = grid.to_visit[index].top();
		while (grid.isDeadEnd(next_goal)) {
			grid.to_visit[index].pop();
			next_goal = grid.to_visit[index].top();
		}
		grid.to_visit[index].pop();

		a_star_search(grid, pos, next_goal, grid.came_from, grid.cost_so_far);

		grid.List_of_intermediate_paths[index] = reconstruct_path(pos, next_goal, grid.came_from);
		grid.came_from.clear();
		grid.cost_so_far.clear();

		next = grid.List_of_intermediate_paths[index].back();
	}
	

	dir = Tile{ next.q - pos.q, next.r - pos.r };
	//choose npc state
	if (grid.isReserved(next)) {

		grid.npc_states[index] = State::BLOQUED;
	}
	else
	{
		grid.npc_states[index] = State::MOVE;
		l.Log(std::to_string(grid.List_of_intermediate_paths[index].back().q) + ' ' + std::to_string(grid.List_of_intermediate_paths[index].back().r));
		grid.List_of_intermediate_paths[index].pop_back();
	}


	//final state machine
	switch (grid.npc_states[index])
	{
	case State::MOVE:
		grid.reserveNext(next);
		grid.visitNext(next, index);
		_orders.push_back(SOrder{ EOrderType::Move, _turnData.npcInfoArray[index].uid, grid.getDir(dir) });
		break;
	case State::BLOQUED:
		_orders.push_back(SOrder{ EOrderType::Move, _turnData.npcInfoArray[index].uid, EHexCellDirection::CENTER });
		break;
	}
	return status::SUCCESS;
}
