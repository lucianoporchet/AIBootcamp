#pragma once
#include "Task.h"
#include "Grid.h"
class UpdateGrid : public Task
{
public:
	status run(std::list<SOrder>&, int, const STurnData& turnInfo) override;
};

