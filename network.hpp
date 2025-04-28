#pragma once

#include "node.hpp" 
#include <cmath>

class Network {
private:
	Server* server; // pointer to point server node
	std::vector<Node *> nodes; // list stores all server and cllients node
	std::vector<std::vector<double>> matrix; // adjacency matrix, store distance

    int numServer = 1;
    int numClient = 0;  // default value, it should be >= 100
    int numNeighbor = 0;  // default value, in real case, it's max number of neighbors actually
    void setNeighbors();
public:
    Network(){};
	void init(int numClient, int numNeighbor);  //initialization
	double distance(const Node* const a, const Node* const b) const; //compute distance between nodes
    ~Network();
};
