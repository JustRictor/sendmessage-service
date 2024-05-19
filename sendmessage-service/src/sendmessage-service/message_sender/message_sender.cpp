#include "message_sender.hpp"
#include <QDateTime>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QEventLoop>
#include <QTimer>

msend::MessageSender::MessageSender(QObject *parent)
    : QObject{parent}
    , manager( new QNetworkAccessManager(this) )
    , logger( &ConsoleLogger::getInstance() )
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
    postData.addQueryItem("Number", data.phoneNum);
    postData.addQueryItem("sms_time",date.toString("yy;MM;dd;hh;mm;ss;+3"));
    postData.addQueryItem("MessageBody", [&data](){
        QString output{};
        for(QChar const& ch : data.message)
            output.append(
                QString("%1").arg(ch.unicode(), 4, 16, QLatin1Char('0'))
                );
        return output;
    }());
    postData.addQueryItem("ID", "-1");
    postData.addQueryItem("encode_type", "UNICODE");

    QNetworkReply* reply = manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    QTimer replyTimer; replyTimer.setSingleShot(true);
    QEventLoop await;
    QObject::connect(reply,&QNetworkReply::finished,
                     &await,&QEventLoop::quit);
    QObject::connect(&replyTimer,&QTimer::timeout,
                     &await,&QEventLoop::quit);
    replyTimer.start(1000);
    await.exec();
    QByteArray response = reply->readAll();
    if(!replyTimer.isActive())
    {
        qDebug() << "cannot connect";
        reply->abort();
        return ResponseAnsw::CannotConnect;
    }
    if(response.contains("success"))
    {
        qDebug() << "success";
        return ResponseAnsw::Success;
    }
    else
    {
        qDebug() << "failure";
        return ResponseAnsw::Failure;
    }
}
