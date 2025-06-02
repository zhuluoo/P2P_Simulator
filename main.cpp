#include "network.hpp"
#include "simulation.hpp"
#include "visualization.hpp"

#include <QApplication>

int main(int argc, char* argv[]) {
    // set network
    Network n;
    n.init(100, 5, 10);

    
    // qt visualization
    QApplication app(argc, argv);
    
    NodeCanvas canvas;
    canvas.setNet(n);
    canvas.show();

    // simulate
    Simulation s(n, &canvas);
    s.run();

    canvas.startVisualization();

    return app.exec();
}
