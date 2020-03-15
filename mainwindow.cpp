#include "mainwindow.h"

#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QtGamepad/QGamepad>
#include <QRandomGenerator>

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

void MainWindow::resizeGL(int w, int h)
{
    starfield.clear();

    for (int i = 0; i < 100; i++) {
        Star star;
        star.pos.rx() = qrd.bounded(1, w);
        star.pos.ry() = qrd.bounded(1, h);
        star.speed = 1.0 / qrd.bounded(1, 4);
        star.color = star.speed;
        starfield.append(star);
    }
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
    if (frameTime.elapsed() > 1000.0) {
        lastTimeForOneFrame = accumulatedFrameTime / frames;
        accumulatedFrameTime = 0;
         fps = frames / (frameTime.elapsed()/1000.0);
         frames = 0;
         frameTime.restart();
    }

    painter.setPen(Qt::white);
    painter.drawText(0, 10, QString::number(fps, 'f', 0) + " FPS " + QString::number(lastTimeForOneFrame,'f',0) + "ms per frame");
}

void MainWindow::paintGL()
{
    frames++;

    timeForOneFrame.start();
    painter.begin(this);

    updateBackground();
    updateBullets();
    updatePlayer();
    updateFPSCounter();

    painter.end();

    accumulatedFrameTime += timeForOneFrame.elapsed();
}

void MainWindow::updateBackground()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_SCISSOR_TEST);
    for (Star& star: starfield) {
        // rects.append(QRectF{star.pos.rx(), star.pos.ry(), 4, 4});
        //const auto color = 0.45 * star.speed;
        // painter.fillRect(QRectF{star.pos.rx(), star.pos.ry(), 4, 4}, qRgb(255, 255, 255));
        // star.state = player.state;
        // star.paint(painter);
        //painter.drawImage(QPointF{star.pos.rx(), star.pos.ry()}, player.normal);

        glScissor(star.pos.rx(), star.pos.ry(), 4, 4);
        glClearColor(star.color, star.color, star.color, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        // Remember to disable scissor test, or, perhaps reset the scissor rectangle:
        star.pos.rx() -= star.speed;
        star.pos.ry() += sin(0.0117 * star.pos.rx());

        if (star.pos.rx() < -1) {
            star.pos.rx() = width() + 1;
            star.pos.ry() = qrd.bounded(0, height());
        }
    }

    //painter.setPen(qRgb(255,255,255));

    //painter.drawRects(rects);
    glDisable(GL_SCISSOR_TEST);
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
