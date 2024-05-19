#include "message_validator.hpp"

MessageValidator::MessageValidator(QObject *parent)
    : QObject{parent}
{}

bool MessageValidator::isValid(QString const& number) {
    return (number[0] == '7' || number[0] == '8')
           && (number.length() == 11);
}
