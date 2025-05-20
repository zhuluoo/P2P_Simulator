#include "network.hpp"

int main(){
    Network n;
    n.init(100, 10);

    n.testPrintMatrix();
    n.testPrintNumNeighbor();
    n.isGraphConnected();
return 0;
}
