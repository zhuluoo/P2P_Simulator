#pragma once

#include "network.hpp"
#include <QWidget>
#include <QTimer>
#include <vector>

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
    void startVisualization(double totalTime = 100.0);

protected:
    void paintEvent(QPaintEvent* event) override;  // paint function

private slots:
    void updateAnimation();  // move active packets forward

private:
    std::vector<GuiNode> nodes;
    std::vector<std::vector<double>> matrix; // adjaceney matrix
    std::vector<AnimatedPacket> allPackets;  // store transmission packet
    QTimer* animationTimer = nullptr;
    double curTime;
    double totalTime;
};
