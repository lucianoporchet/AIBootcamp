#include "Selector.h"

status Selector::run(std::list<SOrder>& t, int i, const STurnData& turnInfo, Logger& l) {
	for (auto c : children) {
		if (c->run(t, i, turnInfo, l) == status::SUCCESS)
			return status::SUCCESS;
	}
	return status::FAILURE;
}