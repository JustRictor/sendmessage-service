#include "message_validator.hpp"

MessageValidator::MessageValidator(QObject *parent)
    : QObject{parent}
{
    static bool isValid(QString number) {
            return (number[0] == '7' || number[0] == '8') && (size(number) == 11)
    }
}
