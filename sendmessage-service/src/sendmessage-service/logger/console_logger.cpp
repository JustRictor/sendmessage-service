#include "console_logger.hpp"
#include <QDateTime>
#include <QDebug>

ConsoleLogger::ConsoleLogger(QObject *parent)
    : QObject{parent}
    , logFile("sendmessage-service.log")
{
    if(!logFile.open(QFile::OpenModeFlag::Append))
        qDebug() << QDateTime::currentDateTime().toString("dd:MM:yy hh:mm:ss -")
                 << "cant create/open log file";
}

void ConsoleLogger::log(const QString &message)
{
    QString logmess = QString("%1 - %2")
                          .arg( QDateTime::currentDateTime().toString("dd:MM:yy hh:mm:ss") )
                          .arg( message )
        ;
    qDebug() << logmess;
    logFile.write(logmess.toUtf8());
}
