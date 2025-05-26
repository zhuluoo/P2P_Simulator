#include "visualization.hpp"
#include <QPainter>

NodeCanvas::NodeCanvas(QWidget* parent) : QWidget(parent) {
    setMinimumSize(1000, 1000);
}

void NodeCanvas::setNet(const Network& net) {
    for ( const Node* pn : net.nodes) {
        nodes.push_back({pn->getid(), pn->getx(), pn->gety()});
    }

    matrix = net.matrix;

    update();  // trigger rapaint
}

void NodeCanvas::startPacket(int from, int to) {
    activePackets.push_back({from, to, 0.0});
    if(!animationTimer) {
        animationTimer = new QTimer(this);
        connect(animationTimer, &QTimer::timeout, this, &NodeCanvas::updateAnimation);
        animationTimer->start(30);
    }
}

void NodeCanvas::updateAnimation() {
    bool anyRemaining = false;

    for(auto& pkt : activePackets) {
        pkt.progress += 0.02;
        if(pkt.progress < 1.0) anyRemaining = true;
    }

    activePackets.erase(
        std::remove_if(activePackets.begin(), activePackets.end(),
            [](const AnimatedPacket& pkt){ return pkt.progress >= 1.0;}),
        activePackets.end());

    if (!anyRemaining && animationTimer) {
        animationTimer->stop();
        delete animationTimer;
        animationTimer = nullptr;
    }

    update();
}

void NodeCanvas::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // draw nodes
    painter.setPen(Qt::black);
    painter.setBrush(Qt::cyan);
    for (const GuiNode& node : nodes) {
        int radius = 6;
        painter.setBrush(Qt::cyan);
        painter.drawEllipse(QPoint(static_cast<int>(node.x), static_cast<int>(node.y)), radius, radius);
        painter.drawText(node.x + 10, node.y - 10, QString::number(node.id));
    }

    // draw edges
    painter.setPen(QPen(Qt::gray, 1));
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] != 0) {
                painter.drawLine(nodes[i].x, nodes[i].y, nodes[j].x, nodes[j].y);
            }
        }
    }

    // draw animated packets
    painter.setBrush(Qt::red);
    for (const auto& pkt : activePackets) {
        const auto& a = nodes[pkt.fromId];
        const auto& b = nodes[pkt.toId];
        double x = a.x + (b.x - a.x) * pkt.progress;
        double y = a.y + (b.y - a.y) * pkt.progress;
        painter.drawEllipse(QPointF(x, y), 4, 4);
    }

}

NodeCanvas::~NodeCanvas(){}
