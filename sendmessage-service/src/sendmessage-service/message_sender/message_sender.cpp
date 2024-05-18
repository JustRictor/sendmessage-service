#include "message_sender.hpp"
#include <QDateTime>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QEventLoop>

msend::MessageSender::MessageSender(QObject *parent)
    : QObject{parent}
    , manager( new QNetworkAccessManager(this) )
{
    request.setUrl(URL);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/html");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0");
}

msend::ResponseAnsw msend::MessageSender::sendMessage(const Tdo &data)
{
    QUrlQuery postData;
    QDateTime date{QDateTime::currentDateTime()};
    postData.addQueryItem("isTest", "false");
    postData.addQueryItem("goformId", "SEND_SMS");
    postData.addQueryItem("notCallback", "true");
    postData.addQueryItem("Number", "+79216495428");
    postData.addQueryItem("sms_time",date.toString("yy;MM;dd;hh;mm;ss;+3"));
    postData.addQueryItem("MessageBody", "003100320034");
    postData.addQueryItem("ID", "-1");
    postData.addQueryItem("encode_type", "UNICODE");

    QNetworkReply* reply = manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

    QEventLoop await;
    QObject::connect(reply,&QNetworkReply::finished,
                     &await,&QEventLoop::quit);
    await.exec();
    if (reply->error() == QNetworkReply::NoError) {
        // Запрос выполнен успешно
        QByteArray response = reply->readAll();
        qDebug() << "Response:" << response;
    } else {
        // Произошла ошибка
        qDebug() << "Error:" << reply->errorString();
    }

    return ResponseAnsw::Success;
}

void msend::MessageSender::rep(QNetworkReply *reply)
{
    QString answer = reply->readAll();
    qDebug() << answer;
}
