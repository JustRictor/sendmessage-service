#include "http_server.hpp"

#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

#include "logic_core/message_validator.hpp"

HttpServer::HttpServer(QObject *parent)
    : QObject{parent}
    , server(new QHttpServer(this))
    , logger( &ConsoleLogger::getInstance() )
{
    server->route("/sendMessage",
                  QHttpServerRequest::Method::Post,
                  [this](const QHttpServerRequest& request){
                      logger->log(
                          QString("get request /sendMessage/%1").arg(QString(request.body()))
                          );
                      return this->sendMessage(request);
                  });
    server->route("/genToken",
                  QHttpServerRequest::Method::Get,
                  [this](const QHttpServerRequest& request){
                      logger->log(
                          QString("get request /genToken/%1").arg(QString(request.body()))
                          );
                      return this->genToken(request);
                  });
    server->route("/getTokens",
                  QHttpServerRequest::Method::Get,
                  [this](const QHttpServerRequest& request){
                      logger->log(
                          QString("get request /getTokens/%1").arg(QString(request.body()))
                          );
                      return this->getTokens(request);
                  });
    server->route("/delToken",
                  QHttpServerRequest::Method::Delete,
                  [this](const QHttpServerRequest& request){
                      logger->log(
                          QString("get request /getTokens/%1").arg(QString(request.body()))
                          );
                      return this->delToken(request);
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
            "Invalid JSON values",
            QHttpServerResponder::StatusCode::BadRequest
            );
    }

    ///проверка токена
    if( !api.isValid(json["token"].toString(), __FUNCTION__) )
        return QHttpServerResponse({},
            "Unauthorized",
            QHttpServerResponder::StatusCode::BadRequest
            );

    ///валидация номера
    if ( MessageValidator::isValid(json["msg"].toString()) )
        return QHttpServerResponse({},
            "Invalide phone",
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
            "Cannot connect to modem",
            QHttpServerResponder::StatusCode::InternalServerError
            );
    }


}

QHttpServerResponse HttpServer::genToken(const QHttpServerRequest &request)
{

}

QHttpServerResponse HttpServer::getTokens(const QHttpServerRequest &request)
{

}

QHttpServerResponse HttpServer::delToken(const QHttpServerRequest &request)
{

}
