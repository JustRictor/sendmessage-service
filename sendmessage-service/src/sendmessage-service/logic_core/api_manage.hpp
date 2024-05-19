#ifndef __API_MANAGE_HPP_
#define __API_MANAGE_HPP_

#include <QObject>
#include <QHash>
#include <QList>
#include <QString>

#include "logger/console_logger.hpp"

namespace api
{

class ApiManage : public QObject
{
    Q_OBJECT
private:
    static inline QHash<QString, QVector<QString>> groups{
        { "admin", {"all"} },
        { "user ", {"sendMessage"} },
    };
    static inline QHash<QString, QVector<QString>> tokens{
        { "17a65071c496aff94e9ae6a296724beb", {"all"} }, ///<main admin token
    };
    ConsoleLogger* logger;
public:
    explicit ApiManage(QObject *parent = nullptr);
    bool isValid(QString const &token, QString const &find_method);

    QList<QString> getTokens() const;
    QString addToken();
    bool delToken(QString const& token);
private:
    static QString genToken();
signals:
};

}

#endif // API_MANAGE_HPP_
