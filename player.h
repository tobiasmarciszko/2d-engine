#ifndef PLAYER_H
#define PLAYER_H

#include <QPointF>
#include <QImage>
#include <QPainter>

struct Player {

    enum class State {
        MovingUp,
        MovingDown,
        Hovering
    };

    Player() {
        QImage textures;
        textures.load(":/textures");

        // Set up textures from the sprite sheet
        normal.fill(Qt::transparent);
        QPainter p(&normal);
        p.drawImage(normal.rect(), textures, QRectF(150, 0, 35, 40));

        up.fill(Qt::transparent);
        QPainter p1(&up);
        p1.drawImage(up.rect(), textures, QRectF(7, 0, 35, 40));

        down.fill(Qt::transparent);
        QPainter p2(&down);
        p2.drawImage(down.rect(), textures, QRectF(390, 0, 35, 40));
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
        switch (state) {
        case State::Hovering:
            p.drawImage(pos, normal);
            break;
        case State::MovingUp:
            p.drawImage(pos, up);
            break;
        case State::MovingDown:
            p.drawImage(pos, down);
            break;
        default:
            break;
        }
    }

    State state{State::Hovering};

    qreal moveDelta{6.0};
    QPointF pos{0, 0};
    int width{75};
    int height{80};

    QImage normal{width, height, QImage::Format_ARGB32};
    QImage up{width, height, QImage::Format_ARGB32};
    QImage down{width, height, QImage::Format_ARGB32};

};

#endif // PLAYER_H
