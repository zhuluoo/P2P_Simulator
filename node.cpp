#include "node.hpp"

Node::Node(int nid, double nx, double ny, int bufSize) : id(nid), x(nx), y(ny), buffer(bufSize) {}

Server::Server(int nid, double nx, double ny, int bufSize) : Node(nid, nx, ny, bufSize) {}  // set Server's bufSize euqal to the data size it produces per second

Client::Client(int nid, double nx, double ny, int bufSize) : Node(nid, nx, ny, bufSize) {}


void Server:: addNeighbor(int id) {
    neighbors.push_back(id);
}

void Client:: addNeighbor(int id) {
    neighbors.push_back(id);
}


void Server:: recvBlock(const DataBlock& block) {
    buffer.addBlock(block);
}

void Client:: recvBlock(const DataBlock& block) {
    buffer.addBlock(block);
    ++neededSeq;
}


int Server:: getid() const {
    return id;
}

int Client:: getid() const {
    return id;
}

double Server:: getx() const {
    return x;
}

double Client:: getx() const {
    return x;
}

double Server:: gety() const {
    return y;
}

double Client:: gety() const {
    return y;
}

size_t Server:: getNumNeighbor() const {
    return neighbors.size();
}

size_t Client:: getNumNeighbor() const {
    return neighbors.size();
}

const std::vector<int>& Server:: getNeighbor() const {
    return neighbors;
}

const std::vector<int>& Client:: getNeighbor() const {
    return neighbors;
}

const std::deque<DataBlock>& Server:: getBufferBlk() const {
    return buffer.blocks;
}

const std::deque<DataBlock>& Client:: getBufferBlk() const {
    return buffer.blocks;
}

bool Client:: hasConsec(int start, int count) {
    return buffer.hasConsecutives(start, count);
}

bool Client:: tryplay(double curTime) {
    if (buffer.hasConsecutives(nextPlaySeq, 5)) {
        playedBlocks.push_back(curTime);
        ++nextPlaySeq;
        return true;
    }
    return false;
}

int Client:: getNeededSeq() {
    return neededSeq;
}
