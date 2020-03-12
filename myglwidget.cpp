#include "myglwidget.h"
#include "QRgb"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    textures.load(":/textures");
    QPainter p(&player);
    p.drawImage(player.rect(), textures, QRectF(150, 0, 35, 35));

    QPainter p2(&bullet);
    p2.drawImage(bullet.rect(), textures, QRectF(240, 50, 15, 10));

    grabKeyboard();
    update();
}

void MyGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    QString glType;
    QString glVersion;
    QString glProfile;

    // Get Version Information
    glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    // Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch (format().profile())
    {
        CASE(NoProfile);
        CASE(CoreProfile);
        CASE(CompatibilityProfile);
    }
#undef CASE

    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";

}

void MyGLWidget::paintGL() {

    painter.begin(this);
    painter.fillRect(0,0, width(), height(), Qt::black);

    // updated bullets

    if (isKeySpacePressed && (currentBulletTick >= ticksBetweenBullets)) {
        QPointF bulletPos{playerPos.rx()+player.width(), playerPos.ry() + player.height()/2};
        bullets.append(bulletPos);
        currentBulletTick = 0;
    }

    for(QPointF& b: bullets) {
        b.rx()+=12.0;

        if (b.rx() > width()) {
            bullets.removeFirst();
        }

        painter.drawImage(b, bullet);
    }

    // update player
    const qreal delta = 6.0;

    if (isKeyDownPressed) {
        playerPos.ry()+=delta;
        player.fill(Qt::transparent);
        QPainter p(&player);
        p.drawImage(player.rect(), textures, QRectF(390, 0, 35, 40));
    }

    if (isKeyUpPressed) {
        playerPos.ry()-=delta;
        player.fill(Qt::transparent);
        QPainter p(&player);
        p.drawImage(player.rect(), textures, QRectF(7, 0, 35, 40));
    }

    if (isKeyLeftPressed) {
        playerPos.rx()-=delta;
    }

    if (isKeyRightPressed) {
        playerPos.rx()+=delta;
    }

    if (!isKeyUpPressed && !isKeyDownPressed) {
        player.fill(Qt::transparent);
        QPainter p(&player);
        p.drawImage(player.rect(), textures, QRectF(150, 0, 35, 40));
    }

    // Limit player to height and width if the viewport
    if (playerPos.rx() > width() - player.rect().width()) {
        playerPos.rx() = width() - player.rect().width();
    }

    if (playerPos.rx() <= 0) {
        playerPos.rx() = 0;
    }

    if (playerPos.ry() > height() - player.rect().height()) {
        playerPos.ry() = height() - player.rect().height();
    }

    if (playerPos.ry() <= 0) {
        playerPos.ry() = 0;
    }

    painter.drawImage(playerPos, player);
    painter.end();

    currentBulletTick = currentBulletTick + 1 % 200;
    update();
}

void MyGLWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Down) {
        isKeyDownPressed = true;
    }

    if (e->key() == Qt::Key_Up) {
        isKeyUpPressed = true;
    }

    if (e->key() == Qt::Key_Left) {
        isKeyLeftPressed = true;
    }

    if (e->key() == Qt::Key_Right) {
        isKeyRightPressed = true;
    }

    if (e->key() == Qt::Key_Space) {
        isKeySpacePressed = true;
    }
}

void MyGLWidget::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Down) {
        isKeyDownPressed = false;
    }

    if (e->key() == Qt::Key_Up) {
        isKeyUpPressed = false;
    }

    if (e->key() == Qt::Key_Left) {
        isKeyLeftPressed = false;
    }

    if (e->key() == Qt::Key_Right) {
        isKeyRightPressed = false;
    }

    if (e->key() == Qt::Key_Space) {
        isKeySpacePressed = false;
    }

}


