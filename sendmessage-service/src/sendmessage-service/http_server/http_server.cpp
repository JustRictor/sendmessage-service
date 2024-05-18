#include "http_server.hpp"

HttpServer::HttpServer(QObject *parent)
    : QObject{parent}
    , server(new QHttpServer(this))
{
    server->route("/",[](){
        return "hello world";
    });
    server->listen(QHostAddress::Any,5000);
}

void HttpServer::sendMessage(QString token, QString num, QString mess)
{
    ///\todo validate token
}
