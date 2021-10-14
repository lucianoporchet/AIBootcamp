#include "Move_to_next.h"

status Move_to_next::run(std::list<SOrder>& _orders, int index, const STurnData& _turnData, Logger& l)
{
	l.Log("Move to next", true);
	Grid& grid = Grid::get();

	Tile pos = Tile{ _turnData.npcInfoArray[index].q, _turnData.npcInfoArray[index].r };
	Tile dir{ 0,0 };
	Tile next{ 0,0 };

	std::vector<Tile> neighbors = grid.neighbors(pos);
	int size = neighbors.size();
	int c = 0;

	for (auto n : neighbors) {
		if (!grid.wasVisited(n, index)) {
			next = n;
			dir = Tile{ next.q - pos.q, next.r - pos.r };
			break;
		}
		++c;
	}

	l.Log(std::to_string(size), true);

	

	if (c == size) {
		next = neighbors.back();
		dir = Tile{ next.q - pos.q, next.r - pos.r };
	}

	


	//Move
	//choose npc state

	if (grid.isReserved(next)) {

		grid.npc_states[index] = State::BLOQUED;
	}
	else
		grid.npc_states[index] = State::MOVE;


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

	if (size == 1) grid.addForbidden(pos);
	return status::SUCCESS;
}
