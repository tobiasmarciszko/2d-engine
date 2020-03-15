#ifndef PLAYER_H
#define PLAYER_H

#include <QPointF>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QVector>

struct Player {

    enum class State {
        MovingUp,
        MovingDown,
        Hovering
    };

    Player() {
        addSpriteFrame(0, QRectF(7, 0, 35, 40));
        addSpriteFrame(1, QRectF(55, 0, 35, 40));
        addSpriteFrame(2, QRectF(100, 0, 35, 40));
        addSpriteFrame(3, QRectF(150, 0, 35, 40));
        addSpriteFrame(4, QRectF(200, 0, 35, 40));
        addSpriteFrame(5, QRectF(250, 0, 35, 40));
        addSpriteFrame(6, QRectF(296, 0, 35, 40));
        addSpriteFrame(7, QRectF(340, 0, 35, 40));
        addSpriteFrame(8, QRectF(390, 0, 35, 40));
    }

    void addSpriteFrame(int index, QRectF fromRect) {
        QImage frame{width, height, QImage::Format_ARGB32};
        frame.fill(Qt::transparent);
        QPainter painter(&frame);
        painter.drawImage(frame.rect(), textures, fromRect);
        frames.insert(index, frame);
    }

    void moveUp() {
        pos.ry() -= moveDelta;
        state = State::MovingUp;
    }

    void moveDown() {
        pos.ry() += moveDelta;
        state = State::MovingDown;
    }

    void moveLeft() {
        pos.rx() -= moveDelta;
    }

    void moveRight() {
        pos.rx() += moveDelta;
    }

    void hovering() {
        state = State::Hovering;
    }

    void paint(QPainter& p) {
        tick++;
//        switch (state) {
//        case State::Hovering:
//            p.drawImage(pos, normal);
//            break;
//        case State::MovingUp:
//            p.drawImage(pos, up);
//            break;
//        case State::MovingDown:
//            p.drawImage(pos, down);
//            break;
//        default:
//            break;
//        }

        int delta = 5;
        if (tick % delta == 0) {
            frameIndex = (tick / delta) - 1;

            if (reverse) {
                frameIndex = (frames.count() - 1) - frameIndex;
            }
        }

        if (tick > (delta * frames.count())) {
            tick = 0;
            reverse = !reverse;
        }

        p.drawImage(pos, frames.at(frameIndex));
    }

//    update() {
//        tick++;

//        if (tick > 360) {
//            tick = 0;
//        }
//    }

    State state{State::Hovering};

    qreal moveDelta{6.0};
    QPointF pos{0, 0};
    int width{75};
    int height{80};

    QImage textures{":textures"};
    QVector<QImage> frames{};
    int tick{0};
    int frameIndex{0};
    bool reverse{false};
};

#endif // PLAYER_H
