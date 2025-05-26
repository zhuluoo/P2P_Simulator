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
    double progress;
};

class NodeCanvas : public QWidget {
    Q_OBJECT
public:
    explicit NodeCanvas(QWidget* parent = nullptr);
    ~NodeCanvas();

    void setNet(const Network& net);

    void startPacket(int from, int to);
protected:
    void paintEvent(QPaintEvent* event) override;

    void updateAnimation();
private:
    std::vector<GuiNode> nodes;
    std::vector<std::vector<double>> matrix; // adjaceney matrix
    std::vector<AnimatedPacket> activePackets;  // store transmission packet
    QTimer* animationTimer = nullptr;
};
