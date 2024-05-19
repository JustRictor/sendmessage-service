#ifndef __API_MANAGE_HPP_
#define __API_MANAGE_HPP_

#include <QObject>
#include <QHash>
#include <QList>
#include <QString>
#include <QFile>

#include "logger/console_logger.hpp"

namespace api
{

class ApiManage : public QObject
{
    Q_OBJECT
private:
    static inline QList<QString> tokens{
        "24d8389ec9fd8a2dbacd672122d13fbf112e01b0a164a993f5d46cf62bfcf0ad", ///<main admin token
    };
    ConsoleLogger* logger;
    QFile tokensFile;
public:
    explicit ApiManage(QObject *parent = nullptr);
    static bool isValid(QString const &token);

    QList<QString> const& getTokens() const;
    QString addToken();
    bool delToken(QString const& token);
private:
    static QString genToken();
    void updateTokens();
signals:
};

}

#endif // API_MANAGE_HPP_
