#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QTimer>
#include <QPainter>
#include <QOpenGLFunctions>
#include <QRectF>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = nullptr);

protected:
    void initializeGL() override;

    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QPainter painter;
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
#endif // MYGLWIDGET_H
