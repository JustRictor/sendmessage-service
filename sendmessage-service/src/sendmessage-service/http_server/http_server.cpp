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
        return QHttpServerResponse({},
            "Invalid JSON",
            QHttpServerResponder::StatusCode::BadRequest
            );
    }

    if (!jsonDoc.isObject()) {
        return QHttpServerResponse({},
            "Invalid JSON",
            QHttpServerResponse::StatusCode::BadRequest
            );
    }

    QJsonObject json = jsonDoc.object();
    if(json.isEmpty())
    {
        return QHttpServerResponse({},
            "Invalid JSON",
            QHttpServerResponder::StatusCode::BadRequest
            );
    }

    if ( !(json.contains("token") && json["token"].isString()
          && json.contains("phone") && json["phone"].isString()
          && json.contains("msg") && json["msg"].isString())
        )
    {
        return QHttpServerResponse({},
            "bad JSON values",
            QHttpServerResponder::StatusCode::BadRequest
            );
    }

    ///проверка токена
    if( !api.isValid(json["token"].toString(), __FUNCTION__) )
        return QHttpServerResponse({},
            "unauthorized",
            QHttpServerResponder::StatusCode::BadRequest
            );

    ///валидация номера
    if ( MessageValidator::isValid(json["msg"].toString()) )
        return QHttpServerResponse({},
            "phone not valid",
            QHttpServerResponder::StatusCode::BadRequest
            );

    msend::ResponseAnsw resp = sender.sendMessage({
        .phoneNum = json["phone"].toString(),
        .message  = json["msg"].toString()
    });
    switch(resp)
    {
    case msend::ResponseAnsw::Success:
        return QHttpServerResponse(
            QHttpServerResponder::StatusCode::Ok
            );
    case msend::ResponseAnsw::Failure:
        return QHttpServerResponse({},
            "Message not sended",
            QHttpServerResponder::StatusCode::InternalServerError
            );
    case msend::ResponseAnsw::CannotConnect:
        return QHttpServerResponse({},
            "Cant connect to modem",
            QHttpServerResponder::StatusCode::InternalServerError
            );
    }


}
