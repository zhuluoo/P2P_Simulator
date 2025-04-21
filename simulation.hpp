#pragma once

#include "network.hpp"
#include "videodata.hpp"

class Simulation {
public:
	Simulation(Network& network); //conf
	void run();
	void printResults();
private:
	Network& net;
	double curTime;
};
