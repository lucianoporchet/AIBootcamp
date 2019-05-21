#include "stdafx.h"
#include "Interop_API.h"
#include "combaseapi.h"

#include "AIBot.h"

#include <list>

extern "C"
{
	INTEROP_API AIBot* Internal_CreateNativeAIBot()
	{
		return new AIBot();
	}

	INTEROP_API void Internal_ConfigureAIBot(AIBot* _pAIBot, const SConfigData& _configData)
	{
		_pAIBot->Configure(_configData);
	}

	INTEROP_API void Internal_InitAIBot(AIBot* _pAIBot, const SInitData& _initData)
	{
		_pAIBot->Init(_initData);
	}

	INTEROP_API SMoveOrder* Internal_GetAIBotTurnOrders(AIBot* _pAIBot, const STurnData& _turnData, int& nbOrders)
	{
		std::list<SMoveOrder> orders;
		
		_pAIBot->GetTurnOrders(_turnData, orders);

		nbOrders = (int)orders.size();
		SMoveOrder* orderArr = (SMoveOrder*)CoTaskMemAlloc(sizeof(SMoveOrder) * nbOrders);

		int i = 0;
		for (SMoveOrder& order : orders) 
		{
			orderArr[i] = order;
			i++;
		}
		return orderArr;
	}
}