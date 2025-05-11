#pragma once

#include "network.hpp"
#include "videodata.hpp"
#include <vector>
#include <unordered_set>

class Simulation {
private:
	Network& net;
	double curTime;  // current running time
    double totalTime = 100.0;  // total running time
    int blockSeq;  // globally unique integer sequence number, start from 1.

    std::vector<InTransitPacket> transmissionQueue;
    std::vector<std::unordered_set<int>> requestedBlocks;

    void gen1SecData(int startTime);  // The server produces video data of 30 KByte per second, divided into 1 KByte size blocks.
public:
	Simulation(Network& network);

	void run(double maxTime);
	void printResults();
};

struct InTransitPacket {
    int fromId;
    int toId;
    DataBlock block;
    int deliveryTime;
}
