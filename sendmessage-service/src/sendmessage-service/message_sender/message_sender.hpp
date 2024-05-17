#ifndef __MESSAGE_SENDER_HPP_
#define __MESSAGE_SENDER_HPP_

#include <QObject>

class MessageSender : public QObject
{
    Q_OBJECT
public:
    explicit MessageSender(QObject *parent = nullptr);

signals:
};

#endif // __MESSAGE_SENDER_HPP_
