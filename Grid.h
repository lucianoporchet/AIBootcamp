#pragma once
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <tuple>
#include <algorithm>
#include <cstdlib>
#include <string>
#include "Globals.h"


struct Tile {
	int q, r;
};
bool operator==(Tile a, Tile b);
bool operator!=(Tile a, Tile b);
bool operator<(Tile a, Tile b);

namespace std {
	template <> struct hash<Tile> {
		typedef Tile argument_type;
		typedef std::size_t result_type;
		std::size_t operator()(const Tile& id) const noexcept {
			return std::hash<int>()(id.q ^ (id.r << 4));
		}
	};
}

template<typename T, typename priority_t>
struct PriorityQueue {
	typedef std::pair<priority_t, T> PQElement;
	std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> elements;

	inline bool empty() const {
		return elements.empty();
	}

	inline void put(T item, priority_t priority) {
		elements.emplace(priority, item);
	}

	T get() {
		T best_item = elements.top().second;
		elements.pop();
		return best_item;
	}
};

struct Grid
{
	static std::array<Tile, 6> DIRS;
	int width, height, q_bound, r_bound;
	std::unordered_set<Tile> forbidden;
	std::unordered_set<Tile> map;
	std::unordered_set<Tile> reserved;
	std::vector<SObjectInfo> objects;

	Grid(int width_, int height_, int qb, int rb) : width(width_), height(height_), q_bound(qb), r_bound(rb) {}

	EHexCellDirection reverseDir(EHexCellDirection dir) const;
	EHexCellDirection getDir(const Tile& t) const;

	bool notObstructed(Tile next, Tile id, EHexCellDirection dir) const;
	bool inBounds(Tile id) const;
	bool passable(Tile id) const;
	std::vector<Tile> neighbors(Tile id) const;
	bool isReserved(std::vector<Tile>& path);

	inline void reserveNext(std::vector<Tile>& path) {
		reserved.emplace(path.back());
	}

	static void addForbidden(Grid& grid, Tile t);
	
};

