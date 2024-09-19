#include "SQLiteDB.h"

#include <QtDebug>

main_window::main_window(QWidget* parent)
	: QMainWindow(parent)
{
	user_counter = 0;
	m_loginSuccesfull = false;

	connectDB(); // ������ �� � ����������� � ���
	// ������ QSqlQuery query ����� ����� ��� ���� ���������� � ��� ������� � ������� ������ ���������� ���������.

	QSqlQuery query; // ��� ����������� ��������� � ��. ������������� ������ ��������� � ��. � SQlite ����� ���� ������ ���� �������� � �������.

	// ������ �� �������� ������� c ������ userlist
	// ������ ������� � ������ number. PRIMARY KEY (��������� ����) ������ ��������. ������ � ���� ���� �� ������ ���� ������. � ����� ������� ������ ���� ����.
	db_input = "CREATE TABLE channelTable ( "
		"number VARCHAR(20), " // NOT NULL - �������� � ������� �� ����� ���� ������. ���� ��������� NULL �� ����� ����� ��������� ������.    
		"channelFirst VARCHAR(20), " // VARCHAR - ���������� ��� ������ � ������ ������ ��������� � ������� � ������.
		"channelSecond VARCHAR(20), "
		"channelThird VARCHAR(20), " // INTEGER - ������������� ��� ������
		"channelFour VARCHAR(20));";


	if (!query.exec(db_input)) // ��������� ������. exec - ����� true ���� �������. ��������� ������ �������� ������������� ��.
	{
		qDebug() << "Unable to create a table" << query.lastError(); // ���������� ���������� � ��������� ������. ��� ������� exec, ������� ������, ��� ���������� � lastError(). �� ����� � ���������..
	}
	else
	{
		qDebug() << "Table was create!";
	}
}

main_window::~main_window()
{
	if (m_loginSuccesfull)
	{
		QString str_t = "UPDATE userlist " // INSERT - ��������� ��������� � �������� �������� ������� 
			"SET xpos = %2, ypos = %3, width = %4, length = %5 " // SET - ����� ������ ����� � �� ����� ��������
			"WHERE name = '%1';"; // WHERE - �������
		db_input = str_t.arg(m_username)
			.arg((this->x()) + 1) // ���������� ��������� ��������� � ������� ���� � ����. +1 � +30 ���������� �������������� �������� ���� � ������� ����� ��������
			.arg((this->y()) + 30) // 
			.arg(this->width())
			.arg(this->height());
		QSqlQuery query;
		if (!query.exec(db_input))
		{
			qDebug() << "Unable to insert data" << query.lastError() << " : " << query.lastQuery();
		}
	}
	mw_db.removeDatabase("DataBaseMilanRF"); // ������� ���������� � �� � ������ �� ������.

	qDebug() << "MainWindow Destroyed";

	exit(0);
}


bool main_window::connectDB()
{
	mw_db = QSqlDatabase::addDatabase("QSQLITE"); // ��������� ����� ������������ ������� ��� ����������� � �� � ��� �����������. ���� ��� �� ������ �� �� ��������� ������������ � ���� ����. ����� ���.
	mw_db.setDatabaseName("DataBaseMilanRF"); // ��������� � ����� �� �����������������. ���� ������ ����� �� ����� �� ���� �� � ��������� ������ ����� ������.

	if (!mw_db.open()) // ��������� ��. ���� �� ��������� �� ����� false
	{
		qDebug() << "Cannot open database: " << mw_db.lastError();
		return false;
	}

	return true;
}



void main_window::writeData()
{
	QSqlQuery query;
	QSqlRecord rec;
	// SELECT - �������� �������. ������ ������� ����������� �������� (name, age) �� ������� (employees): "SELECT name, age FROM employees;".
	// ��� ������ ���� �������� ������������ �������� *: "SELECT * FROM employees;"
	// COUNT - ������������ ���������� ������� � �������.
	// WHERE - �������� ������ ������� ���� �������� ��� � �����

	QString str_t = "SELECT COUNT(*) "
		"FROM userlist;";

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
		qDebug() << "Was user_counter = " << user_counter;
	}


	user_counter++;
	qDebug() << "Now user_counter = " << user_counter << Qt::endl;

	// INSERT + VALUES - ���������� �������� ������ �� ���������� ���������� � VALUES. INTO ��������� ������� � ����� ������ ������� ���������� ������
	str_t = "INSERT INTO channelTable(number, channelFirst, channelSecond, channelThird, channelFour)"
		"VALUES(%1, '%2', '%3', %4, %5);"; // VALUES - ���������� �� �������� ������� ����� ��������� � ������



	db_input = str_t.arg(user_counter) // .arg - �������� ������ ����� � ���������� ������� ������� ��� ���������.

		.arg("serialNumber")
		.arg("channelFirst")
		.arg("channelSecond")
		.arg("channelThird")
		.arg("channelFour");

	if (!query.exec(db_input))
	{
		qDebug() << "Unable to insert data" << query.lastError() << " : " << query.lastQuery();
	}
}












