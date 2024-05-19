#include "message_sender.hpp"
#include <QDateTime>
#include <QNetworkReply>
#include <QUrlQuery>
<<<<<<< HEAD
#include <QEventLoop>
=======
>>>>>>> origin/addValidation

msend::MessageSender::MessageSender(QObject *parent)
    : QObject{parent}
    , manager( new QNetworkAccessManager(this) )
{
    request.setUrl(URL);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/html");
<<<<<<< HEAD
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0");
=======
    QObject::connect(manager, &QNetworkAccessManager::finished,
                     this, &MessageSender::rep);
>>>>>>> origin/addValidation
}

msend::ResponseAnsw msend::MessageSender::sendMessage(const Tdo &data)
{
<<<<<<< HEAD
    qDebug() << data.phoneNum << data.message;
    QUrlQuery postData;
    QDateTime date{QDateTime::currentDateTime()};
    postData.addQueryItem("isTest", "false");
    postData.addQueryItem("goformId", "SEND_SMS");
    postData.addQueryItem("notCallback", "true");
    postData.addQueryItem("Number", data.phoneNum);
    postData.addQueryItem("sms_time",date.toString("yy;MM;dd;hh;mm;ss;+3"));
    postData.addQueryItem("MessageBody", data.message);
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

=======
    // QJsonObject obj;
    // obj["isTest"] = "false";
    // obj["goformId"] = "SEND_SMS";
    // obj["notCallback"] = "true";
    // obj["Number"] = "+79818398770";
    //     //"%2B" + QString::number(data.phoneNum);
    // obj["sms_time"] = QDateTime::currentDateTime()
    //                       .toString("yy%3BMM%3Bdd%3Bhh%3Bmm%3Bss%3B%2B3"); ///\todo +3fixme
    // obj["MessageBody"] = QString("0063006800650063006B");
    //     //QString(data.message.toUtf8().toHex());
    // obj["ID"] = "-1";
    // obj["encode_type"] = "GSM7_default";

    QUrlQuery postData;
    postData.addQueryItem("isTest", "false");
    postData.addQueryItem("goformId", "SEND_SMS");
    postData.addQueryItem("notCallback","true");
    postData.addQueryItem("Number","%2B79818398770");
    postData.addQueryItem("sms_time",QDateTime::currentDateTime()
                                        .toString("yy%3BMM%3Bdd%3Bhh%3Bmm%3Bss%3B%2B3")
                          );
    postData.addQueryItem("MessageBody","0063006800650063006B");
    postData.addQueryItem("ID","-1");
    postData.addQueryItem("encocde_type","GSM7_default");

    QString pDataStr{postData.toString(QUrl::FullyEncoded)};
    QByteArray debug{pDataStr.toUtf8()};
    manager->post(request, pDataStr.toUtf8());
    // manager->get(QNetworkRequest(
    //     URL + QString("/isTest=false"
    //                   "&goformId=SEND_SMS"
    //                   "&notCallback=true"
    //                   "&Number=%2B79818398770"
    //                   "&sms_time=%1"
    //                   "&MessageBody=0063006800650063006B"
    //                   "&ID=-1&encode_type=GSM7_default")
    //               .arg(QDateTime::currentDateTime()
    //                        .toString("yy%3BMM%3Bdd%3Bhh%3Bmm%3Bss%3B%2B3")
    //                                 //24%3B05%3B18%3B16%3B48%3B44%3B%2B3
    //                    )
    //     ));
>>>>>>> origin/addValidation
    return ResponseAnsw::Success;
}

void msend::MessageSender::rep(QNetworkReply *reply)
{
    QString answer = reply->readAll();
    qDebug() << answer;
}
