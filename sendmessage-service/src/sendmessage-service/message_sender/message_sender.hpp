#ifndef __MESSAGE_SENDER_HPP_
#define __MESSAGE_SENDER_HPP_

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include "message_sender_data.hpp"

namespace msend
{

class MessageSender : public QObject
{
    Q_OBJECT
private:
    static inline const QString URL{"http://192.168.0.1/goform/goform_set_cmd_process"};
    QNetworkAccessManager* manager;
    QNetworkRequest request;
public:
    explicit MessageSender(QObject *parent = nullptr);

    ResponseAnsw sendMessage(Tdo const& data);
private slots:
    void rep(QNetworkReply *reply);
signals:
};

}

#endif // __MESSAGE_SENDER_HPP_
