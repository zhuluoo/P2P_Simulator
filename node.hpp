#pragma once

#include<vector>
#include<string>

class Node {
protected:
	int id;
	double x, y;   //coordinate
	std::vector<int> neighbors;  // store neighbors' id
public:
	Node(int,double,double);   //configuration
	virtual ~Node() = default;
    
    virtual void addNeighbor(int) = 0;
    virtual int getid() const = 0;
    virtual double getx() const = 0;
    virtual double gety() const = 0;
    virtual size_t getNumNeighbor() const = 0;
};

class Server: public Node {
public:
	Server(int,double,double);   //configuration

    void addNeighbor(int) override;
    int getid() const override;
    double getx() const override;
    double gety() const override;
    size_t getNumNeighbor() const override;
};	

class Client : public Node {
public:
	Client(int,double,double);   //configuration

    void addNeighbor(int) override;
    int getid() const override;
    double getx() const override;
    double gety() const override;
    size_t getNumNeighbor() const override;
};	
