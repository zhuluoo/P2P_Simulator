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
    int fromId;
    int toId;
    double progress;  // from 0.0 to 1.0
};

class NodeCanvas : public QWidget {
    Q_OBJECT
public:
    explicit NodeCanvas(QWidget* parent = nullptr);
    ~NodeCanvas();

    void setNet(const Network& net);  // get nodes and adjacency matrix

    void startPacket(int from, int to, int seq);  //Starts an animated packet from node from to node to
protected:
    void paintEvent(QPaintEvent* event) override;  // paint function

    void updateAnimation();  // move active packets forward
private:
    std::vector<GuiNode> nodes;
    std::vector<std::vector<double>> matrix; // adjaceney matrix
    std::vector<AnimatedPacket> activePackets;  // store transmission packet
    QTimer* animationTimer = nullptr;
};
