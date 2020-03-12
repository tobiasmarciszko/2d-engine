#include "mainwindow.h"

#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    MainWindow window;
    window.resize(640, 480);
    window.show();

    return a.exec();
}
