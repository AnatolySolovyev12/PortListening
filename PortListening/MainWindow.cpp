#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), clearTimer(new QTimer)
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
	textEdit->setStyleSheet(
		"QTextEdit {"
		"    background-color: rgb(50, 50, 50);" 
		"}"
	);

	QPushButton* clear = new QPushButton("Clear", this);
	clear->setMaximumWidth(50);
	clear->setStyleSheet(
		"QPushButton {"
		"    background-color: #2a9d8f;"
		"    color: white;"
		"    border-radius: 5px;"
		"}"
		"QPushButton:pressed {"
		"    background-color: rgb(50, 50, 50);" // в HEX #3cbaa2. Допустимо background-color: #3cbaa2;
	
		"}"
	);


	QCheckBox* checkClear = new QCheckBox("AutoClear", this);
	checkClear->setMaximumWidth(80);
	checkClear->setStyleSheet(
		"QCheckBox{"
		"    background-color: rgb(50, 50, 50);"
		"    color: white;"
		"    border-radius: 5px;"
		"}"
		"QCheckBox:checked {"
		"    background-color: #2a9d8f;" // в HEX #3cbaa2. Допустимо background-color: #3cbaa2;

		"}"
	);

	QHBoxLayout* Hlayout = new QHBoxLayout;

	Hlayout->addWidget(clear);
	Hlayout->addWidget(checkClear);

	QVBoxLayout* layout = new QVBoxLayout;
	
	layout->addWidget(textEdit);
	layout->addLayout(Hlayout);

	QWidget* centralWidget = new QWidget(this);
	centralWidget->setLayout(layout);
	setCentralWidget(centralWidget);

	connect(clear, &QPushButton::clicked, this, &MainWindow::clearWindow);

	QTimer::singleShot(500, this, &MainWindow::readPropertiesFile);

	clearTimer->start(600000);
	connect(clearTimer, &QTimer::timeout, this, &MainWindow::checkClear);

	todayDate = QDate::currentDate().toString("dd-MM-yyyy");
	
}

MainWindow::~MainWindow()
{}

void MainWindow::clearWindow()
{
	textEdit->clear();
} 

void MainWindow::readPropertiesFile()
{
	QFile file(QCoreApplication::applicationDirPath() + "\\ports.txt");

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
		connect(serverList[counter], SIGNAL(messegeLog(QString, QColor)), this, SLOT(outputMessage(QString, QColor)));
		dbList.push_back(serverList[counter]->returnPtrDb());
		connect(dbList[counter], SIGNAL(messegeLog(QString, QColor)), this, SLOT(outputMessage(QString, QColor)));

		++counter;
	}

	if (myLine != nullptr)
	{
		delete myLine;
		myLine = nullptr;
	}
	
	file.close();
}


void MainWindow::checkClear()
{
	if (todayDate != QDate::currentDate().toString("dd-MM-yyyy"))
	{
		todayDate = QDate::currentDate().toString("dd-MM-yyyy");

		clearWindow();
	}
}

void MainWindow::setTextColour(QColor any)
{
	textEdit->setTextColor(any);
}