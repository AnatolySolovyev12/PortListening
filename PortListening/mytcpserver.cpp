#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>

#include <QTime>


QTextStream out(stdout);

MyTcpServer::MyTcpServer(QObject* parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 6000)) // ������� � ������ ������ �� ���� 6000. ����� ������� ����������� host ��� �������������
    {
        qDebug() << "server is not started";
    }
    else {
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection()
{
    mTcpSocket = mTcpServer->nextPendingConnection(); // ���������� ������ QTcpSocket ��� �������� ����������. ����� nullptr ���� ������� ��� ������� ��� ������� ����������. ����� ����� ������� QTcpSocket ��������� � �� ����� ��������.

    mTcpSocket->write("Hello, World!!! I am echo server!\r\n");

    connect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead); // ���� ���� ��� ������ (������������ ������) ��������� ����
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected); // ���� ����� ������������ (������������ ������) ��������� ����
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

        // mTcpSocket->write(array); ��� ������ � ��������� ��������� ������� ������

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
    mTcpSocket->close(); // ������� ������ void QIODevice::aboutToClose() � ����� ������������� ��� OpenMode ��������� NotOpen.
    delete mTcpSocket;
    mTcpSocket = nullptr;
}