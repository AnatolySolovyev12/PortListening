#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QTime>

QTextStream out(stdout);

MyTcpServer::MyTcpServer(QObject* parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);

    dataWrite = new SQLiteDB();

    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    QTimer::singleShot(500, [this]() {

        if (!mTcpServer->listen(QHostAddress::Any, 6000)) // слушаем с любого адреса на порт 6000. Можно указать определённый host для прослушивания
        {
            emit messegeLog("server is not started\n");
            // qDebug() << "server is not started\n";
        }
        else
        {
            emit messegeLog("server is started\n");
            // qDebug() << "server is started\n";
        }

        });

}

void MyTcpServer::slotNewConnection()
{
    mTcpSocket = mTcpServer->nextPendingConnection(); // возвращает объект QTcpSocket для текущего соединения. Вернёт nullptr если вызвать эту функцию без наличия соединения. Лучше потом удалять QTcpSocket указатель и по итогу занулять.

  //  mTcpSocket->write("Echo server!\r\n");

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

       // qDebug() << curDate.toString("dd-MM-yyyy");

        emit messegeLog(curDate.toString("dd-MM-yyyy") + " " + curTime.toString());

        qDebug() << curDate.toString("dd-MM-yyyy") << " " << curTime.toString();

       // qDebug() << QByteArray::fromHex(array);
       // emit messegeLog(QByteArray::fromHex(array));

       // emit messegeLog(array);
       //  qDebug() << array;

       //  mTcpSocket->write(array); // Эхо эффект с отправкой принятого обратно сокету

        QString str = array.toHex();

       // qDebug() << str << "\n";

        emit messegeLog("Str size = " + QString::number(str.size()));

        qDebug() << "Str size = " << str.size();



        
        ///////////////////////////////////

        QString temporary;

        QString middleString;

        QString translate;

        int counter = 0;

        bool ok;

        QList <QString> myListTest;

        for (auto val : str)
        {
            ++counter;

            temporary += val;

            if (counter == 2)
            {
                myListTest.append(temporary);
                temporary = "";
                counter = 0;
            }
        }

        temporary = "";

        for (auto& val : myListTest)
            temporary += val + " ";
       // out << Qt::endl;
       // out << Qt::endl;

        emit messegeLog(temporary);

        temporary = "";

        middleString = "";

        translate = "";

        counter = 0;
        ///////////////////////////////////////////////



        if (str.size() != 312) // out-of-array warning
            continue;
        /*
        QString temporary;

        QString middleString;

        QString translate;

        int counter = 0;

        bool ok;
        */
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

        for (auto& val : myList)
            temporary += val + " ";

        emit messegeLog(temporary);

        qDebug() << temporary + '\n';

        ///////////////////////////////////////




        if (str.size() < 310) // out-of-array warning
        {
            emit messegeLog("");
            continue;
        }

         temporary = "";

         middleString = "";

         translate = "";

         counter = 0;
/*
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
*/
        QString numberStr;

        for (int counter = 3; counter >= 0; counter--)
        {

            numberStr += myList[counter];

        }

        uint valTrans = numberStr.toUInt(&ok, 16);

        emit messegeLog("Number - " + QString::number(valTrans));
        //qDebug() << "Number - " << valTrans;

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
         emit messegeLog("first - " + QString::number(valTrans));
        //qDebug() << "first - " << valTrans;

         valTrans = two.toUInt(&ok, 16);
         emit messegeLog("two - " + QString::number(valTrans));
        //qDebug() << "two - " << valTrans;

         valTrans = three.toUInt(&ok, 16);
         emit messegeLog("three - " + QString::number(valTrans));
        //qDebug() << "three - " << valTrans;

         valTrans = four.toUInt(&ok, 16);
         emit messegeLog("four - " + QString::number(valTrans) + '\n');
        //qDebug() << "four - " << valTrans << "\n";

        
		QString str_t = QString("INSERT INTO channelTable(number, date, channelFirst, channelSecond, channelThird, channelFour) VALUES('%1', '%2', '%3', '%4', '%5', '%6')") // VALUES - определяет те значения которые будут записаниы в строку
			.arg(numberStr.toUInt(&ok, 16))
            .arg(curDate.toString("yyyy-MM-dd"))
			.arg(first.toUInt(&ok, 16))
			.arg(two.toUInt(&ok, 16))
			.arg(three.toUInt(&ok, 16))
			.arg(four.toUInt(&ok, 16));

		dataWrite->writeData(str_t);
	}
}

void MyTcpServer::slotClientDisconnected()
{
    mTcpSocket->close(); // создлаёт сигнал void QIODevice::aboutToClose() а затем устанавливает для OpenMode состояние NotOpen.
    delete mTcpSocket;
    mTcpSocket = nullptr;
}

SQLiteDB* MyTcpServer::returnPtrDb()
{
    return dataWrite;
}