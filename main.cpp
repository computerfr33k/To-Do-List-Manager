#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("To Do List Manager");
    QApplication::setApplicationVersion("0.2 Alpha");
    MainWindow w;
    w.show();
    
    return a.exec();
}
