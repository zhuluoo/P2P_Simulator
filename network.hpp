#pragma once

#include "node.hpp" 
#include <cmath>
#include <iostream>
#include <queue>
#include <algorithm>

class Network {
public:
	Server* server; // pointer to point server node
	std::vector<Node *> nodes; // list stores all server and cllients node
	std::vector<std::vector<double>> matrix; // adjacency matrix, store distance

    int numServer = 1;
    int numClient = 0;  // default value, it should be >= 100
    int numNeighbor = 0;  // default value, in real case, it's max number of neighbors actually
    void setNeighbors();
    Network(){};
	void init(int numClient, int numNeighbor);  //initialization
	double distance(const Node* const a, const Node* const b) const; //compute distance between nodes
    ~Network();

    void testPrintMatrix() {
        for (std::vector<double> m : matrix) { 
            for ( double d : m) {
                std::cout << d << " ";
            }
            std::cout << "\n";
        }
    }

    void testPrintNumNeighbor() {
        for ( Node * n : nodes) {
            std::cout << n->getNumNeighbor() << "\n";
        }
    }

    void dfs(int node, const std::vector<std::vector<double>>& matrix, std::vector<bool>& visited) {
        visited[node] = true;

        for (size_t i = 0; i < matrix[node].size(); ++i) {
            if (matrix[node][i] != 0 && !visited[i]) {
                dfs(i, matrix, visited);
            }
        }
    }

    void isGraphConnected() {
        int n = matrix.size();
        std::vector<bool> visited(n, false);

        // Start DFS from node 0
        dfs(0, matrix, visited);

        // Check if all nodes were visited
        for (bool v : visited) {
            if (!v) std::cout << "false" << "\n";
        }

        std::cout << "true" << "\n";
    }

};
