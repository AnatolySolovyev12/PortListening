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

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);

	~MainWindow();

	void clearWindow();
	void readPropertiesFile();
	void checkClear();
	void setTextColour(QColor any);
	void queuePrint();
	void queueRefreshInPorts();
	void actualizationQueue();
	void addDeviceFromMenu(QString any, QByteArray some);
	void warningPrint();
	void warningArrayClear();

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

	void getWarningMessege(const QString some)
	{
		warningButton->setStyleSheet(warningButtonStyleYellow);
		warningCounter++;
		warningList.push_back(some);
		warningButton->setText("Warning (" + QString::number(warningCounter) + ')');
	}

private:
	MyTcpServer* TcpServer = nullptr;
	MyTcpServer* TcpServerNext = nullptr;
	SQLiteDB* dbForEmit = nullptr;
	SQLiteDB* dbForEmitNext = nullptr;

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
		    color: white;
		  border-radius: 5px;
		}
    )";

	QString warningButtonStyleGrey = R"(
		QPushButton {
		   background-color: rgb(50, 50, 50);
		    color: blue;
		  border-radius: 5px;
		}
    )";
};