#include "network.hpp"
#include "simulation.hpp"
#include "visualization.hpp"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // implement Network
    Network n;

    // launch dialog window
    SimControlWindow setup;
    // viz sim canvas
    NodeCanvas canvas;

    QObject::connect(&setup, &SimControlWindow::simulationStarted,
        [&](int numClients, int numNeighbors, int cacheSize) {
            // set Network
            n.init(numClients, numNeighbors, cacheSize);
            canvas.setNet(n);

            canvas.show();
            
            QLabel* help = new QLabel("Space: Pause | + / -: Zoom | Arrows: Move D: Check client delay E: Node Exit");
            help->setStyleSheet("background-color: rgba(255,255,255,200); padding: 4px;");
            help->move(10, 100);
            help->setParent(&canvas);
            help->show();

            // simulate
            Simulation s(n, &canvas);
            s.run();

            canvas.startVisualization();
        });

    setup.show();

    return app.exec();
}
