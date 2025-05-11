#pragma once

#include <deque>

struct DataBlock {
	int seqNum;
	double genTime;
};

class Cache {
public:
	int cap;  //capacity
	std::deque<DataBlock> blocks;

	Cache(int cap);
	void addBlock(const DataBlock& block);
	bool hasConsecutives(int start, int count);
};
