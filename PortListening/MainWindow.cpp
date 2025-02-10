#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(QIcon("icon.png"));

	QMenu* menu = new QMenu(this);
	QAction* restoreAction = menu->addAction("Open window");
	QAction* restoreActionHide = menu->addAction("Hide window");
	QAction* quitAction = menu->addAction("Exit");

	connect(restoreAction, &QAction::triggered, this, &MainWindow::show);
	connect(restoreActionHide, &QAction::triggered, this, &MainWindow::hide);
	connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

	trayIcon->setContextMenu(menu);
	trayIcon->setVisible(true);

	connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);

	textEdit = new QTextEdit(this);
	textEdit->setReadOnly(true);

	QPushButton* clear = new QPushButton("Clear", this);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(textEdit);
	layout->addWidget(clear);

	QWidget* centralWidget = new QWidget(this);
	centralWidget->setLayout(layout);
	setCentralWidget(centralWidget);

	connect(clear, &QPushButton::clicked, this, &MainWindow::clearWindow);

	readPropertiesFile();
}

MainWindow::~MainWindow()
{}

void MainWindow::clearWindow()
{
	textEdit->clear();
} 

void MainWindow::readPropertiesFile()
{
	QFile file("ports.txt");

	if (!file.open(QIODevice::ReadOnly))
	{
		textEdit->append("Don't find browse file. Used default parameters\n");

		port = 49000;

		serverList.push_back(new MyTcpServer(port));
		connect(serverList[0], SIGNAL(messegeLog(QString)), this, SLOT(outputMessage(QString)));

		dbList.push_back(serverList[0]->returnPtrDb());
		connect(dbList[0], SIGNAL(messegeLog(QString)), this, SLOT(outputMessage(QString)));

		return;
	}

	QTextStream out(&file);

	bool ok;
	QString temporary;
	QString* myLine = new QString();
	int counter = 0;
	int counterErr = 1;
	bool digitErr = false;

	while (out.readLineInto(myLine, 0))
	{
		if (myLine->length() > 5 || myLine->length() < 4)
		{
			textEdit->append("The port on line " + QString::number(counter + counterErr) + " is incorrect\n");
			counterErr++;
			continue;
		}

		for (auto& val : *myLine)
		{
			if (!val.isDigit())
			{
				textEdit->append("The port on line " + QString::number(counter + counterErr) + " is incorrect\n");
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

		port = myLine->toInt(&ok, 10);

		serverList.push_back(new MyTcpServer(port));
		connect(serverList[counter], SIGNAL(messegeLog(QString)), this, SLOT(outputMessage(QString)));
		dbList.push_back(serverList[counter]->returnPtrDb());
		connect(dbList[counter], SIGNAL(messegeLog(QString)), this, SLOT(outputMessage(QString)));

		++counter;
	}

	if (myLine != nullptr)
	{
		delete myLine;
		myLine = nullptr;
	}
	
	file.close();
}