#pragma once
#include <vector>
#include <iterator>
#include "Grid.h"
#include "Globals.h"
#include "Logger.h"

enum class status
{
	SUCCESS, FAILURE, RUNNING
};

class Task {
public:
	virtual status run(std::list<SOrder>&, int, const STurnData&, Logger&) { return status::FAILURE; };
};