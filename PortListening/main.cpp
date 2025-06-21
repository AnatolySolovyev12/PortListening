
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

	mainWindow.setWindowIcon(QIcon("icon.png"));
	mainWindow.setWindowTitle("PortListening by Solovev");
	mainWindow.resize(700, 300);
	mainWindow.show();




/*
	QString test = "f9240100010504653606307a77000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000006536063040923900000000000000000000000000000000000000000000000000000000000000";

	QString full = "f9240100010504653606307a77";
	QString zero1 = "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

	QString zero2 = "000000000000000000000000000000000000000000000000000000000000";

	qDebug() << test.length();
	qDebug() << full.length();
	qDebug() << zero1.length();
	qDebug() << zero2.length();
	qDebug() << full.length() + zero1.length() + zero2.length();
	*/


	return app.exec();
}