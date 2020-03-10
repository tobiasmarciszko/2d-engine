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
    QRectF myRect{0, 0, 40, 40};
    QImage textures;
    QImage player;
};
#endif // MYGLWIDGET_H
