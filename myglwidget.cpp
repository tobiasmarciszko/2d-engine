#include "myglwidget.h"
#include "QRgb"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    textures.load(":/textures");
    QPainter p(&player);
    p.drawImage(player.rect(), textures, QRectF(150, 0, 40, 40));

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

    const qreal delta = 6.0;

    if (isKeyDownPressed) {
        playerPos.ry()+=delta;
        player.fill(Qt::transparent);
        QPainter p(&player);
        p.drawImage(player.rect(), textures, QRectF(390, 0, 40, 40));
    }

    if (isKeyUpPressed) {
        playerPos.ry()-=delta;
        player.fill(Qt::transparent);
        QPainter p(&player);
        p.drawImage(player.rect(), textures, QRectF(7, 0, 40, 40));
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
        p.drawImage(player.rect(), textures, QRectF(150, 0, 40, 40));
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

    painter.begin(this);
    painter.fillRect(0,0, width(), height(), Qt::lightGray);
    painter.drawImage(playerPos, player);
    painter.end();

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
}


