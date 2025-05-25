#include "network.hpp"
#include "simulation.hpp"
#include "visualization.hpp"

#include <QApplication>

int main(int argc, char* argv[]) {
    Network n;
    n.init(100, 10, 10);

    Simulation s(n);
    s.run();
    

    QApplication app(argc, argv);
    
    NodeCanvas canvas;
    canvas.setNodes(n);
    canvas.show();


    return app.exec();
}
