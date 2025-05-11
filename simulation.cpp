#include "simulation.hpp"

Simulation:: Simulation(Network& n) : net(n) {
    curTime = 0.0;
    blockSeq = 0;
    requestedBlocks.resize(n.nodes.size() - 1);
}

void Simulation:: run(double maxTime) {
    auto cmp = [](const Event& a, const Event& b) {
        return a.time < b.time;
    }
    std::priority_queue<Event , std::vector<Event> , decltype(cmp) > eventQueue(cmp);  // process event in sequence of time

// Server produces blocks
// Clients request blocks from neighbors
// Clients receive blocks (with delay)
// Clients try to play
}

void Simulation:: gen1SecData(int startTime) {
    for (int i = 0; i < 30; ++i) {
        DataBlock block = {blockSeq++, startTime + i / 30.0};
        net.server.recvBlock(block);
    }
}
