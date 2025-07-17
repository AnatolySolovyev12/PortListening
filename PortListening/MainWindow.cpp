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

	QString appStyle = R"(
    QMainWindow {
        background-color: rgb(120,120,120);
    }
    QMenu {
        background-color: rgb(120,120,120);
        color: white;      /* Текстовые элементы */
        border-radius: 6px;
    }
    QMenu::item:selected {
        background-color: #2a9d8f; /* Цвет подсветки пункта */
        color: white;
    }
)";

	qApp->setStyleSheet(appStyle);

	textEdit = new QTextEdit(this);
	textEdit->setReadOnly(true);
	textEdit->setStyleSheet(
		"QTextEdit {"
		"    background-color: rgb(50, 50, 50);"
		"}"
	);




	warningButton = new QPushButton("Warning (" + QString::number(warningCounter) + ')', this);

	QMenu* warningMenu = new QMenu(warningButton); // Инициализируем выпадающую кнопку

	warningMenu->addAction("&Show", this, &MainWindow::warningPrint);
	warningMenu->addAction("&Clear", this, &MainWindow::warningArrayClear);

	warningButton->setMaximumWidth(80);
	warningButton->setStyleSheet(
		"QPushButton {"
		"    background-color: rgb(50, 50, 50);"
		"    color: white;"
		"    border-radius: 5px;"
		"}"
	);

	warningButton->setMenu(warningMenu);





	QPushButton* paramMenu = new QPushButton("QueueList", this);

	QMenu* pm = new QMenu(paramMenu); // Инициализируем выпадающую кнопку

	pm->addAction("&Show", this, &MainWindow::queuePrint);
	pm->addAction("&Reset", this, &MainWindow::queueRefreshInPorts);
	pm->addAction("&Actualization", this, &MainWindow::actualizationQueue);

	paramMenu->setMaximumWidth(80);
	paramMenu->setStyleSheet(
		"QPushButton {"
		"    background-color: #2a9d8f;"
		"    color: white;"
		"    border-radius: 5px;"
		"}"
		"QPushButton:pressed {"
		"    background-color: rgb(50, 50, 50);" // в HEX #3cbaa2. Допустимо background-color: #3cbaa2;

		"}"
	);

	QPushButton* portsMenu = new QPushButton("Ports", this);
	QMenu* ports = new QMenu(portsMenu);
	ports->setTitle("Ports");

	QTimer::singleShot(1500, [this, ports, paramMenu, pm]() {

		pm->addMenu(ports);

		for (int val = 0; val < serverList.length(); val++)
		{
			QMenu* newPort = new QMenu(serverList[val]->getPort());

			ports->addMenu(newPort);

			for (auto& val : serverList[val]->getfullSerialBuffConstant())
			{
				newPort->addAction(val, this, [newPort, val, this]() {

					addDeviceFromMenu(newPort->title(), val);

					});
			}
		}

		paramMenu->setMenu(pm);

		}
	);

	QPushButton* clear = new QPushButton("Clear", this);
	clear->setMaximumWidth(80);
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

	QCheckBox* checkClear = new QCheckBox("  AutoClear", this);
	checkClear->setMaximumWidth(100);
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

	Hlayout->addWidget(warningButton);
	Hlayout->addWidget(paramMenu);
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
{
}

void MainWindow::clearWindow()
{
	textEdit->clear();
}

void MainWindow::readPropertiesFile()
{
	QFile file(QCoreApplication::applicationDirPath() + "\\ports.txt");

	if (!file.open(QIODevice::ReadOnly))
	{
		textEdit->append("\nDon't find browse file. Used default parameters");

		port = 49000;

		serverList.push_back(new MyTcpServer(port));
		connect(serverList[0], SIGNAL(messegeLog(QString, QColor)), this, SLOT(outputMessage(QString, QColor)));
		connect(serverList[0], SIGNAL(warningLog(QString)), this, SLOT(getWarningMessege(QString)));

		dbList.push_back(serverList[0]->returnPtrDb());
		connect(dbList[0], SIGNAL(messegeLog(QString, QColor)), this, SLOT(outputMessage(QString, QColor)));

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
			textEdit->append("\nThe port on line " + QString::number(counter + counterErr) + " is incorrect");
			counterErr++;
			continue;
		}

		for (auto& val : *myLine)
		{
			if (!val.isDigit())
			{
				textEdit->append("\nThe port on line " + QString::number(counter + counterErr) + " is incorrect");
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
		connect(serverList[counter], SIGNAL(warningLog(QString)), this, SLOT(getWarningMessege(QString)));
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


void MainWindow::queuePrint()
{
	setTextColour(QColor(255, 128, 0));

	for (int val = 0; val < serverList.length(); val++)
	{
		textEdit->append(serverList[val]->getQueueInfo());
	}
}


void MainWindow::queueRefreshInPorts()
{
	for (int val = 0; val < serverList.length(); val++)
	{
		serverList[val]->queueRefresh();
	}

	setTextColour(QColor(240, 218, 15));

	textEdit->append("\nPolling queue's restored");
}


void MainWindow::actualizationQueue()
{
	for (int val = 0; val < serverList.length(); val++)
	{
		serverList[val]->checkTodayValues();
	}

	setTextColour(QColor(240, 218, 15));

	textEdit->append("\nPolling queue's actualization");
}


void MainWindow::addDeviceFromMenu(QString any, QByteArray some)
{
	for (int count = 0; count < serverList.length(); count++)
	{
		if (serverList[count]->getPort() == any)
		{
			serverList[count]->addDeviceInArray(some);

			setTextColour(QColor(240, 218, 15));
			textEdit->append("\nIn polling was added " + some);

			break;
		}
	}
};


void MainWindow::warningPrint()
{
	setTextColour(QColor(240, 218, 15));

	if (warningList.isEmpty())
	{
		textEdit->append("\nWarning array is empty.");
		return;
	}

	textEdit->append("\n");

	for (int val = 0; val < warningList.length(); val++)
	{
		textEdit->append(warningList[val]);
	}
}


void MainWindow::warningArrayClear()
{
	warningList.clear();

	warningCounter = 0;

	setTextColour(QColor(240, 218, 15));

	textEdit->append("\nWarning array was clear.");

	warningButton->setStyleSheet(warningButtonStyleGrey);

	warningButton->setText("Warning (" + QString::number(warningCounter) + ')');
}