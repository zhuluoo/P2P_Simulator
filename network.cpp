#include "network.hpp"
#include <random>

void Network:: init(int numC, int numN, int bufSize) {
    numClient = numC;
    numNeighbor = numN;

    int netBound = numC * 10;  // set biggest coordinates
    std::random_device rd;
    std::mt19937 gen(rd());
    // simplify the random generation to be int, and the coordinate range is 0 to netBound
    std::uniform_int_distribution<> intDist(0, netBound);  // gives an even, random spread across the 2D space.
    std::uniform_int_distribution<> nid(0, numClient);  // random neighbor id

    std::vector<std::vector<int>> check(netBound + 1, std::vector<int>(netBound + 1, 0));  // avoid duplicates of coordinates

    // create server node
    double x = static_cast<double>(intDist(gen));
    double y = static_cast<double>(intDist(gen));
    check[x][y] = 1;
    server = new Server(0, x, y, bufSize);
    nodes.push_back(server);  // server node, id 0 

    // create client node
    for(int i = 1; i <= numC; ++i) {
        double x, y;
        do {
            x = static_cast<double>(intDist(gen));
            y = static_cast<double>(intDist(gen));
        } while (check[x][y] == 1);
        check[x][y] = 1;

        nodes.push_back(new Client(i, x, y, bufSize));  // create and push clients, let i be client's id, x and y just be random value
    }



    // set neighbors
    // initialize adjacency matrix
    for (int i = 0; i < numClient + 1; ++i) {
        matrix.push_back(std::vector<double>(numClient + 1 , 0));
    }
    // add neighbors
    for (int i = 0; i < numClient + 1; ++i) {
        std::vector<int> checkn(numClient + 1, 0);  // avoid duplicates
        int numAddNeighbor = numNeighbor - nodes[i]->getNumNeighbor();
        for (int j = 0; j < numAddNeighbor; ++j) {  // this is a undirected graph, so neighbor relation is mutual
            int neighborId;
            int guard = numClient * 100;  // avoid adding invalid neighbors
            do {
                neighborId = nid(gen);
                if (--guard == 0) {
                    // std::cerr << "[WARN] Node " << i << ": Cannot find a neighbor anymore!\n";
                    break;
                }
            } while (checkn[neighborId] == 1 || matrix[i][neighborId] != 0 || neighborId == i || nodes[neighborId]->getNumNeighbor() >= numNeighbor);

            if (guard == 0) break;
            checkn[neighborId] = 1;
            
            nodes[i]->addNeighbor(neighborId);
            nodes[neighborId]->addNeighbor(i);
            
            double d = distance(nodes[i] , nodes[neighborId]);
            double dMax = std::sqrt(2.0) * numClient * 10;
            double rate = 20.0 + (100.0 - 20.0) * (1.0 - std::min(d, dMax) / dMax);
            matrix[i][neighborId] = rate;
            matrix[neighborId][i] = rate;
        }
    }

}

double Network:: distance(const Node* const a, const Node* const b) {
    double distance;
    distance = sqrt( pow( (a->getx() - b->getx()) , 2 ) + pow( (a->gety() - b->gety()) , 2) );
    return distance;
}

void Network::nodeExit(int cid) {
    if (cid < 1 || cid >= nodes.size()) return;

    // get the exiting node's neighbors
    std::vector<int> oldNeighbors = nodes[cid]->getNeighbor();

    // disconnect the node form the neighbors
    for (const int nid : nodes[cid]->getNeighbor()) {
        nodes[nid]->delNeighbor(cid);
        matrix[cid][nid] = 0;
        matrix[nid][cid] = 0;
    }
    nodes[cid]->delNeighbor(-1);  // delete all neighbors

    // reconnect the former neighbors
    for (int nid : oldNeighbors) {
        while (nodes[nid]->getNumNeighbor() < numNeighbor) {
            int best = -1;
            double bestDist = std::numeric_limits<double>::max();

            for (int i = 1; i < nodes.size(); ++i) {
                if (i == nid || matrix[nid][i] != 0 || nodes[i]->getNumNeighbor() >= numNeighbor) continue;

                double d = distance(nodes[nid], nodes[i]);
                if (d < bestDist) {
                    best = i;
                    bestDist = d;
                }
            }
            
            if (best == -1) break;

            // add bidirectional connection
            nodes[nid]->addNeighbor(best);
            nodes[best]->addNeighbor(nid);

            double d = distance(nodes[nid] , nodes[best]);
            double dMax = std::sqrt(2.0) * numClient * 10;
            double rate = 20.0 + (100.0 - 20.0) * (1.0 - std::min(d, dMax) / dMax);
            matrix[nid][best] = d;
            matrix[best][nid] = d;
        }
    }
}

Network:: ~Network() {
    for (Node* ptr : nodes) {
        delete ptr;
    }
    nodes.clear();
}
