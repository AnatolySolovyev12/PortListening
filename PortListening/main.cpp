#include <QCoreApplication>
#include "mytcpserver.h"

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    MyTcpServer server;
    
    
    /*
    QByteArray valStr = "82 00 D9 17 04 02 10 3E 03 10 25 06 10 0B 07 10 1B 31 75 37 38 00 4D 49 4C 41 4E 20 52 46 20 30 34 00 00 00 00 31 39 30 30 34 30 34 30 30 30 39 37 34 31 30 17 0B 14 B5 09 00 00 00 00 4E E3 1B B6 17 00 E2 0A 00 00 00 00 0C 01 01 11 01 00 9A 12 00 00 00 00 00 00 47 16 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 62 00 0A 00 60 00 5A 00 1E 2C 0D 1B 08 18 04 00 00 14 04 05 00 00 06 13 00 9F 06 00 1F 21 00 00 D4 2D 46 52 44 4D 01 04 64 00 2C 01 64 00 85 7F";
   
    qDebug() << valStr;

    qDebug() << QByteArray::fromHex(valStr);

    QByteArray middleBa = QByteArray::fromHex(valStr).trimmed();

    qDebug() << middleBa;
    */

    
    //QByteArray text = QByteArray::fromHex(valStr);
   // qDebug() << text.data();            // returns "Qt is great!"

  // qDebug() << QByteArray::fromHex(valStr);
    /*
    for (int i = 0; i < valStr.size(); i++)
    {

        if (valStr[i] == '\\')
            valStr[i] = '/';

        if (valStr[i] == 'x')
            valStr[i] = ' ';
    }
    
    qDebug() << valStr;

    bool ok;
    uint valTrans = valStr.toUInt(&ok, 16);

    qDebug() << valTrans;
    */


    return a.exec();
}