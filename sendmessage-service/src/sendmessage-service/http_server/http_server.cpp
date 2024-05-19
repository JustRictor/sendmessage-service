#include "http_server.hpp"
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

HttpServer::HttpServer(QObject *parent)
    : QObject{parent}
    , server(new QHttpServer(this))
{
    server->route("/",[](){
        return "hello world";
    });
    server->route("/sendMessage",
                  QHttpServerRequest::Method::Post,
                  [this](const QHttpServerRequest& request){
                      QJsonParseError parseError;
                      QJsonDocument jsonDoc = QJsonDocument::fromJson(request.body(), &parseError);

                      if (parseError.error != QJsonParseError::NoError) {
                          return QHttpServerResponse(
                              QHttpServerResponder::StatusCode::BadRequest
                              );
                      }

                      if (!jsonDoc.isObject()) {
                          return QHttpServerResponse(
                              QHttpServerResponse::StatusCode::BadRequest
                              );
                      }

                      QJsonObject json = jsonDoc.object();
                      if(json.isEmpty())
                      {
                          return QHttpServerResponse(
                              QHttpServerResponder::StatusCode::BadRequest
                              );
                      }
                      sender.sendMessage({
                          .phoneNum = json["phone"].toString(),
                          .message  = json["msg"].toString()
                      });
                      return QHttpServerResponse(
                          QHttpServerResponder::StatusCode::Accepted
                          );
                  }
                  );
    server->listen(QHostAddress::Any,5000);
}

QHttpServerResponse HttpServer::sendMessage(
    QString ,
    QString num,
    QString mess
    )
{
    ///\todo validate token
    ///      validate phone num
    sender.sendMessage({
        .phoneNum = num,
        .message  = mess
    });
    return "Ok";
}
