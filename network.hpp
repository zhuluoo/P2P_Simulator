#pragma once

#include "Node.hpp" 
#include <cmath>

class Network {
private:
	std::vector<Client> nodes; // list stores all server and cllients node
	Server* server; // pointer to point server node

    int numServer = 1;
    int numClient = 0;  // default value, it should be >= 100
    int numNeighbor = 0;  // default value
public:
	void init(int numClients, int numNeighbor);  //initialization
	double distance(const Node& a, const Node& b); //compute distance between nodes
};
