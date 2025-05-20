#include "simulation.hpp"

Simulation:: Simulation(Network& n) : net(n) {
    curTime = 0.0;
    blockSeq = 0;
    requestedBlocks.resize(n.nodes.size());
}

void Simulation:: run() {
    // system time setting
    const double timeStep = 0.01;  // use a time-stepped model, loop every 0.1 second
    curTime = 0.0;
    // tracking Server production
    const double blockInterval = 1.0 / 30.0;
    double nextBlockTime = 0.0;

    while (curTime < totalTime) {
        // Server produces blocks
        if (curTime >= nextBlockTime) {
            DataBlock block{blockSeq++ , curTime};
            net.nodes[0] -> recvBlock(block);
            nextBlockTime += blockInterval;
        }

        // Deliver packets
        for (auto it = transmissionQueue.begin(); it != transmissionQueue.end(); ) {
            // if delivery time comes, diliver block
            if(it->deliveryTime <= curTime) {
                net.nodes[it->toId].recvBlock(it->block);
                requestedBlocks[it->toId].erase(it->block.seqNum);
                it = transmissionQueue.erase(it);
                } else { ++it; }
        }

        // Clients request and play
        for (stze_t i = 1; i < net.nodes.size(); ++i) {
            int cid = net.nodes[i] -> getid(); 
            int needed = dynamic_cast<Client*>(net.nodes[i]) -> getNextPlaySeq();

            if (requestedBlocks[cid].count(needed) == 0 && !(dynamic_cast<Client*>(net.nodes[i]) -> hasConsec(needed, 1))) {
                int bestNeighbor = -1;
                double bestDelay = std::numeric_limits<double>::max();
                DataBlock targetBlock;

                for (int nid : net.nodes[i] -> getNeighbor()) {
                    Node * const neighbor = net.nodes[nid];
                    for (const DataBlock& blk : neighbor -> getBufferBlk()) {
                        if(blk.seqNum == needed) {
                            double rate = matrix[cid][nid];
                            double delay = 1.0 / rate;
                            if (delay < bestDelay) {
                                bestNeighbor = nid;
                                bestDelay = delay;
                                targetBlock = blk;
                            }
                            break;
                        }
                    }
                }
                if (bestNeighbor != -1) {
                    transmissionQueue.push_back({bestNeighbor, cid, targetBlock, curTime + bestDelay});
                    requestedBlocks[cid].insert(needed);
                }
            }

            dynamic_cast<Client*>(net.nodes[i]) -> tryplay(curTime);
        }

        // time is ticking
        curTime += timeStep;
    }
}
