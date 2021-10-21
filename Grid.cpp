#include "Grid.h"
#include "A_star.h"


bool operator==(Tile a, Tile b) {
	return a.q == b.q && a.r == b.r;
}
bool operator!=(Tile a, Tile b) {
	return !(a == b);
}
bool operator<(Tile a, Tile b) {
	return std::tie(a.q, a.r) < std::tie(b.q, b.r);
}


bool Grid::inBounds(const Tile& id) const {
	return map.find(id) != map.end();
}
bool Grid::passable(const Tile& id) const {
	return forbidden.find(id) == forbidden.end();
}
bool Grid::freeGoal(const Tile& t) const {
	return reserved_goals.find(t) == reserved_goals.end();
}
bool Grid::isReserved(const Tile& t) {
	return reserved.find(t) != reserved.end();
}
bool Grid::wasVisited(const Tile& a, int i) {
	return std::find(begin(visited[i]), end(visited[i]), a) != end(visited[i]);
}
bool Grid::isDeadEnd(const Tile& t) {
	return deadEnd.find(t) != deadEnd.end();
}

void Grid::InitGrid(const SInitData& _initData) {
	npc_states.assign(_initData.nbNPCs, State::MOVE);
	int q, r;
	Tile goal{ 0,0 }, start{ 0,0 };
	width = _initData.colCount;
	height = _initData.rowCount;
	std::vector<Tile> v;
	for (int i = 0; i < _initData.nbNPCs; ++i) {
		v.push_back(Tile{_initData.npcInfoArray[i].q, _initData.npcInfoArray[i].r});
		visited.push_back(v);
		List_of_paths.push_back(std::vector<Tile>{});
	}
	for (int i = 0; i < _initData.tileInfoArraySize; i++) {
		q = _initData.tileInfoArray[i].q;
		r = _initData.tileInfoArray[i].r;
		map.insert(Tile{ q,r });
		if (_initData.tileInfoArray[i].type == EHexCellType::Forbidden) {
			forbidden.insert(Tile{ q,r });
		}
		if (_initData.tileInfoArray[i].type == EHexCellType::Goal) {
			goals.push_back(Tile{ _initData.tileInfoArray[i].q, _initData.tileInfoArray[i].r });
		}
	}
	for (int i = 0; i < _initData.objectInfoArraySize; i++) {
		objects.push_back(_initData.objectInfoArray[i]);
	}
	if (goals.size() == 0) newGoal = false;
}

void Grid::updateGrid(const STurnData& turnInfo) {
	int temp = goals.size(), q, r;
	for (int i = 0; i < turnInfo.tileInfoArraySize; i++) {
		q = turnInfo.tileInfoArray[i].q;
		r = turnInfo.tileInfoArray[i].r;
		map.insert(Tile{ q,r });
		if (turnInfo.tileInfoArray[i].type == EHexCellType::Forbidden) {
			forbidden.insert(Tile{ q,r });
		}
		if (turnInfo.tileInfoArray[i].type == EHexCellType::Goal && freeGoal(Tile{ turnInfo.tileInfoArray[i].q, turnInfo.tileInfoArray[i].r })) {
			goals.push_back(Tile{turnInfo.tileInfoArray[i].q, turnInfo.tileInfoArray[i].r });
		}
	}
	if (temp < goals.size()) newGoal = true;
	for (int i = 0; i < turnInfo.objectInfoArraySize; i++) {
		objects.push_back(turnInfo.objectInfoArray[i]);
	}
}

EHexCellDirection Grid::reverseDir(EHexCellDirection dir) const
{
	return (EHexCellDirection)((dir + 3) % 6);
}

EHexCellDirection Grid::getDir(const Tile& t) const {

	if (t.q == 0) {
		if (t.r == 1)
			return EHexCellDirection::E;
		else
			return EHexCellDirection::W;
	}
	else if(t.q == 1){
		if (t.r == 0)
			return EHexCellDirection::SE;
		else
			return EHexCellDirection::SW;
	}
	else{
		if (t.r == 0) {
			return EHexCellDirection::NW;
		}
		else
			return EHexCellDirection::NE;
	}
}

bool Grid::notObstructed(Tile next,Tile id, EHexCellDirection dir) const{
	
	if (!objects.empty()) {
		for (const auto& obj : objects) {
			if ((obj.q == next.q && obj.r == next.r && obj.cellPosition == reverseDir(dir))
				|| (obj.q == id.q && obj.r == id.r && obj.cellPosition == dir)) {
				return false;
			}
		}
	}
	return true;
}

std::array<Tile, 6> Grid::DIRS = {
	/* SE, NW, W, E, NE, SW */ 
	Tile{0, -1}, Tile{-1, 0}, Tile{-1,1}, Tile{0, 1}, Tile{1, 0},  Tile{1, -1}
};

std::vector<Tile> Grid::neighbors(Tile id) const {
	std::vector<Tile> results;
	for (const Tile& dir : DIRS) {
		Tile next{ id.q + dir.q, id.r + dir.r };
		if (inBounds(next) && passable(next) && notObstructed(next, Tile{ id.q, id.r }, getDir(dir))) {
			results.push_back(next);
		}
	}
	if ((id.q + id.r) % 2 == 0) {
		std::reverse(results.begin(), results.end());
	}
	return results;
}

Grid& Grid::get() {
	static Grid instance;
	return instance;
}

EHexCellDirection chooseDirection(std::vector<Tile>& path, SNPCInfo& npc) {

	Tile next;
	next.q = path.back().q;
	next.r = path.back().r;

	if (next.q == npc.q && next.r == npc.r) {
		return EHexCellDirection::CENTER;
	}
	path.pop_back();

	if (next.q < npc.q) {
		if (next.r == npc.r) {
			return EHexCellDirection::NW;
		}
		else
			return EHexCellDirection::NE;
	}
	else if (next.q == npc.q) {
		if (next.r < npc.r) {
			return EHexCellDirection::W;
		}
		else
			return EHexCellDirection::E;
	}
	else {
		if (next.r == npc.r) {
			return EHexCellDirection::SE;
		}
		else
			return EHexCellDirection::SW;
	}

}

EHexCellDirection nextDirection(EHexCellDirection dir) {
	return (EHexCellDirection)((dir + 1) % 6);
}

bool goalInPath(const std::vector<std::vector<Tile>>& list_path, const Tile& goal, int index) {

	for (auto i : list_path) {
		if (!i.empty() && i != list_path[index] && std::any_of(begin(i), end(i), [&](Tile a) { return goal == a;  }))
			return true;
	}
	return false;
}