#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QTime>

QTextStream out(stdout);

MyTcpServer::MyTcpServer(QObject* parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 6000)) // слушаем с любого адреса на порт 6000. Можно указать определённый host для прослушивания
    {
        qDebug() << "server is not started\n";
    }
    else {
        qDebug() << "server is started\n";
    }
}

void MyTcpServer::slotNewConnection()
{
    mTcpSocket = mTcpServer->nextPendingConnection(); // возвращает объект QTcpSocket для текущего соединения. Вернёт nullptr если вызвать эту функцию без наличия соединения. Лучше потом удалять QTcpSocket указатель и по итогу занулять.

  //  mTcpSocket->write("Hello, World!!! I am echo server!\r\n");

    connect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead); // если есть что читать (библиотечный сигнал) сработает слот
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected); // если сокет отсоединился (библиотечный сигнал) сработает слот
}

void MyTcpServer::slotServerRead()
{
    
    while (mTcpSocket->bytesAvailable() > 0)
    {
        QByteArray array = mTcpSocket->readAll();
        
        if (array == "35")
            continue;

        QDate curDate = QDate::currentDate();
        QTime curTime = QTime::currentTime();

        qDebug() << curDate.toString() << " " << curTime.toString() << "\n";

        qDebug() << array << "\n";

        // mTcpSocket->write(array); Эхо эффект с отправкой принятого обратно сокету

        QString str = array.toHex();

        qDebug() << str << "\n";

        qDebug() << "Str size = " << str.size() << "\n";

        if (str.size() < 200) // out-of-array warning
            continue;

        QString temporary;

        QString middleString;

        QString translate;

        int counter = 0;

        bool ok;

        QList <QString> myList;

        for (auto val : str)
        {
            ++counter;

            temporary += val;

            if (counter == 2)
            {
                myList.append(temporary);
                temporary = "";
                counter = 0;
            }
        }

        QString numberStr;

        for (int counter = 3; counter >= 0; counter--)
        {

            numberStr += myList[counter];

        }

        uint valTrans = numberStr.toUInt(&ok, 16);
        qDebug() << "Number - " << valTrans;

        QString first;
        QString two;
        QString three;
        QString four;
        
        for (int counter = 94; counter >= 79; --counter)
        {
            if (counter >= 91)
            {
                four += myList[counter];
                continue;
            }

            if (counter >= 87)
            {
                three += myList[counter];
                continue;
            }

            if (counter >= 83)
            {
                two += myList[counter];
                continue;
            }

            if (counter >= 79)
            {
                first += myList[counter];
                continue;
            }
        }
        
         valTrans = first.toUInt(&ok, 16);
        qDebug() << "first - " << valTrans;

         valTrans = two.toUInt(&ok, 16);
        qDebug() << "two - " << valTrans;

         valTrans = three.toUInt(&ok, 16);
        qDebug() << "three - " << valTrans;

         valTrans = four.toUInt(&ok, 16);
        qDebug() << "four - " << valTrans << "\n";
    } 
}

void MyTcpServer::slotClientDisconnected()
{
    mTcpSocket->close(); // создлаёт сигнал void QIODevice::aboutToClose() а затем устанавливает для OpenMode состояние NotOpen.
    delete mTcpSocket;
    mTcpSocket = nullptr;
}