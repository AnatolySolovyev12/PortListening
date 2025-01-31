
#include "mytcpserver.h"
#include <QTextStream>
#include <qfile.h>


#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

QTextStream myout(stdout);

int main(int argc, char* argv[])
{


	QApplication app(argc, argv);

	MyTcpServer server;


	QSystemTrayIcon trayIcon; 
	trayIcon.setIcon(QIcon("icon.png")); 
	trayIcon.setVisible(true);

	// Создаем контекстное меню для иконки
	QMenu menu;
	QAction* quitAction = menu.addAction("Exit");
	QObject::connect(quitAction, &QAction::triggered, &server, &QApplication::quit);

	trayIcon.setContextMenu(&menu);

	// Скрываем главное окно
	// Если у вас есть главное окно, вы можете его скрыть
	// mainWindow.hide();



	return app.exec();
}