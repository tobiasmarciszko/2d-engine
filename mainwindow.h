#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QOpenGLWindow>
#include <QtGui/QOpenGLFunctions>
#include <QElapsedTimer>

#include "player.h"
#include "bullet.h"

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
QT_END_NAMESPACE

class MainWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    MainWindow();

    void initializeGL() override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:

    void updateBackground();
    void updateBullets();
    void updatePlayer();
    void updateFPSCounter();

    bool isKeyUpPressed = false;
    bool isKeyDownPressed = false;
    bool isKeyLeftPressed = false;
    bool isKeyRightPressed = false;
    bool isKeySpacePressed = false;

    QVector<Bullet> bullets{};
    int ticksBetweenBullets = 5;
    int currentBulletTick = 0;
    Player player;
    QPainter painter;
    int frames{0};
    QElapsedTimer frameTime;
    double fps;
};
#endif // MAINWINDOW_H
