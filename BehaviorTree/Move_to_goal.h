#pragma once
#include "Task.h"
#include "A_star.h"
class Move_to_goal : public Task
{
public:
	status run(std::list<SOrder>& _orders, int, const STurnData&, Logger&) override;
};

