#include "Grid.h"


bool operator==(Tile a, Tile b) {
	return a.q == b.q && a.r == b.r;
}

bool operator!=(Tile a, Tile b) {
	return !(a == b);
}

bool operator<(Tile a, Tile b) {
	return std::tie(a.q, a.r) < std::tie(b.q, b.r);
}

bool Grid::inBounds(Tile id) const {
	return map.find(id) != map.end();
}

bool Grid::passable(Tile id) const {
	return forbidden.find(id) == forbidden.end();
}

std::array<Tile, 6> Grid::DIRS = {
	/* East, West, North, South */
	Tile{1, 0}, Tile{-1, 0},
	Tile{0, -1}, Tile{0, 1}, Tile{-1,1}, Tile{1, -1}
};

std::vector<Tile> Grid::neighbors(Tile id) const {
	std::vector<Tile> results;

	for (const Tile& dir : DIRS) {
		Tile next{ id.q + dir.q, id.r + dir.r };
		if (inBounds(next) && passable(next)) {
			results.push_back(next);
		}
	}

	if ((id.q + id.r) % 2 == 0) {
		std::reverse(results.begin(), results.end());
	}

	return results;
}


void Grid::addForbidden(Grid& grid, Tile t) {
	grid.forbidden.insert(t);
}

bool Grid::isReserved(std::vector<Tile>& path){

	if (reserved.find(path.back()) == reserved.end()) {
		return false;
	}
	return true;
}