#ifndef __MESSAGE_VALIDATOR_HPP_
#define __MESSAGE_VALIDATOR_HPP_

#include <QObject>

class MessageValidator : public QObject
{
    Q_OBJECT
public:
    explicit MessageValidator(QObject *parent = nullptr);
    static bool isValid(QString const& number);
signals:
};

#endif // __MESSAGE_VALIDATOR_HPP_
