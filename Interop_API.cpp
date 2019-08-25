#include "stdafx.h"
#include "Interop_API.h"
#include "combaseapi.h"

#include "AIBot.h"
#include "ConfigData.h"
#include "InitData.h"
#include "TurnData.h"

#include <list>
#include <thread>
#include <future>

using namespace std::chrono_literals;

void config_wrapper(AIBot* _pAIBot, const SConfigData* _configData)
{
	_pAIBot->Configure(*_configData);
}

void init_wrapper(AIBot* _pAIBot, const SInitData* _initData)
{
	_pAIBot->Init(*_initData);
}

SOrder* getaibotturnorders_wrapper(AIBot* _pAIBot, const STurnData* _turnData, int* nbOrders)
{
	std::list<SOrder> orders;

	_pAIBot->GetTurnOrders(*_turnData, orders);

	*nbOrders = (int)orders.size();
	SOrder* orderArr = (SOrder*)CoTaskMemAlloc(sizeof(SOrder) * *nbOrders);

	int i = 0;
	for (SOrder& order : orders)
	{
		orderArr[i] = order;
		i++;
	}
	return orderArr;
}

extern "C"
{
	INTEROP_API AIBot* Internal_CreateNativeAIBot()
	{
		return new AIBot();
	}

	INTEROP_API void Internal_ConfigureAIBot(AIBot* _pAIBot, const SConfigData& _configData)
	{
		std::packaged_task<void(AIBot*, const SConfigData*)> task(config_wrapper);
		auto future = task.get_future();
		std::thread thr(std::move(task), _pAIBot, &_configData);
		if (future.wait_for(std::chrono::milliseconds(_configData.configDelay)) != std::future_status::timeout)
		{
			thr.join();
		}
		else
		{
			thr.detach(); // we leave the thread still running
		}
	}

	INTEROP_API void Internal_InitAIBot(AIBot* _pAIBot, const SInitData& _initData)
	{
		std::packaged_task<void(AIBot*, const SInitData*)> task(init_wrapper);
		auto future = task.get_future();
		std::thread thr(std::move(task), _pAIBot, &_initData);
		if (future.wait_for(std::chrono::milliseconds(_initData.initDelay)) != std::future_status::timeout)
		{
			thr.join();
		}
		else
		{
			thr.detach(); // we leave the thread still running
		}
	}

	INTEROP_API SOrder* Internal_GetAIBotTurnOrders(AIBot* _pAIBot, const STurnData& _turnData, int& nbOrders)
	{
		std::packaged_task<SOrder*(AIBot*, const STurnData*, int*)> task(getaibotturnorders_wrapper);
		auto future = task.get_future();
		std::thread thr(std::move(task), _pAIBot, &_turnData, &nbOrders);
		if (future.wait_for(std::chrono::milliseconds(_turnData.turnDelay)) != std::future_status::timeout)
		{
			thr.join();

			return future.get();
		}
		else
		{
			nbOrders = 0;
			thr.detach(); // we leave the thread still running
			return nullptr;
		}
	}
}

