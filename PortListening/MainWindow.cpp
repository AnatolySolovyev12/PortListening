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

	for (int val = 0; val < counterPorts; val++)
	{
		serverList.push_back(new MyTcpServer(port + val));
		connect(serverList[val], SIGNAL(messegeLog(QString)), this, SLOT(outputMessage(QString)));

		dbList.push_back(serverList[val]->returnPtrDb());
		connect(dbList[val], SIGNAL(messegeLog(QString)), this, SLOT(outputMessage(QString)));
	}
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
		return;
	}

	QTextStream out(&file);

	bool ok;
	QString temporary;
	QString* myLine = new QString();
	int counter = 0;

	while (out.readLineInto(myLine, 0))
	{
		if (counter == 0)
		{
			counterPorts = myLine->toInt(&ok, 10);
		}
	
		else
		{
			port = myLine->toInt(&ok, 10);
			break;
		}
		counter++;
	}
	file.close();
}