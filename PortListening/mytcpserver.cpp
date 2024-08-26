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
        qDebug() << "server is not started";
    }
    else {
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection()
{
    mTcpSocket = mTcpServer->nextPendingConnection(); // возвращает объект QTcpSocket для текущего соединения. Вернёт nullptr если вызвать эту функцию без наличия соединения. Лучше потом удалять QTcpSocket указатель и по итогу занулять.

    mTcpSocket->write("Hello, World!!! I am echo server!\r\n");

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

        qDebug() << array;

        // mTcpSocket->write(array); Эхо эффект с отправкой принятого обратно сокету

        QString str = array;

        QString temporary;

        QString middleString;

        QString translate;

        int counter = 0;

        bool ok;

        for (QChar val : str)
        {
            if (val == 'x')
            {
                temporary += " ";
                counter = 2;
                continue;
            }

            if (val == '\\')
            {
                continue;
            }

            if (counter > 0)
            {
                temporary += val;

                if (counter == 1)
                    temporary += " ";

                --counter;

                continue;
            }

            if (counter == 0)
            {
                temporary += val;
            }
        }

        qDebug() << temporary;
        middleString = temporary;

        middleString.remove(0,0);

        temporary = "";


        QList <QString> myList;

        for (auto val : middleString)
        {
            if (val.isSpace() && temporary != "")
            {
                myList.append(temporary);
                temporary = "";
                continue;
            }

            if (val.isSpace() && temporary == "")
                continue;

            temporary += val;
        }

        for (auto& val : myList)
        {
            
            if (val.size() == 2)
            {
                uint valTrans = val.toUInt(&ok, 16);
                translate += temporary.setNum(valTrans);

            }
            else
                translate += val;

            translate += " ";
            
        }

        qDebug() << translate << "\n";
    }
}


void MyTcpServer::slotClientDisconnected()
{
    mTcpSocket->close(); // создлаёт сигнал void QIODevice::aboutToClose() а затем устанавливает для OpenMode состояние NotOpen.
    delete mTcpSocket;
    mTcpSocket = nullptr;
}