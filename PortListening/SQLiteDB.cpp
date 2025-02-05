#include "SQLiteDB.h"
#include <QtDebug>


SQLiteDB::SQLiteDB(QObject* parent)
	: QObject(parent)
{
	QTimer::singleShot(500, [this]() {

		connectDB(); 

		QSqlQuery query; 

		db_input = "CREATE TABLE channelTable ( "
			"number VARCHAR(20), " 
			"date VARCHAR(20), "  
			"channelFirst VARCHAR(20), " 
			"channelSecond VARCHAR(20), "
			"channelThird VARCHAR(20), " 
			"channelFour VARCHAR(20));";

		if (!query.exec(db_input)) // Выполняем запрос. exec - вернёт true если успешно. Синтаксис должен отвечать запрашиваемой БД.
		{
			emit messegeLog("Unable to create a channelTable. " + query.lastError().text());
			//	qDebug() << "Unable to create a table" << query.lastError(); // Возвращаем информацию о последней ошибке. При вывзове exec, получая ошибку, она помещается в lastError(). Мы можем её прочитать..
		}
		else
		{
			emit messegeLog("channelTable was create!");
			//qDebug() << "Table was create!";
		}

		db_input = "CREATE TABLE counterTable ( "
			"number VARCHAR(20), "
			"date VARCHAR(20), "
			"channelFirst VARCHAR(20), "
			"channelSecond VARCHAR(20), "
			"channelThird VARCHAR(20), "
			"channelFour VARCHAR(20));";

		if (!query.exec(db_input)) // Выполняем запрос. exec - вернёт true если успешно. Синтаксис должен отвечать запрашиваемой БД.
		{
			emit messegeLog("Unable to create a counterTable. " + query.lastError().text());
			//	qDebug() << "Unable to create a table" << query.lastError(); // Возвращаем информацию о последней ошибке. При вывзове exec, получая ошибку, она помещается в lastError(). Мы можем её прочитать..
		}
		else
		{
			emit messegeLog("counterTable was create!");
			//qDebug() << "Table was create!";
		}

		});
}


SQLiteDB::~SQLiteDB()
{
	mw_db.removeDatabase("DataBaseMilanRF");

	emit messegeLog("Object DB was destroyed");
	//qDebug() << "Object DB was destroyed";

	exit(0);
}


bool SQLiteDB::connectDB()
{
	mw_db = QSqlDatabase::addDatabase("QSQLITE"); 
	mw_db.setDatabaseName("DataBaseMilanRF"); // Указываем с какой БД взаимодействовать. Если такого имени не найдёт то файл БД с указанным именем будет создан.

	if (!mw_db.open()) 
	{
		emit messegeLog("Cannot open database: " + mw_db.lastError().text());
		//qDebug() << "Cannot open database: " << mw_db.lastError();
		return false;
	}

	return true;
}


void SQLiteDB::writeData(QString some)
{
	QSqlQuery query;

	db_input = some;

	if (!query.exec(db_input))
	{
		emit messegeLog("Unable to insert data" + query.lastError().text() + query.lastQuery());
		//qDebug() << "Unable to insert data" << query.lastError() << " : " << query.lastQuery();
	}
}












