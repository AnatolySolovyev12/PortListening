#include "SQLiteDB.h"
#include <QtDebug>


SQLiteDB::SQLiteDB(QObject* parent)
	: QObject(parent)
{
	user_counter = 0;

	QTimer::singleShot(500, [this]() {

		connectDB(); // ������ �� � ����������� � ���
		// ������ QSqlQuery query ����� ����� ��� ���� ���������� � ��� ������� � ������� ������ ���������� ���������.

		QSqlQuery query; // ��� ����������� ��������� � ��. ������������� ������ ��������� � ��. � SQlite ����� ���� ������ ���� �������� � �������.

		// ������ �� �������� ������� c ������ userlist
		// ������ ������� � ������ number. PRIMARY KEY (��������� ����) ������ ��������. ������ � ���� ���� �� ������ ���� ������. � ����� ������� ������ ���� ����.
		db_input = "CREATE TABLE channelTable ( "
			"number VARCHAR(20), " // NOT NULL - �������� � ������� �� ����� ���� ������. ���� ��������� NULL �� ����� ����� ��������� ������.   
			"date VARCHAR(20), " // NOT NULL - �������� � ������� �� ����� ���� ������. ���� ��������� NULL �� ����� ����� ��������� ������.    
			"channelFirst VARCHAR(20), " // VARCHAR - ���������� ��� ������ � ������ ������ ��������� � ������� � ������.
			"channelSecond VARCHAR(20), "
			"channelThird VARCHAR(20), " // INTEGER - ������������� ��� ������
			"channelFour VARCHAR(20));";


		if (!query.exec(db_input)) // ��������� ������. exec - ����� true ���� �������. ��������� ������ �������� ������������� ��.
		{
			emit messegeLog("Unable to create a table" + query.lastError().text());
			//	qDebug() << "Unable to create a table" << query.lastError(); // ���������� ���������� � ��������� ������. ��� ������� exec, ������� ������, ��� ���������� � lastError(). �� ����� � ���������..
		}
		else
		{
			emit messegeLog("Table was create!");
			//qDebug() << "Table was create!";
		}

		});
}


SQLiteDB::~SQLiteDB()
{
	mw_db.removeDatabase("DataBaseMilanRF"); // ������� ���������� � �� � ������ �� ������.

	emit messegeLog("Object DB was destroyed");
	//qDebug() << "Object DB was destroyed";

	exit(0);
}


bool SQLiteDB::connectDB()
{
	mw_db = QSqlDatabase::addDatabase("QSQLITE"); // ��������� ����� ������������ ������� ��� ����������� � �� � ��� �����������. ���� ��� �� ������ �� �� ��������� ������������ � ���� ����. ����� ���.
	mw_db.setDatabaseName("DataBaseMilanRF"); // ��������� � ����� �� �����������������. ���� ������ ����� �� ����� �� ���� �� � ��������� ������ ����� ������.

	if (!mw_db.open()) // ��������� ��. ���� �� ��������� �� ����� false
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

	/*
	QSqlRecord rec;
	// SELECT - �������� �������. ������ ������� ����������� �������� (name, age) �� ������� (employees): "SELECT name, age FROM employees;".
	// ��� ������ ���� �������� ������������ �������� *: "SELECT * FROM employees;"
	// COUNT - ������������ ���������� ������� � �������.
	// WHERE - �������� ������ ������� ���� �������� ��� � �����

	QString str_t = "SELECT COUNT(*) "
		"FROM channelTable;";

	db_input = str_t;

	if (!query.exec(db_input)) // ���������� ������ �� ���������� �������
	{
		qDebug() << "Unable to get number " << query.lastError() << " : " << query.lastQuery();
		return;
	}
	else
	{

		query.next(); // �������� �� ���������� ������ (���� exec ������ 8 �� ��������� � 9-�� ������)

		rec = query.record();// record - ���������� QSqlRecord ���������� ���������� ����������� � �������
		// number = a_query.value(rec.indexOf("number")).toInt(); - ��� ����� �������� ������ ������� "number"

		// ���� ���������� ����� �������. �������� � "SELECT forename, surname FROM people;" ���� 0 ��� "forename" � ���� 1 ��� "surname".
		user_counter = rec.value(0).toInt();
	}


	user_counter++;
	*/

	db_input = some;

	//qDebug() << db_input;

	if (!query.exec(db_input))
	{
		emit messegeLog("Unable to insert data" + query.lastError().text() + query.lastQuery());
		//qDebug() << "Unable to insert data" << query.lastError() << " : " << query.lastQuery();
	}
}












