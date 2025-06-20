#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QTime>

QTextStream out(stdout);

MyTcpServer::MyTcpServer(int any, QObject* parent) : QObject(parent), port(any)
{
	mTcpServer = new QTcpServer(this);

	dataWrite = new SQLiteDB();

	connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

	if (port = 6000)
	{
		serialBuff = { "75024", "75001", "74986", "74981", "74995", "74998", "75008", "74980", "75000", "74992" };
	}
	if (port = 49500)
	{
		serialBuff = { "74985", "75020", "74987", "74991", "74988", "74982", "74989", "74990"};
	}
	if (port = 49501)
	{
		serialBuff = { "74993", "74984", "74996", "75002", "74983", "75014", "74997", "74994"};
	}


	QTimer::singleShot(500, [this]() {

		if (!mTcpServer->listen(QHostAddress::Any, port)) // слушаем с любого адреса на порт 6000. Можно указать определённый host для прослушивания
		{
			emit messegeLog("server with port " + QString::number(port) + " is not started\n");
		}
		else
		{
			emit messegeLog("server with port " + QString::number(port) + " is started\n");
		}

		});

	for (int val = 0; val <= 44; val++)
	{
		zeroBuff.push_back("00");
	}


	/*

	QTimer::singleShot(300, [this]() {

		QByteArray testNumber = "74995";

		QByteArray data1 = QByteArray::fromHex("0800FFFFFFFFFFFF");

		data1.push_front(QByteArray::fromHex(serialArrayRotate(testNumber)));

		QString crc1 = crc16Modbus(data1);

		emit messegeLog(data1.toHex().toUpper() + crc1);


		});

		*/
}

void MyTcpServer::slotNewConnection()
{
	mTcpSocket = mTcpServer->nextPendingConnection(); // возвращает объект QTcpSocket для текущего соединения. Вернёт nullptr если вызвать эту функцию без наличия соединения. Лучше потом удалять QTcpSocket указатель и по итогу занулять.

	if (!mTcpSocket) // проверка на некорректное использование
	{
		emit messegeLog("No pending connection\n");
		return; // Выход из функции, если нет соединения
	}

	connect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead); // если есть что читать (библиотечный сигнал) сработает слот
	connect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected); // если сокет отсоединился (библиотечный сигнал) сработает слот

	QDate curDate = QDate::currentDate();
	QTime curTime = QTime::currentTime();

	QString temp = "\nConnect from host " + mTcpSocket->peerAddress().toString().sliced(7) + " - " + curDate.toString("dd-MM-yyyy") + " " + curTime.toString(); // для анализа входящих подключений
	emit messegeLog(temp);
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

		QDate curDate = QDate::currentDate();
		QTime curTime = QTime::currentTime();

		emit messegeLog('\n' + QString::number(port) + " - " + curDate.toString("dd-MM-yyyy") + " " + curTime.toString());

		//  mTcpSocket->write(array); // Эхо эффект с отправкой принятого обратно сокету

		QString str = array.toHex();

		emit messegeLog("Str size = " + QString::number(str.size()));

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

		emit messegeLog(temporary);

		temporary = "";

		middleString = "";

		translate = "";

		counter = 0;


		/*
		if (array == "35" && listen)
		{
			counter--;
		}
		*/


		if ((str.size() == 4) || (str.size() == 16 && listen) || (str.size() == 26 && listen))
		{
			if (str.size() == 4 && listen)
			{
				if (recall == 4)
				{
					recall = 0;
					serialBuffPosition++;
					countMessege = 0;
					oldMessege = false;
					listen = false;
				}
				if (listen)
				{
					countMessege--;
					recall++;
					oldMessege = true;
				}
			}

			if (serialBuffPosition + 1 > serialBuff.length())
			{
				serialBuffPosition = 0;
				return;
			}

			QByteArray testNumber = serialBuff[serialBuffPosition];

			if (oldMessege)
				countMessege++;
			else
			{
				countMessege++;
				recall = 0;
			}


			QByteArray data1;

			switch (countMessege)
			{
			case(1):
			{
				data1 = QByteArray::fromHex("0800FFFFFFFFFFFF");
				listen = true;
				break;
			}
			case(2):
			{
				data1 = QByteArray::fromHex("0105");
				listen = true;
				break;
			}
			case(3):
			{
				answerListMilur += myList;
				data1 = QByteArray::fromHex("0106");
				listen = true;
				break;
			}
			case(4):
			{
				answerListMilur += zeroBuff;

				for (int tempVal = 4; tempVal <= 7; tempVal++)
				{
					answerListMilur.push_back(answerListMilur[tempVal]);
				}

				for (int tempVal = 4; tempVal <= 7; tempVal++)
				{
					answerListMilur.push_back(myList[tempVal]);
				}

				str += strZero;
				recall = 0;
				serialBuffPosition++;
				countMessege = 0;
				oldMessege = false;
				listen = false;
			}

			}

			if (str.length() != 202)
			{

				data1.push_front(QByteArray::fromHex(serialArrayRotate(testNumber)));

				QString crc1 = crc16Modbus(data1);

				data1 += QByteArray::fromHex(crc1.toUtf8());

				emit messegeLog(data1.toHex());

				mTcpSocket->write(data1);

				oldMessege = false;

				continue;
			}
		}



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
			for (int counter = 157; counter >= 118; --counter)
			{
				if (counter >= 150)
				{
					four += str[counter];
					continue;
				}

				if (counter >= 142)
				{
					three += str[counter];
					continue;
				}

				if (counter >= 134)
				{
					two += str[counter];
					continue;
				}

				if (counter >= 126)
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

	if (any.length() == 2 || any.length() == 1)
	{
		if (any.length() == 2)
			any.push_front("0.");
		else
			any.push_front("0.0");

	}
	else
	{
		any.insert((any.length() - 2), '.');
	}

	return any;
}


QString MyTcpServer::crc16Modbus(const QByteArray& data) // CRC16MODBUS для определения контрольной суммы в конце пакетов для Милур107
{
	quint16 crc = 0xFFFF;
	for (auto byte : data) {
		crc ^= static_cast<quint8>(byte);
		for (int i = 0; i < 8; ++i) {
			if (crc & 0x0001)
				crc = (crc >> 1) ^ 0xA001;
			else
				crc >>= 1;
		}
	}

	QString temp = QString("%1%2")
		.arg(crc & 0xFF, 2, 16, QChar('0'))
		.arg((crc >> 8) & 0xFF, 2, 16, QChar('0')).toUpper();

	return temp;
}


QByteArray MyTcpServer::serialArrayRotate(QByteArray testNumber)
{
	bool ok = false;
	int number = testNumber.toInt(&ok);

	QByteArray hexData = QByteArray::number(number, 16).toUpper();

	while (hexData.length() != 8)
		hexData.push_front("0");

	int count = 0;

	QByteArray tempStr;

	QByteArray arrForByte;

	for (auto& val : hexData)
	{
		tempStr += val;
		count++;

		if (count == 2)
		{
			arrForByte.push_front(tempStr);
			tempStr.clear();
			count = 0;
		}
	}

	return arrForByte;
}

