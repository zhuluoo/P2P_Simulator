#include "network.hpp"
#include "simulation.hpp"

int main(){
    Network n;
    n.init(100, 10, 10);

    Simulation s(n);
    s.run();
return 0;
}
