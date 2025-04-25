#include "network.hpp"
#include <random>
#include <iostream>

void Network:: init(int numC, int numN) {
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
    server = new Server(0, x, y);
    nodes.push_back(server);  // server node, id 0 

    // create client node
    for(int i = 1; i <= numC; ++i) {
        double x, y;
        do {
            x = static_cast<double>(intDist(gen));
            y = static_cast<double>(intDist(gen));
        } while (check[x][y] == 1);
        check[x][y] = 1;

        nodes.push_back(new Client(i, x, y));  // create and push clients, let i be client's id, x and y just be random value
    }



    // set neighbors
    // initialize adjacency matrix
    for (int i = 0; i < numClient + 1; ++i) {
        matrix.push_back(std::vector<double>(numClient + 1 , 0));
    }
    // add neighbors
    for (int i = 0; i < numClient + 1; ++i) {
        std::vector<int> checkn(numClient + 1, 0);  // avoid duplicates
        for (int j = 0; j < numNeighbor - nodes[i]->getNumNeighbor(); ++j) {  // this is a undirected graph, so neighbor relation is mutual
            int neighborId;
            do { neighborId = nid(gen); } while (checkn[neighborId] == 1 || matrix[i][neighborId] != 0);
            checkn[neighborId] = 1;
            
            nodes[i]->addNeighbor(neighborId);
            nodes[neighborId]->addNeighbor(i);
            
            matrix[i][neighborId] = distance(nodes[i] , nodes[neighborId]);
            matrix[neighborId][i] = distance(nodes[i] , nodes[neighborId]);
        }
    }

}

double Network:: distance(const Node* const a, const Node* const b) {
    double distance;
    distance = sqrt( pow( (a->getx() - b->getx()) , 2 ) + pow( (a->gety() - b->gety()) , 2) );
    return distance;
}

Network:: ~Network() {
    for (Node* ptr : nodes) {
        delete ptr;
    }
    nodes.clear();
}
