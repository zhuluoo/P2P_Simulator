#include "network.hpp"

void Network:: init(int numC, int numN) {
    numClients = numC;
   for(int i = 0; i < numC; ++i) {
// ----------------------------CAUTION! Randon value of x and y are not assigned!! ------------------------------------------------------
       clients.push_back( new Client(i, x, y) ); // initialize vector storing clients, let i be client's id, x and y just be random value
// ----------------------------neighbor setting ubdone!! ------------------------------------------------------
   }

double Network:: distance(const Node& a, const Node& b) {
    double distance;
    distance = sqrt( pow( (a.getx() - b.getx()) , 2 ) + pow( (a.gety() - b.gety()) , 2) )
    return distance;
}
