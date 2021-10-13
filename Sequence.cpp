#include "Sequence.h"

status Sequence::run(std::list<SOrder>& t, int i, const STurnData& turnInfo) {
	for (auto c : children) {
		if (c->run(t, i, turnInfo) == status::FAILURE) {
			return status::FAILURE;
		}
	}
	return status::SUCCESS;
}

