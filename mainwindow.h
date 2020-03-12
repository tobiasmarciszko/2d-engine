#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QOpenGLWindow>
#include <QtGui/QOpenGLFunctions>

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
QT_END_NAMESPACE

class MainWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MainWindow(QWindow *parent = nullptr);

    void initializeGL() override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;


private:
    bool isKeyUpPressed = false;
    bool isKeyDownPressed = false;
    bool isKeyLeftPressed = false;
    bool isKeyRightPressed = false;
    bool isKeySpacePressed = false;

    QVector<QPointF> bullets;
    int ticksBetweenBullets = 5;
    int currentBulletTick = 0;
    QPointF playerPos{0, 0};
    QImage textures;
    QImage player{75, 80, QImage::Format_ARGB32};
    QImage bullet{40, 20, QImage::Format_ARGB32};
};
#endif // MAINWINDOW_H
