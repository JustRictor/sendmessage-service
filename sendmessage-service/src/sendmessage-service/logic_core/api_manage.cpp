#include "api_manage.hpp"
#include <algorithm>
#include <QDateTime>
#include <QCryptographicHash>

api::ApiManage::ApiManage(QObject *parent)
    : QObject{parent}
    , logger( &ConsoleLogger::getInstance() )
{}


bool api::ApiManage::isValid(
    QString const &token,
    QString const &find_method
    )
{
  if (!tokens.contains(token))
    return false;
  return std::find_if(tokens[token].cbegin(), tokens[token].cend(),
                      [&find_method](const QString& method){
        return find_method == method || method == "all";
    }) != tokens[token].cend();
}

QList<QString> api::ApiManage::getTokens() const
{
    return tokens.keys();
}

QString api::ApiManage::addToken()
{
    QString token = genToken();
    while(tokens.find(token) != tokens.cend())
        token = genToken();
    logger->log(QString("gen new token: %1").arg(token));
    return token;
}

bool api::ApiManage::delToken(QString const& token)
{
    if(tokens.find(token) == tokens.cend())
        return false;
    tokens.erase(tokens.find(token));
    logger->log(QString("erase token %1").arg(token));
    return true;
}

QString api::ApiManage::genToken()
{
    return QCryptographicHash::hash(
        QDateTime::currentDateTime()
            .toString("ddd:dd:MMMM:yyyy:hh:mm:ss:zzz").toUtf8(),
        QCryptographicHash::Algorithm::Sha1
        );
}
