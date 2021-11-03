#pragma once
#include "Task.h"
#include "A_star.h"
class Move_to_next : public Task
{

public:
	status run(std::list<SOrder>&, int, const STurnData&, Logger&) override;
};

