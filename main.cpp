#include "network.hpp"
#include "simulation.hpp"
#include "visualization.hpp"

#include <QApplication>

int main(int argc, char* argv[]) {
    // set network
    Network n;
    n.init(100, 5, 10);

    // simulate
    Simulation s(n);
    s.run();
    
    // qt visualization
    QApplication app(argc, argv);
    
    NodeCanvas canvas;
    canvas.setNet(n);
    canvas.show();

    canvas.startPacket(0 , 3);

    return app.exec();
}
