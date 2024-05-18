#include <QCoreApplication>
#include "http_server/http_server.hpp"
#include "message_sender/message_sender.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    msend::MessageSender core;
    core.sendMessage({
        .phoneNum = 79818398770,
        .message  = "check"
    });
  
    return a.exec();
}
