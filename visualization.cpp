#include "visualization.hpp"
#include "simulation.hpp"
#include <QPainter>

NodeCanvas::NodeCanvas(QWidget* parent) : QWidget(parent) {
    setMinimumSize(1000, 1000);
    slowFactor = 1;
    scale = 1.0;
}

void NodeCanvas::setNet(Network& net) {
    nodes.clear();
    matrix.clear();
    net_ = &net;
    for ( const Node* pn : net.nodes) {
        nodes.push_back({pn->getid(), pn->getx(), pn->gety()});
    }

    matrix = net.matrix;

    update();  // trigger rapaint
}

void NodeCanvas::addPacket(int s, int f, int t, double st, double dt) {
    allPackets.push_back({s,f,t,st,dt});
}

void NodeCanvas::startVisualization(double tTime, int  slowF) {
    curTime = 0.0;
    totalTime = tTime;
    slowFactor = slowF;
    if (!animationTimer) {
        animationTimer = new QTimer(this);
        connect(animationTimer, &QTimer::timeout, this, &NodeCanvas::updateAnimation);
    }
    animationTimer->start(30); // fresh frequency: 30ms
}

void NodeCanvas::updateAnimation() {
    curTime += 0.03 / slowFactor; // correspond to the timer interval
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

    // center the scene
    painter.translate(width() / 2, height() / 2);
    painter.scale(scale, scale);
    painter.translate(-width() / 2, -height() / 2);
    painter.translate(panOffset);

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


void NodeCanvas::togglePause() {
    if (!animationTimer) return;
    
    if (animationTimer->isActive()) {
        animationTimer->stop();
    }
    else {
        animationTimer->start(30);
    }
}

void NodeCanvas::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Plus:
        case Qt::Key_Equal:
            zoomIn();
            break;
        case Qt::Key_Minus:
            zoomOut();
            break;
        case Qt::Key_Space:
            togglePause();
            break;
        case Qt::Key_Left:
            panOffset.rx() += 20;
            update();
            break;
        case Qt::Key_Right:
            panOffset.rx() -= 20;
            update();
            break;
        case Qt::Key_Up:
            panOffset.ry() += 20;
            update();
            break;
        case Qt::Key_Down:
            panOffset.ry() -= 20;
            update();
            break;
        case Qt::Key_D:
            showDelayDialog();
            break;
        case Qt::Key_E:
            promptNodeExit();
            break;
        default:
            QWidget::keyPressEvent(event);
            break;
    }
}

void NodeCanvas::zoomIn() {
    if (scale < 5.0) {
        scale *= 1.1;
        update();
    }
}

void NodeCanvas::zoomOut() {
    if (scale > 0.2) {
        scale /= 1.1;
        update();
    }
}

NodeCanvas::~NodeCanvas(){}

SimControlWindow::SimControlWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("P2P模拟器设置");

    clientSpin = new QSpinBox;
    clientSpin->setRange(1, 1000);
    clientSpin->setValue(100);

    neighborSpin = new QSpinBox;
    neighborSpin->setRange(1,10);
    neighborSpin->setValue(5);

    cacheSpin = new QSpinBox;
    cacheSpin->setRange(1,500);
    cacheSpin->setValue(10);

    startBtn = new QPushButton("模拟开始");

    auto layout = new QVBoxLayout;
    layout->addWidget(new QLabel("客户端数量:"));
    layout->addWidget(clientSpin);
    layout->addWidget(new QLabel("邻居数量"));
    layout->addWidget(neighborSpin);
    layout->addWidget(new QLabel("缓存大小"));
    layout->addWidget(cacheSpin);
    layout->addWidget(startBtn);

    setLayout(layout);

    connect(startBtn, &QPushButton::clicked,  [=]() {
        emit simulationStarted(clientSpin->value(), neighborSpin->value(), cacheSpin->value());
        this->close();
    });
}

void NodeCanvas::showDelayDialog() {
    if (!net_) return;

    QDialog dialog(this);
    dialog.setWindowTitle("查看延迟");

    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QLabel* label = new QLabel("客户ID:");
    QSpinBox* spin = new QSpinBox;
    spin->setRange(1, net_->nodes.size() - 1);

    QPushButton* checkBtn = new QPushButton("查看");
    QLabel* resultLabel = new QLabel("");

    layout->addWidget(label);
    layout->addWidget(spin);
    layout->addWidget(checkBtn);
    layout->addWidget(resultLabel);

    QObject::connect(checkBtn, &QPushButton::clicked, [&]() {
        int cid = spin->value();
        Node* client = net_->nodes[cid];
        const auto& playedBlk = dynamic_cast<Client*>(client) -> getPlayedBlk();
        if (playedBlk.empty()) {
            resultLabel->setText("没有播放数据");
            return;
        }

        double sum = 0;
        for (int i = 0; i < playedBlk.size(); ++i) {
            double expected = i / 30.0;
            sum += playedBlk[i] - expected;
        }
        double avg = sum / playedBlk.size();
        resultLabel->setText(QString("平均延迟: %1 s").arg(avg, 0, 'f', 4));
    });

    dialog.exec();
}

void NodeCanvas::promptNodeExit() {
    if (!net_) return;
    
    // let user input cid
    bool ok;
    int cid = QInputDialog::getInt(
        this,
        "节点退出",
        "输入退出客户ID",
        1, 1, net_->nodes.size() - 1, 1, &ok);
    
    if (!ok) return;

    // remove node
    net_->nodeExit(cid);

    // reset
    for (const auto& p : net_->nodes) p->restart();

    allPackets.clear();
    curTime = 0.0;

    // rerun simualtion
    Simulation sim(*net_, this);
    sim.run();

    // rerun visualization
    startVisualization();
}
