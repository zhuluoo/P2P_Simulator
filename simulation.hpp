#pragma once

#include "network.hpp"
#include <vector>
#include <unordered_set>
#include <cmath>
#include <iostream>
#include <algorithm>

struct InTransitPacket {
    int fromId;
    int toId;
    DataBlock block;
    double deliveryTime;
};

class Simulation {
private:
	Network& net;
	double curTime;  // current running time
    double totalTime;  // total running time
    int blockSeq;  // globally unique integer sequence number, start from 0.

    std::vector<InTransitPacket> transmissionQueue;
    std::vector<std::unordered_set<int>> requestedBlocks;
public:
	Simulation(Network& network);

	void run(double totalT = 100.0);
	void printResults();
};
