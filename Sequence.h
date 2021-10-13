#pragma once
#include "Task.h"
class Sequence : public Task
{
	std::vector<Task*> children;

public:
	status run(std::list<SOrder>&, int, const STurnData&) override;

	template<class ... Ts>
	Sequence(Ts ... tasks) : children{ tasks... } {}
	
};

