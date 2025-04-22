#include "node.hpp"

Node::Node(int nid, double nx, double ny) : id(nid), x(nx), y(ny) {}

Server::Server(int nid, double nx, double ny) : Node(nid, nx, ny){}

Client::Client(int nid, double nx, double ny) : Node(nid, nx, ny) {}

int Server:: getid() {
    return Node.id;
}

int Client:: getid() {
    return Node.id;
}

double Server:: getx() {
    return Node.x;
}

double Client:: gety() {
    return Node.y;
}

double Server:: getx() {
    return Node.x;
}

double Client:: gety() {
    return Node.y;
}
