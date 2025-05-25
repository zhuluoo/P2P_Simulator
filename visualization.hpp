#pragma once

#include "network.hpp"
#include <QWidget>
#include <vector>

struct GuiNode {
    int id;
    double x,y;
};

class NodeCanvas : public QWidget {
    Q_OBJECT
public:
    explicit NodeCanvas(QWidget* parent = nullptr);
    ~NodeCanvas();
    void setNodes(const Network& net);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    std::vector<GuiNode> nodes;
};
