#ifndef __HTTP_SERVER_HPP_
#define __HTTP_SERVER_HPP_

#include <QObject>

class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(QObject *parent = nullptr);

signals:
};

#endif // __HTTP_SERVER_HPP_
