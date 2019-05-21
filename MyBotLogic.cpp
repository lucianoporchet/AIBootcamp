#include "MyBotLogic.h"

#include "Globals.h"
#include "ConfigData.h"
#include "InitData.h"
#include "TurnData.h"

MyBotLogic::MyBotLogic()
{
	//Write Code Here
}

/*virtual*/ MyBotLogic::~MyBotLogic()
{
	//Write Code Here
}

/*virtual*/ void MyBotLogic::Configure(const SConfigData& _configData)
{
#ifdef BOT_LOGIC_DEBUG
	mLogger.Init(_configData.logpath, "MyBotLogic.log");
#endif

	BOT_LOGIC_LOG(mLogger, "Configure", true);


	//Write Code Here
}

/*virtual*/ void MyBotLogic::Init(const SInitData& _initData)
{
	BOT_LOGIC_LOG(mLogger, "Init", true);
	
	//Write Code Here
}

/*virtual*/ void MyBotLogic::GetTurnOrders(const STurnData& _turnData, std::list<SMoveOrder>& _orders)
{
	BOT_LOGIC_LOG(mLogger, "GetTurnOrders", true);
}