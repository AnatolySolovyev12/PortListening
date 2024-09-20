#include <QCoreApplication>
#include "mytcpserver.h"
#include <QTextStream>
#include <qfile.h>

QTextStream myout(stdout);

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    MyTcpServer server;
    
    return a.exec();
}