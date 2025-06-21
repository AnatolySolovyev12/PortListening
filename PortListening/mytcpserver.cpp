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

	if (port == 6000)
	{
		serialBuff = { "75024", "75001", "74986", "74981", "74995", "74998", "75008", "74980", "75000", "74992" };
	}
	if (port == 49500)
	{
		serialBuff = { "74985", "75020", "74987", "74991", "74988", "74982", "74989", "74990"};
	}
	if (port == 49501)
	{
		serialBuff = { "74993", "74984", "74996", "75002", "74983", "75014", "74997", "74994"};
	}


	QTimer::singleShot(500, [this]() {

		if (!mTcpServer->listen(QHostAddress::Any, port)) // ñëóøàåì ñ ëþáîãî àäðåñà íà ïîðò 6000. Ìîæíî óêàçàòü îïðåäåë¸ííûé host äëÿ ïðîñëóøèâàíèÿ
		{
			emit messegeLog("server with port " + QString::number(port) + " is not started\n");
		}
		else
		{
			emit messegeLog("server with port " + QString::number(port) + " is started\n");
		}


		/*
		QString testString;
		QString myList{ "f824010001060415954200a74e" };

		QString answerListMilur = myList;

		answerListMilur += zeroBuff;

		for (int tempVal = 14; tempVal <= 21; tempVal++)
		{
			answerListMilur.push_back(answerListMilur[tempVal]);
		}

		for (int tempVal = 14; tempVal <= 21; tempVal++)
		{
			answerListMilur.push_back(myList[tempVal]);
		}

		answerListMilur += strZero;
		testString = answerListMilur;


		emit messegeLog(testString);
		emit messegeLog(QString::number(testString.length()));
		QString ttt = testString[154];
		ttt += testString[155];
		ttt += testString[156];
		ttt += testString[157];
		emit messegeLog(ttt);

		*/







	
	/*
	
	QTimer::singleShot(300, [this]() {

		QByteArray testNumber = "74995";

		QByteArray data1 = QByteArray::fromHex("0800FFFFFFFFFFFF");

		data1.push_front(QByteArray::fromHex(serialArrayRotate(testNumber)));

		QString crc1 = crc16Modbus(data1);

		emit messegeLog(data1.toHex().toUpper() + crc1);



		});

		*/






	


	






		

		});
}

void MyTcpServer::slotNewConnection()
{
	mTcpSocket = mTcpServer->nextPendingConnection(); // âîçâðàùàåò îáúåêò QTcpSocket äëÿ òåêóùåãî ñîåäèíåíèÿ. Âåðí¸ò nullptr åñëè âûçâàòü ýòó ôóíêöèþ áåç íàëè÷èÿ ñîåäèíåíèÿ. Ëó÷øå ïîòîì óäàëÿòü QTcpSocket óêàçàòåëü è ïî èòîãó çàíóëÿòü.

	if (!mTcpSocket) // ïðîâåðêà íà íåêîððåêòíîå èñïîëüçîâàíèå
	{
		emit messegeLog("No pending connection\n");
		return; // Âûõîä èç ôóíêöèè, åñëè íåò ñîåäèíåíèÿ
	}

	connect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead); // åñëè åñòü ÷òî ÷èòàòü (áèáëèîòå÷íûé ñèãíàë) ñðàáîòàåò ñëîò
	connect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected); // åñëè ñîêåò îòñîåäèíèëñÿ (áèáëèîòå÷íûé ñèãíàë) ñðàáîòàåò ñëîò

	QDate curDate = QDate::currentDate();
	QTime curTime = QTime::currentTime();

	QString temp = "\nConnect from host " + mTcpSocket->peerAddress().toString().sliced(7) + " - " + curDate.toString("dd-MM-yyyy") + " " + curTime.toString(); // äëÿ àíàëèçà âõîäÿùèõ ïîäêëþ÷åíèé
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

		//  mTcpSocket->write(array); // Ýõî ýôôåêò ñ îòïðàâêîé ïðèíÿòîãî îáðàòíî ñîêåòó

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
				answerListMilur += str;
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

				emit messegeLog(data1.toHex());

				mTcpSocket->write(data1);

				oldMessege = false;

				continue;
			}

			emit messegeLog(QString::number(str.length()) + " - " + str);////////////////////////
		}

		if(str.size() == 206) str.replace(QRegularExpression(pattern), "");

		if(str.size() == 404) str.remove(202, 404);

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

		if(str.size() == 286) str = str.sliced(84);

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

			emit messegeLog("first - " + converFuncString(first));
			//qDebug() << "first - " << valTrans;

			emit messegeLog("two - " + converFuncString(two));
			//qDebug() << "two - " << valTrans;

			emit messegeLog("three - " + converFuncString(three));
			//qDebug() << "three - " << valTrans;

			emit messegeLog("four - " + converFuncString(four));
			//qDebug() << "four - " << valTrans << "\n";


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
		emit messegeLog("Was disconnect but mTcpSocket was nullptr\n");
		return;
	}

	mTcpSocket->close(); // ñîçäëà¸ò ñèãíàë void QIODevice::aboutToClose() à çàòåì óñòàíàâëèâàåò äëÿ OpenMode ñîñòîÿíèå NotOpen.
	emit messegeLog("\n" + QString::number(port) + " - " + "socket close");
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

