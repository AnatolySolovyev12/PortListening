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

		if (!query.exec(db_input)) // ��������� ������. exec - ����� true ���� �������. ��������� ������ �������� ������������� ��.
		{
			if (query.lastError().text() != "table channelTable already exists Unable to execute statemen")
			{
			}
			else
				emit messegeLog("Unable to create a channelTable. " + query.lastError().text() + '\n', QColor(240, 14, 14));
		}
		else
		{
			emit messegeLog("channelTable was create!\n", QColor(255, 128, 0));
		}
}

SQLiteDB::~SQLiteDB()
{
	mw_db.removeDatabase("DataBaseMilanRF");

	emit messegeLog("\nObject DB was destroyed", QColor(240, 14, 14));

	exit(0);
}

bool SQLiteDB::connectDB()
{
	mw_db = QSqlDatabase::addDatabase("QSQLITE");
	mw_db.setDatabaseName("DataBaseMilanRF"); // ��������� � ����� �� �����������������. ���� ������ ����� �� ����� �� ���� �� � ��������� ������ ����� ������.

	if (!mw_db.open())
	{
		emit messegeLog("Cannot open database: " + mw_db.lastError().text() + '\n', QColor(240, 14, 14));

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
		emit messegeLog("Unable to insert data" + query.lastError().text() + query.lastQuery() + '\n', QColor(240, 14, 14));
	}
}


QString SQLiteDB::readData(QString any)
{
	QSqlQuery query;
	
	QString queryString = "select date from counterTable where number = " + any + " order by date desc";

	/*
	query.prepare("select date from counterTable where number = :MeterInfoPrep order by date desc"); // ���������� �������������� ������ � ������ ��� ������� �������� �� ��������
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
	query.prepare("select date from counterTable where number = :MeterInfoPrep order by date desc"); // ���������� �������������� ������ � ������ ��� ������� �������� �� ��������
	query.bindValue(":MeterInfoPrep", any);
	*/

	if (!query.exec(queryString) || !query.next())
	{
		qDebug() << "Query failed or no results in current DB: " << query.lastError();

		return "0 0";
	}
	else
	{
		return query.value(1).toString() + " " + query.value(2).toString();
	}
}