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

		if (!query.exec(db_input)) // ��������� ������. exec - ������ true ���� �������. ��������� ������ �������� ������������� ��.
		{
			if (query.lastError().text() != "table channelTable already exists Unable to execute statemen")
			{
			}
			else
				emit messegeLog("Unable to create a channelTable. " + query.lastError().text() + '\n');
			//	qDebug() << "Unable to create a table" << query.lastError(); // ���������� ���������� � ��������� ������. ��� ������� exec, ������� ������, ��� ���������� � lastError(). �� ����� � ���������..
		}
		else
		{
			emit messegeLog("channelTable was create!\n");
			//qDebug() << "Table was create!";
		}

		db_input = "CREATE TABLE counterTable ( "
			"number VARCHAR(20), "
			"date VARCHAR(20), "
			"channelFirst VARCHAR(20), "
			"channelSecond VARCHAR(20), "
			"channelThird VARCHAR(20), "
			"channelFour VARCHAR(20));";

		if (!query.exec(db_input)) // ��������� ������. exec - ������ true ���� �������. ��������� ������ �������� ������������� ��.
		{
			if (query.lastError().text() != "table counterTable already exists Unable to execute statemen")
			{
			}
			else
				emit messegeLog("Unable to create a counterTable. " + query.lastError().text() + '\n');
			//	qDebug() << "Unable to create a table" << query.lastError(); // ���������� ���������� � ��������� ������. ��� ������� exec, ������� ������, ��� ���������� � lastError(). �� ����� � ���������..
		}
		else
		{
			emit messegeLog("counterTable was create!\n");
			//qDebug() << "Table was create!";
		}

		});
}

SQLiteDB::~SQLiteDB()
{
	mw_db.removeDatabase("DataBaseMilanRF");

	emit messegeLog("\nObject DB was destroyed");
	//qDebug() << "Object DB was destroyed";

	exit(0);
}

bool SQLiteDB::connectDB()
{
	mw_db = QSqlDatabase::addDatabase("QSQLITE");
	mw_db.setDatabaseName("DataBaseMilanRF"); // ��������� � ����� �� �����������������. ���� ������ ����� �� ����� �� ���� �� � ��������� ������ ����� ������.

	if (!mw_db.open())
	{
		emit messegeLog("Cannot open database: " + mw_db.lastError().text() + '\n');
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
		emit messegeLog("Unable to insert data" + query.lastError().text() + query.lastQuery() + '\n');
		//qDebug() << "Unable to insert data" << query.lastError() << " : " << query.lastQuery();
	}
}