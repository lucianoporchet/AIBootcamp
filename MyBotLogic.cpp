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

//#include <chrono>
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
//nanoseconds result;
void MyBotLogic::Init(const SInitData& _initData)
{
	
	//BOT_LOGIC_LOG(mLogger, "Init", true);
	//auto t = high_resolution_clock::now();
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
		grid.newGoal = false;
		behaviourTree.run(_orders, i, _turnData, mLogger);
	}
}