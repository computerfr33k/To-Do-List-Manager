#include <QtWidgets/QApplication>
#include <QDebug>
#include <QFile>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(argc > 1)
    {
        if(a.arguments().at(1).compare("-portable") == 0)
        {
            qDebug() << "-portable specified";
            QFile file("portable.dat");
            file.open(QIODevice::WriteOnly);
            file.write("");
            file.close();
        }
    }

    // Prerequisite for the Fervor updater
        QApplication::setOrganizationName("Computerfr33k");
        QApplication::setOrganizationDomain("computerfr33k.com");
        QApplication::setApplicationVersion("0.3");

    MainWindow w;
    w.show();
    
    return a.exec();
}
