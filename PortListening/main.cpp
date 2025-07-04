
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

	mainWindow.setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "\\icon.png"));
	mainWindow.setWindowTitle("PortListening by Solovev");
	mainWindow.resize(700, 300);

	mainWindow.setStyleSheet(
		"    background-color: rgb(120, 120, 120);"
	);

	mainWindow.show();

	return app.exec();
}