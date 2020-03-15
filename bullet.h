#ifndef BULLET_H
#define BULLET_H

#include <QImage>
#include <QPainter>

struct Bullet {

    Bullet() {
        QImage textures{":/textures"};

        bullet.fill(Qt::transparent);
        QPainter p2{&bullet};
        p2.drawImage(bullet.rect(), textures, QRectF(240, 50, 15, 10));
    }

    void update() {
        pos.rx() += moveDelta;
    }

    void paint(QPainter& painter) {
        painter.drawImage(pos, bullet);
    }

    qreal moveDelta{12.0};
    QPointF pos{0, 0};
    int width{40};
    int height{20};

    QImage bullet{width, height, QImage::Format_ARGB32};

};

#endif // BULLET_H
