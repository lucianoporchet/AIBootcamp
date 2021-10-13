#include "Selector.h"

status Selector::run(std::list<SOrder>& t, int i, const STurnData& turnInfo) {
	
	for (auto c : children) {
		if (c->run(t, i, turnInfo) == status::SUCCESS)
			return status::SUCCESS;
	}
	return status::FAILURE;
}