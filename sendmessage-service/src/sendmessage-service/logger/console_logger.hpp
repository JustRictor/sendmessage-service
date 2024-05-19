#ifndef __CONSOLE_LOGGER_HPP_
#define __CONSOLE_LOGGER_HPP_

#include <QObject>
#include <QString>
#include <QFile>

class ConsoleLogger : public QObject
{
    Q_OBJECT
private:
    QFile logFile;
public:
    static ConsoleLogger& getInstance() {
        static ConsoleLogger instance{};
        return instance;
    }
    void log(QString const& message);

private:
    explicit ConsoleLogger(QObject *parent = nullptr)
        : QObject{parent}
    {}
    explicit ConsoleLogger(ConsoleLogger& other) = delete;
    explicit ConsoleLogger(ConsoleLogger&& other) = delete;
    ConsoleLogger operator=(ConsoleLogger& other) = delete;
    ConsoleLogger operator=(ConsoleLogger&& other) = delete;

signals:
};

#endif // __CONSOLE_LOGGER_HPP_
