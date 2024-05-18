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
