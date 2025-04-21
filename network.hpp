#pragma once

#include "Node.hpp" 

class Network {
public:
	std::vector<Client> clients;
	Server* server;

	void init(int numClients, int t);  //initialization
	double distance(const Node& a, const Node& b); //compute distance between nodes
};
