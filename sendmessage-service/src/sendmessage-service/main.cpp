#include <QCoreApplication>
#include "http_server/http_server.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HttpServer core;
  
    return a.exec();
}
