#include "visualization.hpp"
#include <QPainter>

NodeCanvas::NodeCanvas(QWidget* parent) : QWidget(parent) {
    setMinimumSize(800, 600);
}

void NodeCanvas::setNodes(const Network& net) {
    for ( const Node* pn : net.nodes) {
        nodes.push_back({pn->getid(), pn->getx(), pn->gety()});
    }

    update();  // trigger rapaint
}

void NodeCanvas::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // draw each node
    for (const GuiNode& node : nodes) {
        int radius = 0;
        painter.setBrush(Qt::cyan);
        painter.drawEllipse(QPoint(static_cast<int>(node.x), static_cast<int>(node.y)), radius, radius);
        painter.drawText(node.x + 10, node.y - 10, QString::number(node.id));
    }
}

NodeCanvas::~NodeCanvas(){}
