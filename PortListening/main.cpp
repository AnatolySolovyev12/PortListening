
#include "MainWindow.h"
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

	MainWindow mainWindow;
	mainWindow.resize(400, 300);
	mainWindow.show();


	return app.exec();
}