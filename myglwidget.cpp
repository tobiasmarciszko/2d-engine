#include "myglwidget.h"
#include "QRgb"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    textures.load(":/textures");

    player = QImage(40, 40, QImage::Format_RGB32);
    QPainter p(&player);
    p.drawImage(myRect, textures, QRectF(150, 0, 40, 40));

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

    const qreal delta = 3.0;

    if (isKeyDownPressed) {
        myRect.translate(0, delta);
    }

    if (isKeyUpPressed) {
        myRect.translate(0, -delta);
    }

    if (isKeyLeftPressed) {
        myRect.translate(-delta, 0);
    }

    if (isKeyRightPressed) {
        myRect.translate(delta, 0);
    }

    painter.begin(this);
    painter.fillRect(myRect, qRgb(255, 255, 255));
    painter.drawImage(myRect, player);
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


