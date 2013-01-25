#include <QtGui/QApplication>
#include "mainwindow.h"
#include "fervor/fvupdater.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Prerequisite for the Fervor updater
        QApplication::setOrganizationName("Computerfr33k");
        QApplication::setOrganizationDomain("computerfr33k.com");
        QApplication::setApplicationVersion("0.2");

        // Set feed URL before doing anything else
        FvUpdater::sharedUpdater()->SetFeedURL("http://dl.dropbox.com/u/23794188/To%20Do%20List%20Manager/Appcast.xml");

        // Check for updates automatically
        FvUpdater::sharedUpdater()->CheckForUpdatesSilent();


    MainWindow w;
    w.show();
    
    return a.exec();
}
