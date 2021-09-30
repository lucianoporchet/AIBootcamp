#include "MyBotLogic.h"

#include "Globals.h"
#include "ConfigData.h"
#include "InitData.h"
#include "TurnData.h"


#include <Grid.h>


enum class State {
	MOVE, BLOQUED, ARRIVED
};

EHexCellDirection chooseDirection(std::vector<Tile>& path, SNPCInfo& npc){

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


bool goalInPath(std::vector<std::vector<Tile>>& list_path, Tile goal, int index) {

	bool result = false;
	for (auto i : list_path) {
		if(!i.empty() && i != list_path[index])
			result = std::any_of(begin(i), end(i), [&](Tile a) { return goal == a;  });
	}
	return result;
}

inline int heuristic(Tile a, Tile b) {
	return std::abs(a.q - b.q) + std::abs(a.r - b.r);
}

template<typename Location, typename Graph>
void a_star_search(
	Graph graph,
	Location start, Location goal,
	std::unordered_map<Location, Location>& came_from, std::unordered_map<Location, int>& cost_so_far) {

	PriorityQueue<Location, int> frontier;
	frontier.put(start, 0);

	came_from[start] = start;
	cost_so_far[start] = 0;

	while (!frontier.empty()) {
		Location current = frontier.get();

		if (current == goal) {
			break;
		}

		for (Location next : graph.neighbors(current)) {
			int new_cost = cost_so_far[current] + 1;
			if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]) {
				cost_so_far[next] = new_cost;
				int priority = new_cost + heuristic(next, goal);
				frontier.put(next, priority);
				came_from[next] = current;
			}
		}
	}
}

template<typename Location>
std::vector<Location> reconstruct_path(Location start, Location goal, std::unordered_map<Location, Location> came_from) {
	std::vector<Location> path;
	Location current = goal;
	while (current != start) {
		path.push_back(current);
		current = came_from[current];
	}
	//path.push_back(start);
	//std::reverse(path.begin(), path.end());
	return path;
}


MyBotLogic::MyBotLogic()
{
	//Write Code Here
}

MyBotLogic::~MyBotLogic()
{
	//Write Code Here
}

void MyBotLogic::Configure(const SConfigData& _configData)
{
#ifdef BOT_LOGIC_DEBUG
	mLogger.Init(_configData.logpath, "MyBotLogic.log");
#endif

	BOT_LOGIC_LOG(mLogger, "Configure", true);

	//Write Code Here
}



std::unordered_map<Tile, Tile> came_from;
std::unordered_map<Tile, int> cost_so_far;
std::vector<std::vector<Tile>> List_of_paths;//the path to follow for each mpc
std::vector<Tile> goals;//vector of all goal tiles in the map
std::vector<State> npc_states;
Grid grid{ 0,0,0,0 };
void MyBotLogic::Init(const SInitData& _initData)
{
	BOT_LOGIC_LOG(mLogger, "Init", true);

	npc_states.assign(_initData.nbNPCs, State::MOVE);
	
	int colCount = _initData.colCount;
	int rowCount = _initData.rowCount;
	int q{ 0 }, r{ static_cast<int>(-(floor((rowCount - 1) / 2))) };
	Tile goal{ 0,0 }, start{ 0,0 };

	grid.width = colCount;
	grid.height = rowCount;
	grid.q_bound = q;
	grid.r_bound = r;

	for (int i = 0; i < _initData.tileInfoArraySize; i++) {
		q = _initData.tileInfoArray[i].q;
		r = _initData.tileInfoArray[i].r;
		grid.map.insert(Tile{ q,r });
		if (_initData.tileInfoArray[i].type == EHexCellType::Forbidden) {
			Grid::addForbidden(grid, Tile{ q,r });
		}
		if (_initData.tileInfoArray[i].type == EHexCellType::Goal) {
			goals.push_back(Tile{ _initData.tileInfoArray[i].q, _initData.tileInfoArray[i].r });
		}
	}


	
	//find path for each npc
	for (int i = 0; i < _initData.nbNPCs; i++) {

		start.q = _initData.npcInfoArray[i].q;
		start.r = _initData.npcInfoArray[i].r;

		//sort goals closest to start first
		std::sort(begin(goals), end(goals), [&](const Tile& a, const Tile& b) { return heuristic(a, start) < heuristic(b, start); });
		
		goal.q = goals[0].q;
		goal.r = goals[0].r;
		//reserve goal for a specific npc
		goals.erase(begin(goals));

		a_star_search(grid, start, goal, came_from, cost_so_far);
		List_of_paths.push_back(reconstruct_path(start, goal, came_from));

		came_from.clear();
		cost_so_far.clear();
	}

}

bool changePath = false;

void MyBotLogic::GetTurnOrders(const STurnData& _turnData, std::list<SOrder>& _orders)
{
	BOT_LOGIC_LOG(mLogger, "GetTurnOrders", true);

	/*std::string info = "Player pos: " + std::to_string(_turnData.npcInfoArray[0].q) + ' ' + std::to_string(_turnData.npcInfoArray[0].r);
	BOT_LOGIC_LOG(mLogger, info, true);*/

	_orders.clear();
	grid.reserved.clear();

	for (int i = 0; i < _turnData.npcInfoArraySize; ++i) {	
		
		//if the npc is not done
		if (!List_of_paths[i].empty()) {
			
			//redo A* to find new path if necessary
			if (changePath) {
				Tile goal = List_of_paths[i][0];
				a_star_search(grid, Tile{ _turnData.npcInfoArray[i].q, _turnData.npcInfoArray[i].r }, List_of_paths[i][0], came_from, cost_so_far);
				List_of_paths[i].swap(reconstruct_path(Tile{ _turnData.npcInfoArray[i].q, _turnData.npcInfoArray[i].r }, goal, came_from));
				changePath = false;
			}


			//choose npc state
			if (List_of_paths[i].size() <= 1) {
				npc_states[i] = State::ARRIVED;
			}
			else if (grid.isReserved(List_of_paths[i])) {

				npc_states[i] = State::BLOQUED;
			}
			else
				npc_states[i] = State::MOVE;


			//final state machine
			switch (npc_states[i])
			{
			case State::MOVE:
				grid.reserveNext(List_of_paths[i]);
				_orders.push_back(SOrder{ EOrderType::Move, _turnData.npcInfoArray[i].uid, chooseDirection(List_of_paths[i], _turnData.npcInfoArray[i]) });
				break;
			case State::BLOQUED:
				_orders.push_back(SOrder{ EOrderType::Move, _turnData.npcInfoArray[i].uid, EHexCellDirection::CENTER });
				break;
			case State::ARRIVED:
				grid.reserveNext(List_of_paths[i]);
				if (goalInPath(List_of_paths, List_of_paths[i][0], i)) {
					Grid::addForbidden(grid, List_of_paths[i][0]);
					changePath = true;
				}
				_orders.push_back(SOrder{ EOrderType::Move, _turnData.npcInfoArray[i].uid, chooseDirection(List_of_paths[i], _turnData.npcInfoArray[i]) });
				break;
			}
		}
	}


}



/*
	Possible improvements:
		
		run in release lol
		See if a goal is in the path an npc from init so no need to redo A_star
		
*/