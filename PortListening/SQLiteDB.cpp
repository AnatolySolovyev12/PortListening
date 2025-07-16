#include "SQLiteDB.h"
#include <QtDebug>

SQLiteDB::SQLiteDB(QObject* parent)
	: QObject(parent)
{
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
			if (query.lastError().text() != "table channelTable already exists Unable to execute statemen")
			{
			}
			else
				emit messegeLog("Unable to create a channelTable. " + query.lastError().text() + '\n');
		}
		else
		{
			emit messegeLog("channelTable was create!\n");
		}
}

SQLiteDB::~SQLiteDB()
{
	mw_db.removeDatabase("DataBaseMilanRF");

	emit messegeLog("\nObject DB was destroyed");

	exit(0);
}

bool SQLiteDB::connectDB()
{
	mw_db = QSqlDatabase::addDatabase("QSQLITE");
	mw_db.setDatabaseName("DataBaseMilanRF"); // Указываем с какой БД взаимодействовать. Если такого имени не найдёт то файл БД с указанным именем будет создан.

	if (!mw_db.open())
	{
		emit messegeLog("Cannot open database: " + mw_db.lastError().text() + '\n');

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
		emit messegeLog("Unable to insert data" + query.lastError().text() + query.lastQuery() + '\n');
	}
}


QString SQLiteDB::readData(QString any)
{
	QSqlQuery query;
	
	QString queryString = "select date from counterTable where number = " + any + " order by date desc";

	/*
	query.prepare("select date from counterTable where number = :MeterInfoPrep order by date desc"); // используем подготовленный запрос в начале как хорошую практику от инъекций
	query.bindValue(":MeterInfoPrep", any);
	*/

	if (!query.exec(queryString) || !query.next())
	{
		qDebug() << "Query failed or no results in current DB: " << query.lastError();

		return "1984-01-01";
	}
	else
		return query.value(0).toString();
}


QString SQLiteDB::readValues(QString any)
{
	QSqlQuery query;

	QString queryString = "select date, channelFirst, channelSecond from counterTable where number = " + any + " order by date desc";

	/*
	query.prepare("select date from counterTable where number = :MeterInfoPrep order by date desc"); // используем подготовленный запрос в начале как хорошую практику от инъекций
	query.bindValue(":MeterInfoPrep", any);
	*/

	if (!query.exec(queryString) || !query.next())
	{
		qDebug() << "Query failed or no results in current DB: " << query.lastError();

		return "0 0";
	}
	else
	{
		qDebug() << query.value(1).toString() + " " + query.value(2).toString();
		return query.value(1).toString() + " " + query.value(2).toString();
	}
}