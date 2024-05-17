#ifndef __CONSOLE_LOGGER_HPP_
#define __CONSOLE_LOGGER_HPP_

#include <QObject>

class ConsoleLogger : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleLogger(QObject *parent = nullptr);

signals:
};

#endif // __CONSOLE_LOGGER_HPP_
