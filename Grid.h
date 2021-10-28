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
#include <stack>

#include "Globals.h"
#include "InitData.h"
#include "TurnData.h"

enum class State {
	MOVE, BLOQUED, ARRIVED
};

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

private:
	Grid() : width{ 0 }, height{ 0 }, q_bound{ 0 }, r_bound{ 0 }, newGoal{ false }{}
	int width, height, q_bound, r_bound;
	std::unordered_set<Tile> forbidden;
	std::unordered_set<Tile> map;
	std::vector<std::vector<Tile>> visited;
	std::unordered_set<Tile> reserved_goals;
	std::vector<SObjectInfo> objects;
	std::unordered_set<Tile> deadEnd;
public:
	Grid(const Grid&) = delete;
	Grid& operator=(const Grid&) = delete;

	static std::array<Tile, 6> DIRS;
	std::unordered_set<Tile> reserved;
	bool newGoal;
	std::vector<Tile> goals;
	std::unordered_map<Tile, Tile> came_from;
	std::unordered_map<Tile, int> cost_so_far;
	std::vector<std::vector<Tile>> List_of_paths;
	std::vector<std::vector<Tile>> List_of_intermediate_paths;
	std::vector<State> npc_states;
	bool changePath = true;
	int nextDir = 0;
	std::vector<std::stack<Tile>> to_visit;
public:
	void InitGrid(const SInitData& _initData);
	void updateGrid(const STurnData& turnInfo);
	EHexCellDirection reverseDir(EHexCellDirection dir) const;
	EHexCellDirection getDir(const Tile& t) const;
	bool notObstructed(Tile next, Tile id, EHexCellDirection dir) const;
	bool inBounds(const Tile& id) const;
	bool passable(const Tile& id) const;
	bool isReserved(const Tile& t);
	bool wasVisited(const Tile& a, int i);
	bool freeGoal(const Tile& t) const;
	bool isDeadEnd(const Tile& t);
	std::vector<Tile> neighbors(Tile id) const;

	inline void reserveNext(Tile a) {
		reserved.insert(a);
	}
	inline void visitNext(Tile a, int i) {
		visited[i].push_back(a);
	}
	inline void addForbidden(Tile t) {
		forbidden.insert(t);
	}
	inline void reserveGoal(Tile a) {
		reserved_goals.insert(a);
	}
	inline void addDeadEnd(Tile a) {
		deadEnd.insert(a);
	}
	
	static Grid& get();
};

EHexCellDirection chooseDirection(std::vector<Tile>& path, SNPCInfo& npc);
EHexCellDirection nextDirection(EHexCellDirection dir);
bool goalInPath(const std::vector<std::vector<Tile>>& list_path, const Tile& goal, int index);