#include "node.hpp"

Node::Node(int nid, double nx, double ny) : id(nid), x(nx), y(ny) {}

Server::Server(int nid, double nx, double ny) : Node(nid, nx, ny){}

Client::Client(int nid, double nx, double ny) : Node(nid, nx, ny) {}

void Server:: addNeighbor(int id) {
    neighbors.push_back(id);
}

void Client:: addNeighbor(int id) {
    neighbors.push_back(id);
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
