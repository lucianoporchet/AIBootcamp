#pragma once
#include "Task.h"
#include "Grid.h"


class NewGoalVisible : public Task
{
public:
	status run(std::list<SOrder>&, int, const STurnData&, Logger&) override;
};

