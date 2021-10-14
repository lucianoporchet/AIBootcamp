#pragma once
#include "Task.h"

class Selector : public Task
{
	std::vector<Task*> children;
public:
	status run(std::list<SOrder>&, int, const STurnData&, Logger&) override;
	template<class ... Ts>
	Selector(Ts ... tasks) : children{ tasks... } {}

};

