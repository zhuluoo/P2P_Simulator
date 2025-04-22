#pragma once

#include "Node.hpp" 
#include <cmath>

class Network {
private:
	std::vector<Client> clients; // list stores all cllients node
	Server* server; // pointer to point server node
    int numServer = 1;
    int numClients = 0;  // default value, it should be >= 100
public:
	void init(int numClients, int numNeighbor);  //initialization
	double distance(const Node& a, const Node& b); //compute distance between nodes
};
