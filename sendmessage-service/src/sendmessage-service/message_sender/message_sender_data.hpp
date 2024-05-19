#ifndef __MESSAGE_SENDER_DATA_HPP_
#define __MESSAGE_SENDER_DATA_HPP_

#include <QString>

namespace msend
{

struct Tdo
{
    QString phoneNum;
    QString message;
};

enum class ResponseAnsw
{
    CannotConnect,
    Success,
    Failure
};

}

#endif // __MESSAGE_SENDER_DATA_HPP_
