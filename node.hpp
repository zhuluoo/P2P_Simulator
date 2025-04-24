#pragma once

#include<vector>
#include<string>

class Node {
public:
	int id;
	double x, y;   //coordinate
	std::vector<int> neighbors;  // store neighbors' id

	Node(int,double,double);   //configuration
	virtual ~Node() = default;
};

class Server: private Node {
public:
	Server(int,double,double);   //configuration

    void addNeighbor(int);
    int getid();
    int getx();
    int gety();
};	

class Client : private Node {
public:
	Client(int,double,double);   //configuration

    void addNeighbor(int);
    int getid();
    int getx();
    int gety();
};	
