#ifndef __HTTP_SERVER_HPP_
#define __HTTP_SERVER_HPP_

#include <QObject>
#include <QHttpServer>
#include "message_sender/message_sender.hpp"

class HttpServer : public QObject
{
    Q_OBJECT
private:
    QHttpServer* server;
    msend::MessageSender sender;
public:
    explicit HttpServer(QObject *parent = nullptr);
private:
    QHttpServerResponse sendMessage(
        QString token,
        QString num,
        QString mess
        );
signals:
};

#endif // __HTTP_SERVER_HPP_
