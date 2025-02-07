#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMainWindow>
#include <QWidget>

#include "mytcpserver.h"

#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);

	~MainWindow();

	//void outputMessage();

	void clearWindow();
	void readPropertiesFile();

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

	void outputMessage(const QString some) 
	{
		textEdit->append(some);
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
};