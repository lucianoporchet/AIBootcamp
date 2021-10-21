#pragma once
#include "Task.h"
class HasPath : public Task
{
public:
	status run(std::list<SOrder>&, int, const STurnData&, Logger&) override;
};

