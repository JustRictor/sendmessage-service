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
    static inline const QString INTERNAL_TOKEN{
        "66c245eb9cf96e2cb886aa112745cb1ba0917fe8"
    };
    static inline QList<QString> tokens{
        INTERNAL_TOKEN, ///<main admin token
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
