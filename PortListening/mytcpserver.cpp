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

	if (!mTcpSocket) // проверка на некорректное использование
	{
		emit messegeLog("No pending connection\n");
		return; // Выход из функции, если нет соединения
	}
	//  mTcpSocket->write("Echo server!\r\n");

	connect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead); // если есть что читать (библиотечный сигнал) сработает слот
	connect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected); // если сокет отсоединился (библиотечный сигнал) сработает слот
}

void MyTcpServer::slotServerRead()
{
	if (!mTcpSocket) {
		emit messegeLog("Socket is null in start slotServerRead\n");
		return;
	}

	while (mTcpSocket->bytesAvailable() > 0)
	{
		QByteArray array = mTcpSocket->readAll();

		if (array == "35")
			continue;

		QDate curDate = QDate::currentDate();
		QTime curTime = QTime::currentTime();

		// qDebug() << curDate.toString("dd-MM-yyyy");

		emit messegeLog('\n' + curDate.toString("dd-MM-yyyy") + " " + curTime.toString());

		// qDebug() << curDate.toString("dd-MM-yyyy") << " " << curTime.toString();

		// qDebug() << QByteArray::fromHex(array);
		// emit messegeLog(QByteArray::fromHex(array));

		// emit messegeLog(array);
		//  qDebug() << array;

		//  mTcpSocket->write(array); // Эхо эффект с отправкой принятого обратно сокету

		QString str = array.toHex();

		// qDebug() << str << "\n";

		emit messegeLog("Str size = " + QString::number(str.size()));

		// qDebug() << "Str size = " << str.size();


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

		for (auto& val : myList)
			temporary += val + " ";

		// out << Qt::endl;
		// out << Qt::endl;

		emit messegeLog(temporary);

		temporary = "";

		middleString = "";

		translate = "";

		counter = 0;

		if (str.size() == 312 || str.size() == 202) // out-of-array warning
		{
		}
		else
			continue;

		QString numberStr;

		if (myList.size() < 4) {
			emit messegeLog("Not enough elements in myList");
			continue;
		}

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

		if (str.size() == 312)
		{
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
			emit messegeLog("four - " + QString::number(valTrans));
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

		if (str.size() == 202)
		{
			for (int counter = 148; counter >= 109; --counter)
			{
				if (counter >= 141)
				{
					four += str[counter];
					continue;
				}

				if (counter >= 133)
				{
					three += str[counter];
					continue;
				}

				if (counter >= 125)
				{
					two += str[counter];
					continue;
				}

				if (counter >= 117)
				{
					first += str[counter];
					continue;
				}
			}


			emit messegeLog("first - " + converFuncString(first));
			//qDebug() << "first - " << valTrans;

			emit messegeLog("two - " + converFuncString(two));
			//qDebug() << "two - " << valTrans;

			emit messegeLog("three - " + converFuncString(three));
			//qDebug() << "three - " << valTrans;

			emit messegeLog("four - " + converFuncString(four));
			//qDebug() << "four - " << valTrans << "\n";


			QString str_t = QString("INSERT INTO counterTable(number, date, channelFirst, channelSecond, channelThird, channelFour) VALUES('%1', '%2', '%3', '%4', '%5', '%6')") // VALUES - определяет те значения которые будут записаниы в строку
				.arg(numberStr.toUInt(&ok, 16))
				.arg(curDate.toString("yyyy-MM-dd"))
				.arg(first)
				.arg(two)
				.arg(three)
				.arg(four);

			dataWrite->writeData(str_t);
		}
	}

	/*
	// проверка на проблемы с некорректным использованием указателей

	if (mTcpSocket == nullptr) return;

	mTcpSocket->close(); // создлаёт сигнал void QIODevice::aboutToClose() а затем устанавливает для OpenMode состояние NotOpen.
	delete mTcpSocket;
	mTcpSocket = nullptr;
	*/
}

void MyTcpServer::slotClientDisconnected()
{
	// ntcnbhetv на проблемы с некорректным использованием указателей

	if (mTcpSocket == nullptr)
	{
		emit messegeLog("Was disconnect but mTcpSocket was nullptr\n");
		return;
	}

	mTcpSocket->close(); // создлаёт сигнал void QIODevice::aboutToClose() а затем устанавливает для OpenMode состояние NotOpen.
	delete mTcpSocket;
	mTcpSocket = nullptr;
}

SQLiteDB* MyTcpServer::returnPtrDb()
{
	return dataWrite;
}


QString MyTcpServer::converFuncString(QString& any)
{
	int testInt = any.toInt();

	any = QString::number(testInt);

	if (any.length() == 2)
	{
		any.push_front("0.");

	}
	else
	{
		any.insert((any.length() - 2), '.');
	}

	return any;
}