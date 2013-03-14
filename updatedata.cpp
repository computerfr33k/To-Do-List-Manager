#include "updatedata.h"

updateData::updateData(QObject *parent) :
    QObject(parent)
{
}

void updateData::setVersion(QString v)
{
    version = v;
}

void updateData::setUrl(QString u)
{
    url = QUrl::fromUserInput(u);
}

QString updateData::getVersion()
{
    return version;
}

QUrl updateData::getUrl()
{
    return url;
}
