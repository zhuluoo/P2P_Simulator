#include "network.hpp"
#include <random>

void Network:: init(int numC, int numN) {
    numClient = numC;
    numNeighbor = numN;

    int netBound = numC * 10;  // set biggest coordinates
    std::random_device rd;
    std::mt19937 gen(rd());
    // gives an even, random spread across the 2D space. and random neighbor id
    set::uniform_real_distribution<> intDist(0, netBound)  // simplify the random generation to be int, and the coordinate range is 0 to netBound
    set::uniform_real_distribution<> nid(0, numNeighbor)

    std::vector<vector<bool>> check(netBound , vector<bool>(netBound , 0);  // avoid duplicates of coordinates

    double x = static_cast<double>(intDist(gen));
    double y = static_cast<double>(intDist(gen));
    check[x][y] = 1;

    nodes.emplace_back(0, x, y);  // server node, id 0 
    server = nodes[0];

    for(int i = 1; i <= numC; ++i) {
        double x, y;
        do {
            x = static_cast<double>(intDist(gen));
            y = static_cast<double>(intDist(gen));
        } while (check[x][y] == 1);
        check[x][y] = 1;

        nodes.emplace_back(i, x, y); // initialize vector storing clients, let i be client's id, x and y just be random value
        
        // set neighborsintDist
        vector<bool> checkn(numNeighbor , 0);
        for(int i = 0; i < numNeighbor; ++i) {
            int neighborId;
            do {  neighborId = nid(gen()); } while (checkn[neighborId] == 1);
            checkn[neighborId] = 1;
            node[i].addneighbor(neighborId);
        }
    }
}
// Set neighbors for the server node (id 0)
    std::vector<bool> checkn_server(numC + 1, false); // Initialize a vector to avoid duplicate neighbor IDs for the server
    for (int i = 0; i < numNeighbor; ++i) {
        int neighborId;
        std::uniform_int_distribution<> nid(1, numC); // Neighbor ID 范围是 1 到 numC
        do {
            neighborId = nid(gen()); // 生成随机邻居ID
        } while (checkn_server[neighborId] == true);  //
        checkn_server[neighborId] = true;
        nodes[0].addneighbor(neighborId); // 添加到服务器 (id 0) 的邻居列表
    }

double Network:: distance(const Node& a, const Node& b) {
    double distance;
    distance = sqrt( pow( (a.getx() - b.getx()) , 2 ) + pow( (a.gety() - b.gety()) , 2) )
    return distance;
}
