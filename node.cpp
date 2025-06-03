#include "node.hpp"

Node::Node(int nid, double nx, double ny, int bufSize) : id(nid), x(nx), y(ny), buffer(bufSize) {}

Server::Server(int nid, double nx, double ny, int bufSize) : Node(nid, nx, ny, bufSize) {}  // set Server's bufSize euqal to the data size it produces per second

Client::Client(int nid, double nx, double ny, int bufSize) : Node(nid, nx, ny, bufSize) {}


void Server:: restart(){
    buffer.blocks.clear();
}

void Client:: restart(){
    buffer.blocks.clear();
    nextPlaySeq = 0;
    neededSeq = 0;
    playedBlocks.clear();
    delay = 0;
}


void Server:: addNeighbor(int id) {
    neighbors.push_back(id);
}

void Client:: addNeighbor(int id) {
    neighbors.push_back(id);
}


void Server:: delNeighbor(int id) {
    if (id = -1) {
        neighbors.clear();
        return;
    }
    neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), id), neighbors.end());
}

void Client:: delNeighbor(int id) {
    if (id = -1) {
        neighbors.clear();
        return;
    }
    neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), id), neighbors.end());
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
        computeDelay();
        return true;
    }
    return false;
}

void Client:: computeDelay() {
    double sum = 0;
    for (int i = 0; i < playedBlocks.size(); ++i) {
        double expected = i / 30.0;
        sum += playedBlocks[i] - expected;
    }
    delay = sum / playedBlocks.size();
}

int Client:: getNeededSeq() {
    return neededSeq;
}

const std::vector<double>& Client::getPlayedBlk() const {
    return playedBlocks;
}
