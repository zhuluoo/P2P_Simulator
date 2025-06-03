#pragma once

#include "node.hpp" 
#include <cmath>

class Network {
public:
	Server* server; // pointer to point server node
	std::vector<Node *> nodes; // list stores all server and cllients node
	std::vector<std::vector<double>> matrix; // adjacency matrix, store rate

    int numServer = 1;
    int numClient = 0;  // default value, it should be >= 100
    int numNeighbor = 0;  // default value, in real case, it's max number of neighbors actually

    Network(){};
    ~Network();
	void init(int numClient, int numNeighbor, int bufSize);  //initialization
	double distance(const Node* const a, const Node* const b); //compute distance between nodes

    void nodeExit(int cid);
};
