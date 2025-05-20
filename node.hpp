#pragma once

#include<vector>
#include<string>
#include "videpdata.hpp"

class Node {
protected:
	int id;
	double x, y;   //coordinate
	std::vector<int> neighbors;  // store neighbors' id
    Cache buffer;  // cache
public:
	Node(int id, double x, double y, int bufSize);   //configuration
	virtual ~Node() = default;
    
    virtual void addNeighbor(int) = 0;
    virtual void recvBlock(const DataBlock&) = 0;

    virtual int getid() const = 0;
    virtual double getx() const = 0;
    virtual double gety() const = 0;

    virtual size_t getNumNeighbor() const = 0;
    virtual const std::vector<int>& getNeighbor() const = 0;
    virtual const std::deque<DataBlock>& getBufferBlk() const = 0;
};

class Server: public Node {
public:
	Server(int id, double x, double y, int bufSize);   //configuration

    void addNeighbor(int) override;
    void recvBlock(const DataBlock&) override;

    int getid() const override;
    double getx() const override;
    double gety() const override;
    
    size_t getNumNeighbor() const override;
    const std::vector<int>& getNeighbor() const override;
    const std::deque<DataBlock>& getBufferBlk() const override;
}

class Client : public Node {
private:
    int nextPlaySeq = 0;
    std::vector<double> playedBlocks;  // for computing delay
public:
	Client(int id, double x, double y, int bufSize);   //configuration

    void addNeighbor(int) override;
    void recvBlock(const DataBlock&) override;

    int getid() const override;
    double getx() const override;
    double gety() const override;

    size_t getNumNeighbor() const override;
    const std::vector<int>& getNeighbor() const override;
    const std::deque<DataBlock>& getBufferBlk() const override;

    // Client's unique functions
    bool hasConsec(int start, int count);
    bool tryplay(double curTime);
    int getNextPlaySeq();
};	
