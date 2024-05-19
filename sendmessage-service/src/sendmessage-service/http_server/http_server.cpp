#include "http_server.hpp"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QByteArray>

#include "logic_core/message_validator.hpp"

HttpServer::HttpServer(QObject *parent)
    : QObject{parent}
    , server(new QHttpServer(this))
    , logger( &ConsoleLogger::getInstance() )
{
    server->route("/ping",
                  QHttpServerRequest::Method::Get,
                  [this](const QHttpServerRequest& request){
                      if( !isAuthenticated(request) )
                          return QHttpServerResponse(
                              QHttpServerResponse::StatusCode::Unauthorized
                              );
                      return QHttpServerResponse(
                          QHttpServerResponse::StatusCode::Ok
                          );
                  });
    server->route("/sendMessage",
                  QHttpServerRequest::Method::Post,
                  [this](const QHttpServerRequest& request){
                          if( !isAuthenticated(request) )
                              return QHttpServerResponse(
                                  QHttpServerResponse::StatusCode::Unauthorized
                                  );
                      logger->log(
                          QString("get request /sendMessage/%1").arg(QString(request.body()))
                          );
                      return this->sendMessage(request);
                  });
    server->route("/genToken",
                  QHttpServerRequest::Method::Get,
                  [this](const QHttpServerRequest& request){
                      if( !isAuthenticated(request) )
                          return QHttpServerResponse(
                              QHttpServerResponse::StatusCode::Unauthorized
                              );
                      logger->log(
                          QString("get request /genToken/%1").arg(QString(request.body()))
                          );
                      return this->genToken(request);
                  });
    server->route("/getTokens",
                  QHttpServerRequest::Method::Get,
                  [this](const QHttpServerRequest& request){
                      if( !isAuthenticated(request) )
                          return QHttpServerResponse(
                              QHttpServerResponse::StatusCode::Unauthorized
                              );
                      logger->log(
                          QString("get request /getTokens/%1").arg(QString(request.body()))
                          );
                      return this->getTokens(request);
                  });
    server->route("/delToken/<token>",
                  QHttpServerRequest::Method::Delete,
                  [this](const QString& token, const QHttpServerRequest& request){
                      if( !isAuthenticated(request) )
                          return QHttpServerResponse(
                              QHttpServerResponse::StatusCode::Unauthorized
                              );
                      logger->log(
                          QString("get request /getTokens/%1").arg(QString(request.body()))
                          );
                      return this->delToken(request, token);
                  });
    server->listen(QHostAddress::Any,5000);
}

bool HttpServer::isAuthenticated(const QHttpServerRequest &request)
{
    auto authHeader = request.value("Authorization");
    if (!authHeader.startsWith("Bearer ")) {
        return false;
    }
    auto token = authHeader.mid(7);
    return api::ApiManage::isValid(token);
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
            QHttpServerResponse::StatusCode::BadRequest
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
            QHttpServerResponse::StatusCode::BadRequest
            );
    }

    if ( !(json.contains("phone") && json["phone"].isString()
          && json.contains("msg") && json["msg"].isString())
        )
    {
        return QHttpServerResponse({},
            "Invalid JSON values",
            QHttpServerResponse::StatusCode::BadRequest
            );
    }

    ///валидация номера
    if ( !MessageValidator::isValid(json["phone"].toString()) )
        return QHttpServerResponse({},
            "Invalide phone",
            QHttpServerResponse::StatusCode::BadRequest
            );

    msend::ResponseAnsw resp = sender.sendMessage({
        .phoneNum = json["phone"].toString(),
        .message  = json["msg"].toString()
    });
    switch(resp)
    {
    case msend::ResponseAnsw::Success:
        return QHttpServerResponse(
            QHttpServerResponse::StatusCode::Ok
            );
    case msend::ResponseAnsw::Failure:
        return QHttpServerResponse({},
            "Message not sended",
            QHttpServerResponse::StatusCode::InternalServerError
            );
    case msend::ResponseAnsw::CannotConnect:
        return QHttpServerResponse({},
            "Cannot connect to modem",
            QHttpServerResponse::StatusCode::InternalServerError
            );
    }


}

QHttpServerResponse HttpServer::genToken(const QHttpServerRequest &)
{
    return QHttpServerResponse({},
        api.addToken().toUtf8(), ///\todo добавить хеширование
        QHttpServerResponse::StatusCode::Ok
        );
}

QHttpServerResponse HttpServer::getTokens(const QHttpServerRequest &)
{
    QJsonArray jsonArray{};
    QList<QString> tokens = api.getTokens();
    for( const auto& item : tokens)
        jsonArray.append(item);
    return QHttpServerResponse({},
        QJsonDocument(jsonArray).toJson(QJsonDocument::Compact),
        QHttpServerResponse::StatusCode::Ok
        );
}

QHttpServerResponse HttpServer::delToken(
    const QHttpServerRequest &,
    const QString& token
    )
{
    if(api.delToken(token))
        return QHttpServerResponse::StatusCode::Ok;
    return QHttpServerResponse({},
        "invalid token id",
        QHttpServerResponse::StatusCode::BadRequest
        );
}
