#ifndef __HTTP_SERVER_HPP_
#define __HTTP_SERVER_HPP_

#include <QObject>
#include <QHttpServer>
#include "message_sender/message_sender.hpp"
#include "logic_core/api_manage.hpp"
#include "logger/console_logger.hpp"

class HttpServer : public QObject
{
    Q_OBJECT
private:
    QHttpServer* server;
    msend::MessageSender sender;
    api::ApiManage api;
    ConsoleLogger* logger;
public:
    explicit HttpServer(QObject *parent = nullptr);
private:
    QHttpServerResponse sendMessage(const QHttpServerRequest& request);
    QHttpServerResponse genToken(const QHttpServerRequest& request);
    QHttpServerResponse getTokens(const QHttpServerRequest& request);
    QHttpServerResponse delToken(const QHttpServerRequest& request);
signals:
};

#endif // __HTTP_SERVER_HPP_
