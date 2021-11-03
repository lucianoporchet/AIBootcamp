#include "MyBotLogic.h"

#include "Globals.h"
#include "ConfigData.h"
#include "InitData.h"
#include "TurnData.h"

#include "Grid.h"
#include "A_star.h"
#include "BehaviorTree/Selector.h"
#include "BehaviorTree/Sequence.h"
#include "BehaviorTree/A_star_to_goal.h"
#include "BehaviorTree/UpdateGrid.h"
#include "BehaviorTree/NewGoalVisible.h"
#include "BehaviorTree/HasPath.h"
#include "BehaviorTree/Move_to_goal.h"
#include "BehaviorTree/Move_to_next.h"



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



void MyBotLogic::Init(const SInitData& _initData)
{
	
	grid.InitGrid(_initData);
	behaviourTree = Selector(
						new Sequence(
							new UpdateGrid(),
							new HasPath(),
							new Move_to_goal()
						), 
						new Sequence(
							new NewGoalVisible(), 
							new A_star_to_goal(), 
							new Move_to_goal() 
						),
						new Move_to_next()
					);
}



void MyBotLogic::GetTurnOrders(const STurnData& _turnData, std::list<SOrder>& _orders)
{
	_orders.clear();
	grid.reserved.clear();
	for (int i = 0; i < _turnData.npcInfoArraySize; ++i) {	
		grid.newGoal = false;
		behaviourTree.run(_orders, i, _turnData, mLogger);
	}
}