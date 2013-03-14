#include <QCoreApplication>
#include <QUrl>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
#include <QStringList>
#include <QFile>
#include <QSysInfo>

#include "http.h"
#include "updatedata.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    updateData updateInfo;

    QFile file("update.json");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "Could Not Open File: update.json";

    QByteArray json;
    while(!file.atEnd())
    {
        json = file.readAll();
    }
    file.close();

    QJsonDocument jsonDoc;
    jsonDoc = jsonDoc.fromJson(json);
    updateInfo.setVersion(jsonDoc.object().value(QString("version")).toString());
#ifdef Q_OS_WIN
    updateInfo.setUrl(jsonDoc.object().value(QString("windows")).toObject().value(QString("url")).toString());
#endif
#ifdef Q_OS_Linux
    updateInfo.setUrl(jsonDoc.object().value(QString("linux")).toObject().value(QString("url")).toString());
#endif

    qDebug() << "current version: " << updateInfo.getVersion();
    qDebug() << "file url: " << updateInfo.getUrl().toString();
    
    return a.exec();
}
