#pragma once
#include "Task.h"
class Move_to_next : public Task
{

public:
	status run(std::list<SOrder>&, int, const STurnData&) override;
};

