#include "network.hpp"
#include <random>


//Utility: Euclidean distance 
double Network::distance(const Node* a, const Node* b) const
{
    double dx = a->getx() - b->getx();
    double dy = a->gety() - b->gety();
    return std::sqrt(dx * dx + dy * dy);
}

void Network:: init(int numC, int numN) {
    numClient = numC;
    numNeighbor = numN;

    for (Node* ptr : nodes) delete ptr;
    nodes.clear();
    matrix.clear();

    int netBound = numClient * 10;   // set biggest coordinates
    std::random_device rd;
    std::mt19937 gen(rd());
    // simplify the random generation to be int, and the coordinate range is 0 to netBound
    std::uniform_int_distribution<> posDist(0, netBound);   // for coordinates
    std::uniform_int_distribution<> nid(0, numClient);  // random neighbor id

    // occupancy table to avoid duplicate positions 
    std::vector<std::vector<char>> occupied(netBound + 1, std::vector<char>(netBound + 1, 0));
   
    // create server node
    double x,y;
    do { x = posDist(gen); y = posDist(gen); } while (occupied[x][y]);
    occupied[x][y] = 1;
    
    server = new Server(0, static_cast<double>(x), static_cast<double>(y));
    nodes.push_back(server);  // server node, id 0 

    // create client node
    for(int i = 1; i <= numClient; ++i) {
        double x, y;
        do {
           x = posDist(gen); y = posDist(gen); 
        } while (occupied[x][y]);
       occupied[x][y] = 1;

        nodes.push_back(new Client(i, static_cast<double>(cx), static_cast<double>(cy)));  // create and push clients, let i be client's id, x and y just be random value
    }
    setNeighbors();
}

    // set neighbors
void Network::setNeighbors()
{
   matrix.assign(numClient + 1,std::vector<double>(numClient + 1, 0.0));
 
    //random undirected edges (degree ≤ numNeighbor) 
    std::vector<int> idList(numClient + 1);
    std::iota(idList.begin(), idList.end(), 0);

 std::random_device rd; std::mt19937 gen(rd());

     auto addEdge = [&](int u, int v)
    {
        if (matrix[u][v] != 0)                                return; // already linked
        if (nodes[u]->getNumNeighbor() >= numNeighbor)        return;
        if (nodes[v]->getNumNeighbor() >= numNeighbor)        return;

        // distance → bandwidth : 20 KB/s … 100 KB/s (linear inverse) 
        double d    = distance(nodes[u], nodes[v]);
        double dMax = std::sqrt(2.0) * numClient * 10;           // diagonal of area
        double rate = 20.0 + (100.0 - 20.0) *
                      (1.0 - std::min(d, dMax) / dMax);

        matrix[u][v] = matrix[v][u] = rate;
        nodes[u]->addNeighbor(v);
        nodes[v]->addNeighbor(u);
    };

    // two sweeps are usually enough to fill the degree quotas 
    for (int sweep = 0; sweep < 2; ++sweep)
        for (int u = 0; u <= numClient; ++u) {
            std::shuffle(idList.begin(), idList.end(), gen);
            for (int v : idList) {
                if (u == v) continue;
                if (nodes[u]->getNumNeighbor() >= numNeighbor) break;
                addEdge(u, v);
            }
        }

    //  ensure every node can reach the server (connectivity check) 
    std::vector<char> vis(numClient + 1, 0);
    std::queue<int>   q;
    q.push(0); vis[0] = 1;                        // BFS from server
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : nodes[u]->neighbors)
            if (!vis[v]) { vis[v] = 1; q.push(v); }
    }

    //spare nodes: already connected to server and still have free degree 
    std::vector<int> spare;
    for (int i = 0; i <= numClient; ++i)
        if (vis[i] && nodes[i]->getNumNeighbor() < numNeighbor)
            spare.push_back(i);

    for (int u = 1; u <= numClient; ++u)          // skip server itself
        if (!vis[u]) {
            int v = spare.empty() ? 0 : spare.back(); // fallback: link directly to server
            if (!spare.empty()) spare.pop_back();
            addEdge(u, v);
            vis[u] = 1;                           // now reachable
        }
}

Network:: ~Network() {
    for (Node* ptr : nodes) {
        delete ptr;
    }
    nodes.clear();
}
