#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QOpenGLWindow>
#include <QtGui/QOpenGLFunctions>
#include <QElapsedTimer>

#include "player.h"

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

    bool isKeyUpPressed = false;
    bool isKeyDownPressed = false;
    bool isKeyLeftPressed = false;
    bool isKeyRightPressed = false;
    bool isKeySpacePressed = false;

    QVector<QPointF> bullets;
    int ticksBetweenBullets = 5;
    int currentBulletTick = 0;
    QImage textures;
    QImage bullet{40, 20, QImage::Format_ARGB32};
    Player player;
    QPainter painter;
    int frames{0};
    QElapsedTimer frameTime;
    double fps;
};
#endif // MAINWINDOW_H
