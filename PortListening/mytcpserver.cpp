#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QTime>

QTextStream out(stdout);

MyTcpServer::MyTcpServer(int any, QObject* parent) : QObject(parent), port(any), dateTImer(new QTimer(this))
{
	mTcpServer = new QTcpServer(this);

	dataWrite = new SQLiteDB();

	connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

	todayDate = QDate::currentDate().toString("dd-MM-yyyy");

	dateTImer->start(600000);

	connect(dateTImer, &QTimer::timeout, this, &MyTcpServer::newDayBuffer);

	QTimer::singleShot(500, [this]() {

		if (!mTcpServer->listen(QHostAddress::Any, port))
		{
			emit messegeLog("\nserver with port " + QString::number(port) + " is not started", QColor(240, 14, 14));
		}
		else
		{
			emit messegeLog("\nserver with port " + QString::number(port) + " is started", QColor(255, 128, 0));
		}

		readDeviceFile();

		fullSerialBuffConstant = serialBuff;

		checkTodayValues();
		});
}

void MyTcpServer::slotNewConnection()
{
	mTcpSocket = mTcpServer->nextPendingConnection();

	if (!mTcpSocket) // ïðîâåðêà íà íåêîððåêòíîå èñïîëüçîâàíèå
	{
		emit messegeLog("No pending connection\n", QColor(240, 14, 14));
		return; // Âûõîä èç ôóíêöèè, åñëè íåò ñîåäèíåíèÿ
	}

	connect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead); // åñëè åñòü ÷òî ÷èòàòü (áèáëèîòå÷íûé ñèãíàë) ñðàáîòàåò ñëîò
	connect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected); // åñëè ñîêåò îòñîåäèíèëñÿ (áèáëèîòå÷íûé ñèãíàë) ñðàáîòàåò ñëîò

	QDate curDate = QDate::currentDate();
	QTime curTime = QTime::currentTime();

	QString temp = "\nConnect from host " + mTcpSocket->peerAddress().toString().sliced(7) + " - " + curDate.toString("dd-MM-yyyy") + " " + curTime.toString(); // äëÿ àíàëèçà âõîäÿùèõ ïîäêëþ÷åíèé
	emit messegeLog(temp, QColor(255, 128, 0));
}

void MyTcpServer::slotServerRead()
{
	if (!mTcpSocket) {
		emit messegeLog("Socket is null in start slotServerRead\n", QColor(240, 14, 14));
		return;
	}

	while (mTcpSocket->bytesAvailable() > 0)
	{
		QByteArray array = mTcpSocket->readAll();

		QDate curDate = QDate::currentDate();
		QTime curTime = QTime::currentTime();

		emit messegeLog('\n' + QString::number(port) + " - " + curDate.toString("dd-MM-yyyy") + " " + curTime.toString(), QColor(240, 218, 15));

		//  mTcpSocket->write(array); // Ýõî ýôôåêò ñ îòïðàâêîé ïðèíÿòîãî îáðàòíî ñîêåòó

		QString str = array.toHex();

		emit messegeLog("Str size = " + QString::number(str.size()), QColor(193, 128, 167));

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

		emit messegeLog("RX - " + temporary, QColor(100, 188, 221));

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
					TwoZero = false;
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

			if (threeFazeBuff.indexOf(testNumber) >= 0 || threeFazeBuffTwoZero.indexOf(testNumber) >= 0)
			{
				treeFazeBool = true;

				if (threeFazeBuffTwoZero.indexOf(testNumber) >= 0)
					TwoZero = true;
			}
			else
				treeFazeBool = false;


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
				if (treeFazeBool)
					data1 = QByteArray::fromHex("0177");
				else
					data1 = QByteArray::fromHex("0105");

				listen = true;
				break;
			}
			case(3):
			{
				answerListMilur += str;

				if (treeFazeBool)
					data1 = QByteArray::fromHex("0178");
				else
					data1 = QByteArray::fromHex("0106");

				listen = true;
				break;
			}
			case(4):
			{
				answerListMilur += zeroBuff;

				for (int tempVal = 14; tempVal <= 21; tempVal++)
				{
					answerListMilur.push_back(answerListMilur[tempVal]);
				}

				for (int tempVal = 14; tempVal <= 21; tempVal++)
				{
					answerListMilur.push_back(str[tempVal]);
				}

				answerListMilur += strZero;
				str = answerListMilur;
				answerListMilur.clear();
				recall = 0;
				serialBuffPosition++;
				countMessege = 0;
				oldMessege = false;
				listen = false;
			}

			}

			if (listen)
			{
				data1.push_front(QByteArray::fromHex(serialArrayRotate(testNumber)));

				QString crc1 = crc16Modbus(data1);

				data1 += QByteArray::fromHex(crc1.toUtf8());

				emit messegeLog("TX - " + data1.toHex(), QColor(57, 233, 20));

				mTcpSocket->write(data1);

				oldMessege = false;

				continue;
			}

			emit messegeLog(QString::number(str.length()) + " - " + str, QColor(57, 233, 20));
		}

		if (str.size() == 206 || str.size() == 210 || str.size() == 214 || str.size() == 218) str.replace(QRegularExpression(pattern), "");

		if (str.size() == 404) str.remove(202, 404);

		if (str.size() == 406)
		{
			str.replace(QRegularExpression(pattern), "");
			str.remove(202, 404);
		}

		if (str.size() == 606) str.remove(202, 606);

		if (str.size() == 610)
		{
			str.replace(QRegularExpression(pattern), "");
			str.remove(202, 404);
		}

		if (str.size() == 286) str = str.sliced(84);

		if (str.size() == 244) str = str.sliced(42); //front sliced

		if (str.size() == 312 || str.size() == 202) // out-of-array warning
		{
		}
		else
			continue;

		QString numberStr;

		if (myList.size() < 4) {
			emit messegeLog("Not enough elements in myList", QColor(240, 14, 14));
			continue;
		}

		for (int counter = 3; counter >= 0; counter--)
		{

			numberStr += myList[counter];

		}

		uint valTrans = numberStr.toUInt(&ok, 16);

		emit messegeLog("Number - " + QString::number(valTrans) + " - queue polling = " + QString::number(serialBuff.length() - 1), QColor(57, 233, 20));

		if (serialBuff.indexOf(QString::number(valTrans)) >= 0)
		{
			serialBuff.remove(serialBuff.indexOf(QString::number(valTrans))); // удаляем очереди опроса то что опрошено
		}

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
			emit messegeLog("first - " + QString::number(valTrans), QColor(57, 233, 20));
			//qDebug() << "first - " << valTrans;

			valTrans = two.toUInt(&ok, 16);
			emit messegeLog("two - " + QString::number(valTrans), QColor(57, 233, 20));
			//qDebug() << "two - " << valTrans;

			valTrans = three.toUInt(&ok, 16);
			emit messegeLog("three - " + QString::number(valTrans), QColor(57, 233, 20));
			//qDebug() << "three - " << valTrans;

			valTrans = four.toUInt(&ok, 16);
			emit messegeLog("four - " + QString::number(valTrans), QColor(57, 233, 20));
			//qDebug() << "four - " << valTrans << "\n";


			QString str_t = QString("INSERT INTO channelTable(number, date, channelFirst, channelSecond, channelThird, channelFour) VALUES('%1', '%2', '%3', '%4', '%5', '%6')") // VALUES - îïðåäåëÿåò òå çíà÷åíèÿ êîòîðûå áóäóò çàïèñàíèû â ñòðîêó
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

			emit messegeLog("first - " + converFuncString(first), QColor(57, 233, 20));
			//qDebug() << "first - " << valTrans;

			emit messegeLog("two - " + converFuncString(two), QColor(57, 233, 20));
			//qDebug() << "two - " << valTrans;

			emit messegeLog("three - " + converFuncString(three), QColor(57, 233, 20));
			//qDebug() << "three - " << valTrans;

			emit messegeLog("four - " + converFuncString(four), QColor(57, 233, 20));
			//qDebug() << "four - " << valTrans << "\n";

			TwoZero = false;

			QString str_t = QString("INSERT INTO counterTable(number, date, channelFirst, channelSecond, channelThird, channelFour) VALUES('%1', '%2', '%3', '%4', '%5', '%6')") // VALUES - îïðåäåëÿåò òå çíà÷åíèÿ êîòîðûå áóäóò çàïèñàíèû â ñòðîêó
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
	if (mTcpSocket == nullptr)
	{
		emit messegeLog("Was disconnect but mTcpSocket was nullptr\n", QColor(240, 14, 14));
		return;
	}

	mTcpSocket->close(); // ñîçäëà¸ò ñèãíàë void QIODevice::aboutToClose() à çàòåì óñòàíàâëèâàåò äëÿ OpenMode ñîñòîÿíèå NotOpen.
	emit messegeLog("\n" + QString::number(port) + " - " + "socket close", QColor(255, 128, 0));
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

	if (!treeFazeBool)
	{
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
	}
	else
	{
		if (TwoZero)
		{
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
		}
		else
		{
			if (any.length() == 2 || any.length() == 1)
			{
				if (any.length() == 2)
					any.push_front("0.0");
				else
					any.push_front("0.00");

			}
			else
			{
				any.insert((any.length() - 3), '.');
			}
		}
	}

	return any;
}


QString MyTcpServer::crc16Modbus(const QByteArray& data) // CRC16MODBUS äëÿ îïðåäåëåíèÿ êîíòðîëüíîé ñóììû â êîíöå ïàêåòîâ äëÿ Ìèëóð107
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


void MyTcpServer::newDayBuffer()
{
	if (todayDate != QDate::currentDate().toString("dd-MM-yyyy"))
	{
		todayDate = QDate::currentDate().toString("dd-MM-yyyy");
		serialBuff = fullSerialBuffConstant;
		emit messegeLog("Polling queue's restored\n", QColor(240, 218, 15));
	}
}


void MyTcpServer::readDeviceFile()
{
	QFile file(QCoreApplication::applicationDirPath() + "\\" + QString::number(port) + ".txt");

	if (!file.open(QIODevice::ReadOnly))
	{
		emit messegeLog("Don't find file for port " + QString::number(port) + " with devices.\n", QColor(240, 14, 14));

		return;
	}

	QTextStream out(&file);

	QString* myLine = new (QString);
	int counter = 0;
	int counterErr = 1;
	bool digitErr = false;
	bool threeFazeInFunc = false;
	bool twoZeroInFunc = false;

	while (out.readLineInto(myLine, 0))
	{
		if (*myLine == "@")
		{
			threeFazeInFunc = true;
			continue;
		}

		if (*myLine == "$")
		{
			threeFazeInFunc = false;
			twoZeroInFunc = true;
			continue;
		}

		if (myLine->length() > 5 || myLine->length() < 4)
		{
			emit messegeLog("The device on line " + QString::number(counter + counterErr) + " is incorrect\n", QColor(240, 14, 14));
			counterErr++;
			continue;
		}

		for (auto& val : *myLine)
		{

			if (!val.isDigit())
			{
				emit messegeLog("The device on line " + QString::number(counter + counterErr) + " is incorrect\n", QColor(240, 14, 14));
				counterErr++;
				digitErr = true;
				continue;
			}

		}

		if (digitErr)
		{
			digitErr = false;
			continue;
		}

		if (!threeFazeInFunc && !twoZeroInFunc)
			serialBuff.push_back(myLine->toUtf8());

		if (threeFazeInFunc)
			threeFazeBuff.push_back(myLine->toUtf8());

		if (twoZeroInFunc)
			threeFazeBuffTwoZero.push_back(myLine->toUtf8());

		++counter;
	}

	file.close();
}


QString MyTcpServer::getQueueInfo()
{
	return QString(QString::number(port) + " - queue polling = " + QString::number(serialBuff.length()) + "\n");
}


void MyTcpServer::checkTodayValues()
{
	QDate curDate = QDate::currentDate();

	QString timeInQuery = curDate.toString("yyyy-MM-dd"); // Разворачиваем формат даты так как в БД.

	for (int counter = 0; counter < serialBuff.length(); counter++)
	{
		QString temp = dataWrite->readData(serialBuff[counter]);

		if (temp != "")
		{
			if (dataWrite->readData(serialBuff[counter]) == timeInQuery)
			{
				serialBuff.remove(counter); // удаляем из очереди опроса то что опрошено
				if (serialBuff.length() != 0)
				{
					counter--;
				}
			}
		}
	}
}


void MyTcpServer::queueRefresh()
{
	serialBuff = fullSerialBuffConstant;
}


QString MyTcpServer::getPort()
{
	return QString::number(port);
}


QList<QByteArray> MyTcpServer::getfullSerialBuffConstant()
{
	return fullSerialBuffConstant;
}

void MyTcpServer::addDeviceInArray(QByteArray any)
{
	serialBuff.push_back(any);
}