#pragma once
#include "Task.h"
#include "Grid.h"
#include "A_star.h"

class A_star_to_goal : public Task
{

public:
	status run(std::list<SOrder>&, int, const STurnData&, Logger&) override;
};

