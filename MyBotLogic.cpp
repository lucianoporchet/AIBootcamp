#include "MyBotLogic.h"

#include "Globals.h"
#include "ConfigData.h"
#include "InitData.h"
#include "TurnData.h"

#include "Grid.h"
#include "A_star.h"
#include "Selector.h"
#include "Sequence.h"
#include "A_star_to_goal.h"
#include "UpdateGrid.h"
#include "NewGoalVisible.h"
#include "HasPath.h"
#include "Move_to_goal.h"
#include "Move_to_next.h"

#include <chrono>
//using namespace std::chrono;


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





Grid& grid = Grid::get();
Selector behaviourTree;
bool omniscient;

//nanoseconds result;
void MyBotLogic::Init(const SInitData& _initData)
{
	
	//BOT_LOGIC_LOG(mLogger, "Init", true);
	//auto t = high_resolution_clock::now();
	omniscient = _initData.omniscient;


	grid.InitGrid(_initData);
	//result = high_resolution_clock::now() - t;
	behaviourTree = Selector(
						new Sequence(
							new UpdateGrid(),
							new HasPath(),
							new Move_to_goal()
						), 
						new Sequence(
							new UpdateGrid(),
							new NewGoalVisible(), 
							new A_star_to_goal(), 
							new Move_to_goal() 
						),
						new Sequence(
							new UpdateGrid(),
							new Move_to_next()
						)
					);
	
}



void MyBotLogic::GetTurnOrders(const STurnData& _turnData, std::list<SOrder>& _orders)
{
	//BOT_LOGIC_LOG(mLogger, "GetTurnOrders", true);

	/*std::string info = "Player pos: " + std::to_string(_turnData.npcInfoArray[0].q) + ' ' + std::to_string(_turnData.npcInfoArray[0].r);
	BOT_LOGIC_LOG(mLogger, info, true);*/

	_orders.clear();
	grid.reserved.clear();
	for (int i = 0; i < _turnData.npcInfoArraySize; ++i) {	
		
		if (omniscient) {
			//if the npc is not done
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
		}
		else {
			BOT_LOGIC_LOG(mLogger, "GetTurnOrders", true);
			grid.newGoal = false;
			behaviourTree.run(_orders, i, _turnData, mLogger);
		}
	}


}