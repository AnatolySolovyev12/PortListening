#pragma once

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMainWindow>
#include <QWidget>
#include <QObject>
#include <QTimer.h>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox.h>
#include "mytcpserver.h"
#include "QInputDialog.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);

	~MainWindow();

	void clearWindow();
	void readPropertiesFile();
	void checkDateAndClear();
	void setTextColour(QColor any);
	void queuePrint();
	void queueRefreshInPorts();
	void actualizationQueue();
	void addDeviceFromMenu(QString any, QByteArray some);
	void warningPrint();
	void warningArrayClear();
	void setStateValidationBool();
	void setWarningDot();
	void setAlarmDot();

private slots:
	void iconActivated(QSystemTrayIcon::ActivationReason reason)
	{
		if (reason == QSystemTrayIcon::DoubleClick)
		{
			if (!windowShow)
			{
				this->show();
				windowShow = true;
				//trayIcon->setVisible(false); // 
			}
			else
			{
				this->hide();
				windowShow = false;
				//trayIcon->setVisible(true); // 
			}
		}
		/*
		if (reason == QSystemTrayIcon::Context) //
		{
			trayIcon->showMessage("CONTEX MENU", "WHATS THAT?", QSystemTrayIcon::Information, 5000); //
		}
		*/
	}

	void outputMessage(const QString some, QColor any)
	{
		setTextColour(any);
		textEdit->append(some);
	}

	void getWarningMessege(const QString some, bool writeInfile)
	{
		QString mainTemp = some;
		QString temp = some;

		warningButton->setStyleSheet(warningButtonStyleYellow);
		warningCounter++;

		QRegularExpression dateTimePattern(QString(R"([0-9]{2,5} - [0-9]{2}-[0-9]{2}-[0-9]{4} [0-9]{2}:[0-9]{2}:[0-9]{2} - )"));
		QRegularExpression dayNightPattern(QString(R"(\s*\(.*?\))"));

		QRegularExpressionMatch matchReg = dateTimePattern.match(temp);

		if (matchReg.hasMatch())
			temp.remove(matchReg.captured());

		matchReg = dayNightPattern.match(temp);

		if (matchReg.hasMatch())
			temp.remove(matchReg.captured());

		for (auto& val : warningList)
			if (val.contains(temp))
				mainTemp += " !";

		warningList.push_back(mainTemp);
		warningButton->setText("Warning (" + QString::number(warningCounter) + ')');

		if (writeInfile)
		{
			QString filename = QCoreApplication::applicationDirPath() + "\\warnings.txt";
			QFile file(filename);

			// Открываем файл в режиме "Только для записи и дополнения без перезаписи"
			if (file.open(QIODevice::WriteOnly | QIODevice::Append))
			{
				QTextStream out(&file); // поток записываемых данных направляем в файл
				out << mainTemp << Qt::endl;
			}
			else
				qWarning("Could not open file");

			file.close();
		}
	}

private:
	MyTcpServer* TcpServer = nullptr;
	MyTcpServer* TcpServerNext = nullptr;

	QList <MyTcpServer*> serverList;
	QList <SQLiteDB*> dbList;
	int port = 43000;
	int counterPorts = 1;

	QSystemTrayIcon* trayIcon = nullptr;

	bool windowShow = false;

	QTextEdit* textEdit;

	QTimer* clearTimer = nullptr;

	QString todayDate;

	int warningCounter = 0;
	QList<QString>warningList;
	QPushButton* warningButton = nullptr;
	
	QString warningButtonStyleYellow = R"(
		QPushButton {
		   background-color: rgb(240, 218, 15);
		    color: blue;
		  border-radius: 5px;
		}
    )";

	QString warningButtonStyleGrey = R"(
		QPushButton {
		   background-color: rgb(50, 50, 50);
		    color: white;
		  border-radius: 5px;
		}
    )";

	QCheckBox* checkClear = nullptr;
	QAction* validationCheck = nullptr;

	int warningDot = 40;
	int alarmDot = 100;
};