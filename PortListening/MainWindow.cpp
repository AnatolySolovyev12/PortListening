#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    TcpServer = new MyTcpServer();
    dbForEmit = TcpServer->returnPtrDb();

    connect(TcpServer, SIGNAL(messegeLog(QString)), this, SLOT(outputMessage(QString)));
    connect(dbForEmit, SIGNAL(messegeLog(QString)), this, SLOT(outputMessage(QString)));

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

   // QPushButton* button = new QPushButton("Messege send", this);

    QPushButton* clear = new QPushButton("Clear", this);


    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(textEdit);

    layout->addWidget(clear);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(clear, &QPushButton::clicked, this, &MainWindow::clearWindow);

}


MainWindow::~MainWindow()
{}

void MainWindow::clearWindow()
{
    textEdit->clear();
}