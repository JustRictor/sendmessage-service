#include "api_manage.hpp"
#include <QDateTime>
#include <QCryptographicHash>
#include <QDebug>

api::ApiManage::ApiManage(QObject *parent)
    : QObject{parent}
    , logger( &ConsoleLogger::getInstance() )
    , tokensFile("tokens")
{
    if(!tokensFile.open(QFile::OpenModeFlag::Append))
        logger->log("cant create/open tokens file");
    QTextStream in(&tokensFile);
    while(!in.atEnd()) {
        QString token = in.readLine();
        if(tokens.contains(token)) continue;
        tokens.append(in.readLine());
    }
    tokensFile.close();
}


bool api::ApiManage::isValid(QString const &token)
{
    return tokens.contains(token);
}

QList<QString> const& api::ApiManage::getTokens() const
{
    return tokens;
}

QString api::ApiManage::addToken()
{
    QString token = genToken();
    while(tokens.contains(token))
        token = genToken();
    logger->log(QString("gen new token: %1").arg(token));
    tokens.append(token);
    updateTokens();
    return token;
}

bool api::ApiManage::delToken(QString const& token)
{
    if(!tokens.contains(token) && token != INTERNAL_TOKEN)
        return false;
    tokens.removeAll(token);
    logger->log(QString("erase token %1").arg(token));
    updateTokens();
    return true;
}

QString api::ApiManage::genToken()
{
    return QCryptographicHash::hash(
        QDateTime::currentDateTime()
            .toString("ddd:dd:MMMM:yyyy:hh:mm:ss:zzz").toUtf8(),
            QCryptographicHash::Algorithm::Sha1
        ).toHex();
}

void api::ApiManage::updateTokens()
{
    if(!tokensFile.open(QFile::OpenModeFlag::WriteOnly))
        logger->log("cant create/open tokens file");
    for(QString str : tokens)
        tokensFile.write(QString("%1\n").arg(str).toUtf8());
    tokensFile.close();
}
