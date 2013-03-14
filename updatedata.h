#ifndef UPDATEDATA_H
#define UPDATEDATA_H

#include <QObject>
#include <QString>
#include <QUrl>

class updateData : public QObject
{
    Q_OBJECT
public:
    explicit updateData(QObject *parent = 0);
    void setVersion(QString v);
    void setUrl(QString u);
    QString getVersion();
    QUrl getUrl();

protected:
    QString version;
    QUrl url;
    
signals:
    
public slots:
    
};

#endif // UPDATEDATA_H
