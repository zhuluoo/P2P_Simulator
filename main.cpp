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
            
            QLabel* help = new QLabel("快捷键: Space: 暂停  +/-: 缩放  方向键: 移动  D: 查看延迟  E: 节点退出");
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
