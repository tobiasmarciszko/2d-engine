#include "mainwindow.h"

#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QtGamepad/QGamepad>

MainWindow::MainWindow()
{
    setKeyboardGrabEnabled(true);
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

    connect(QGamepadManager::instance(), &QGamepadManager::gamepadAxisEvent, this, [&](int deviceId, QGamepadManager::GamepadAxis axis, double value){
        if (axis == QGamepadManager::AxisLeftX) {
            if (value < 0) {
                isKeyLeftPressed = true;
                isKeyRightPressed = false;
            } else if (value > 0) {
                isKeyRightPressed = true;
                isKeyLeftPressed = false;
            } else {
                isKeyRightPressed = false;
                isKeyLeftPressed = false;
            }
        }

        if (axis == QGamepadManager::AxisLeftY) {
            if (value < 0) {
                isKeyDownPressed = false;
                isKeyUpPressed = true;
            } else if (value > 0) {
                isKeyUpPressed = false;
                isKeyDownPressed = true;
            } else {
                isKeyUpPressed = false;
                isKeyDownPressed = false;
            }
        }
    });

    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonPressEvent, this, [&](int deviceId, QGamepadManager::GamepadButton button, double value){
        isKeySpacePressed = true;
    });

    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonReleaseEvent, this, [&](int deviceId, QGamepadManager::GamepadButton button){
        isKeySpacePressed = false;
    });
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
void MainWindow::updateFPSCounter()
{
    frames++;
    if (frameTime.elapsed() > 1000.0) {
         fps = frames / (frameTime.elapsed()/1000.0);
         frames = 0;
         frameTime.restart();
    }

    painter.drawText(0, 10, "FPS: " + QString::number(fps));
}

void MainWindow::paintGL()
{
    painter.begin(this);

    updateBackground();
    updateBullets();
    updatePlayer();
    updateFPSCounter();

    painter.end();
}

void MainWindow::updateBackground()
{
    painter.fillRect(0,0, width(), height(), Qt::lightGray);
}

void MainWindow::updateBullets()
{
    if (isKeySpacePressed && (currentBulletTick >= ticksBetweenBullets)) {
        QPointF bulletPos{player.pos.rx()+player.width, player.pos.ry() + player.height/2};
        Bullet bullet;
        bullet.pos = bulletPos;
        bullets.append(bullet);
        currentBulletTick = 0;
    }

    for (Bullet& b: bullets) {
        b.update();

        if (b.pos.rx() > width()) {
            bullets.removeFirst();
        } else {
            b.paint(painter);
        }
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
    switch (e->key()) {
        case Qt::Key_Down:
            isKeyDownPressed = true;
            break;
        case Qt::Key_Up:
            isKeyUpPressed = true;
            break;
        case Qt::Key_Left:
            isKeyLeftPressed = true;
            break;
        case Qt::Key_Right:
            isKeyRightPressed = true;
            break;
        case Qt::Key_Space:
            isKeySpacePressed = true;
            break;
        default:
            break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    switch (e->key()) {
        case Qt::Key_Down:
            isKeyDownPressed = false;
            break;
        case Qt::Key_Up:
            isKeyUpPressed = false;
            break;
        case Qt::Key_Left:
            isKeyLeftPressed = false;
            break;
        case Qt::Key_Right:
            isKeyRightPressed = false;
            break;
        case Qt::Key_Space:
            isKeySpacePressed = false;
            break;
        default:
            break;
    }
}
