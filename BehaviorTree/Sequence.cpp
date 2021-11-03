#include "Sequence.h"

status Sequence::run(std::list<SOrder>& t, int i, const STurnData& turnInfo, Logger& l) {
	for (auto c : children) {
		if (c->run(t, i, turnInfo, l) == status::FAILURE) {
			return status::FAILURE;
		}
	}
	return status::SUCCESS;
}

