#include "mainwindow.h"

#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow()
{
    textures.load(":/textures");

    QPainter p2(&bullet);
    p2.drawImage(bullet.rect(), textures, QRectF(240, 50, 15, 10));

    setKeyboardGrabEnabled(true);
}

void MainWindow::initializeGL()
{
    frameTime.start();
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
void MainWindow::paintGL()
{
    painter.begin(this);

    updateBackground();
    updateBullets();
    updatePlayer();

    // show fps counter?
    frames++;
    if (frameTime.elapsed() > 1000.0) {
         fps = frames / (frameTime.elapsed()/1000.0);
         frames = 0;
         frameTime.restart();
    }

    painter.drawText(0, 10, "FPS: " + QString::number(fps));

    painter.end();
    update();
}

void MainWindow::updateBackground()
{
    painter.fillRect(0,0, width(), height(), Qt::lightGray);
}

void MainWindow::updateBullets()
{
    if (isKeySpacePressed && (currentBulletTick >= ticksBetweenBullets)) {
        QPointF bulletPos{player.pos.rx()+player.width, player.pos.ry() + player.height/2};
        bullets.append(bulletPos);
        currentBulletTick = 0;
    }

    for (QPointF& b: bullets) {
        b.rx() += 12.0;

        if (b.rx() > width()) {
            bullets.removeFirst();
        }

        painter.drawImage(b, bullet);
    }

    currentBulletTick = currentBulletTick + 1 % 200;
}

void MainWindow::updatePlayer() {

    if (isKeyDownPressed) {
        player.moveDown();
    }

    if (isKeyUpPressed) {
        player.moveUp();
    }

    if (isKeyLeftPressed) {
        player.moveLeft();
    }

    if (isKeyRightPressed) {
        player.moveRight();
    }

    if (!isKeyUpPressed && !isKeyDownPressed) {
        player.hovering();
    }

    // Limit player to height and width if the viewport
    if (player.pos.rx() > width() - player.width) {
        player.pos.rx() = width() - player.width;
    }

    if (player.pos.rx() <= 0) {
        player.pos.rx() = 0;
    }

    if (player.pos.ry() > height() - player.height) {
        player.pos.ry() = height() - player.height;
    }

    if (player.pos.ry() <= 0) {
        player.pos.ry() = 0;
    }

    player.paint(painter);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
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

void MainWindow::keyReleaseEvent(QKeyEvent *e)
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
