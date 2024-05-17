#ifndef __HTTP_SERVER_HPP_
#define __HTTP_SERVER_HPP_

#include <QObject>
#include <QHttpServer>

class HttpServer : public QObject
{
    Q_OBJECT
private:
    QHttpServer* server;
public:
    explicit HttpServer(QObject *parent = nullptr);

signals:
};

#endif // __HTTP_SERVER_HPP_
