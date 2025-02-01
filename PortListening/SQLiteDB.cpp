#include "SQLiteDB.h"
#include <QtDebug>


SQLiteDB::SQLiteDB(QObject* parent)
	: QObject(parent)
{
	user_counter = 0;

	QTimer::singleShot(500, [this]() {

		connectDB(); // создаём БД и подключение к ней
		// Видимо QSqlQuery query сразу знает что надо обращаться к той таблице к которой сейчас подключена программа.

		QSqlQuery query; // для возможности обращения к БД. Инкапсулирует методы обращения к БД. В SQlite может быть только один оператор в запросе.

		// запрос на создание таблицы c именем userlist
		// создаём столбец с именем number. PRIMARY KEY (первичный ключ) всегда уникален. Запись в этом поле не должна быть пустой. В одной таблице только один ключ.
		db_input = "CREATE TABLE channelTable ( "
			"number VARCHAR(20), " // NOT NULL - значение в таблице не может быть пустым. Если поставить NULL то можно будет оставлять пустым.   
			"date VARCHAR(20), " // NOT NULL - значение в таблице не может быть пустым. Если поставить NULL то можно будет оставлять пустым.    
			"channelFirst VARCHAR(20), " // VARCHAR - символьный тип данных с длиной строки указанной в скобках в байтах.
			"channelSecond VARCHAR(20), "
			"channelThird VARCHAR(20), " // INTEGER - целочисленный тип данных
			"channelFour VARCHAR(20));";


		if (!query.exec(db_input)) // Выполняем запрос. exec - вернёт true если успешно. Синтаксис должен отвечать запрашиваемой БД.
		{
			emit messegeLog("Unable to create a table" + query.lastError().text());
			//	qDebug() << "Unable to create a table" << query.lastError(); // Возвращаем информацию о последней ошибке. При вывзове exec, получая ошибку, она помещается в lastError(). Мы можем её прочитать..
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
	mw_db.removeDatabase("DataBaseMilanRF"); // удаляет соединение с БД с именем из скобок.

	emit messegeLog("Object DB was destroyed");
	//qDebug() << "Object DB was destroyed";

	exit(0);
}


bool SQLiteDB::connectDB()
{
	mw_db = QSqlDatabase::addDatabase("QSQLITE"); // указываем какой использовать драйвер для подключения к БД и имя подключения. Если имя не задано то по умолчанию подключаемся к этой базе. Вроде так.
	mw_db.setDatabaseName("DataBaseMilanRF"); // Указываем с какой БД взаимодействовать. Если такого имени не найдёт то файл БД с указанным именем будет создан.

	if (!mw_db.open()) // открываем БД. Если не открывает то вернёт false
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
	// SELECT - оператор запроса. Пример запроса определённых столбцов (name, age) из таблицы (employees): "SELECT name, age FROM employees;".
	// Для выбора всех столбцов используется оператор *: "SELECT * FROM employees;"
	// COUNT - подсчитывает количество записей в таблице.
	// WHERE - позволит задать условие если дописать его в конце

	QString str_t = "SELECT COUNT(*) "
		"FROM channelTable;";

	db_input = str_t;

	if (!query.exec(db_input)) // Отправляем запрос на количество записей
	{
		qDebug() << "Unable to get number " << query.lastError() << " : " << query.lastQuery();
		return;
	}
	else
	{

		query.next(); // преходим на следующиую строку (если exec вернул 8 то переходим к 9-ой строке)

		rec = query.record();// record - возвращает QSqlRecord содержащий информацию относящуюся к запросу
		// number = a_query.value(rec.indexOf("number")).toInt(); - так можно получить индекс столбца "number"

		// поля нумеруются слева направо. Например в "SELECT forename, surname FROM people;" поле 0 это "forename" а поле 1 это "surname".
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












