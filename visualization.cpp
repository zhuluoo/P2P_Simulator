#include "visualization.hpp"
#include <QPainter>

NodeCanvas::NodeCanvas(QWidget* parent) : QWidget(parent) {
    setMinimumSize(1000, 1000);
}

void NodeCanvas::setNet(const Network& net) {
    nodes.clear();
    matrix.clear();
    for ( const Node* pn : net.nodes) {
        nodes.push_back({pn->getid(), pn->getx(), pn->gety()});
    }

    matrix = net.matrix;

    update();  // trigger rapaint
}

void NodeCanvas::addPacket(int s, int f, int t, double st, double dt) {
    allPackets.push_back({s,f,t,st,dt});
}

void NodeCanvas::startVisualization(double tT) {
    curTime = 0.0;
    totalTime = tT;
    if (!animationTimer) {
        animationTimer = new QTimer(this);
        connect(animationTimer, &QTimer::timeout, this, &NodeCanvas::updateAnimation);
    }
    animationTimer->start(30); // fresh frequency: 30ms
}

void NodeCanvas::updateAnimation() {
    curTime += 0.3; // correspond to the timer interval
    update();
    
    // stop when reach totalTime
    if (curTime >= totalTime && animationTimer) {
        animationTimer->stop();
        delete animationTimer;
        animationTimer = nullptr;
    }
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

    // draw Server
    painter.setBrush(Qt::magenta);
    painter.drawEllipse(QPoint(static_cast<int>(nodes[0].x), static_cast<int>(nodes[0].y)), 10, 10);

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
    for (const AnimatedPacket& pkt : allPackets) {
        if (curTime >= pkt.startTime && curTime <= pkt.deliveryTime) {
            double progress = (curTime - pkt.startTime) / (pkt.deliveryTime - pkt.startTime);
            const GuiNode& a = nodes[pkt.fromId];
            const GuiNode& b = nodes[pkt.toId];
            double x = a.x + (b.x - a.x) * progress;
            double y = a.y + (b.y - a.y) * progress;
            painter.drawEllipse(QPointF(x, y), 4, 4);
        }
    }

}



NodeCanvas::~NodeCanvas(){}
