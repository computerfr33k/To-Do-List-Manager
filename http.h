#ifndef HTTP_H
#define HTTP_H

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class Http : public QObject
{
    Q_OBJECT
public:
    explicit Http(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // HTTP_H
