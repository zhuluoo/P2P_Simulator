#pragma once

#include "network.hpp"
#include <QWidget>
#include <QTimer>
#include <vector>
#include <QKeyEvent>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialog>

struct GuiNode {
    int id;
    double x,y;
};

struct AnimatedPacket {
    int seq;
    int fromId;
    int toId;
    double startTime;
    double deliveryTime;
};

class NodeCanvas : public QWidget {
    Q_OBJECT

public:
    explicit NodeCanvas(QWidget* parent = nullptr);
    ~NodeCanvas();
    void setNet(const Network& net);  // get nodes and adjacency matrix
    void addPacket(int seq, int fromId, int toId, double startTime, double deliveryTime);
    void startVisualization(double totalTime = 100.0, int slowFactor = 100);

    void showDelayDialog();

public slots:
    void togglePause();  // press Spacebar to pause and continue
    void zoomIn();  // press + or = to zoom in 
    void zoomOut();  // press - to zoom out 

protected:
    void paintEvent(QPaintEvent* event) override;  // paint function

    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void updateAnimation();  // move active packets forward

private:
    const Network* net_ = nullptr;
    std::vector<GuiNode> nodes;
    std::vector<std::vector<double>> matrix; // adjaceney matrix
    std::vector<AnimatedPacket> allPackets;  // store transmission packet

    QTimer* animationTimer = nullptr;
    double curTime;
    double totalTime;

    int slowFactor; // slow done speed factor
    double scale;  // zoom in and out scale
    QPointF panOffset = {0, 0}; // shift in logical space
};

// a launch dialog window
class SimControlWindow : public QWidget {
    Q_OBJECT

public:
    SimControlWindow(QWidget* parent = nullptr);

signals:
    void simulationStarted(int numClients, int numNeighbors, int cacheSize);

private:
    QSpinBox *clientSpin, *neighborSpin, *cacheSpin;
    QPushButton *startBtn;
};
