#include "http_server.hpp"

#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

#include "logic_core/message_validator.hpp"

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
                      return this->sendMessage(request);
                  });
    server->listen(QHostAddress::Any,5000);
}

QHttpServerResponse HttpServer::sendMessage(
    const QHttpServerRequest& request
    )
{
    ///парсинг JSON
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

    if ( !(json.contains("token") && json["token"].isString()
          && json.contains("phone") && json["phone"].isString()
          && json.contains("msg") && json["msg"].isString())
        )
    {
        return QHttpServerResponse(
            QHttpServerResponder::StatusCode::BadRequest
            );
    }

    ///проверка токена
    if( !api.isValid(json["token"].toString(), __FUNCTION__) )
        return QHttpServerResponse(
            QHttpServerResponder::StatusCode::BadRequest
            );

    ///валидация номера
    if ( MessageValidator::isValid(json["msg"].toString()) )
        return QHttpServerResponse(
            QHttpServerResponder::StatusCode::BadRequest
            );

    sender.sendMessage({
        .phoneNum = json["phone"].toString(),
        .message  = json["msg"].toString()
    });
    return QHttpServerResponse(
        QHttpServerResponder::StatusCode::Accepted
        );
}
