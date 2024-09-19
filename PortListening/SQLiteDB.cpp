#include "SQLiteDB.h"

#include <QtDebug>

main_window::main_window(QWidget* parent)
	: QMainWindow(parent)
{
	user_counter = 0;
	m_loginSuccesfull = false;

	connectDB(); // создаём БД и подключение к ней
	// Видимо QSqlQuery query сразу знает что надо обращаться к той таблице к которой сейчас подключена программа.

	QSqlQuery query; // для возможности обращения к БД. Инкапсулирует методы обращения к БД. В SQlite может быть только один оператор в запросе.

	// запрос на создание таблицы c именем userlist
	// создаём столбец с именем number. PRIMARY KEY (первичный ключ) всегда уникален. Запись в этом поле не должна быть пустой. В одной таблице только один ключ.
	db_input = "CREATE TABLE channelTable ( "
		"number VARCHAR(20), " // NOT NULL - значение в таблице не может быть пустым. Если поставить NULL то можно будет оставлять пустым.    
		"channelFirst VARCHAR(20), " // VARCHAR - символьный тип данных с длиной строки указанной в скобках в байтах.
		"channelSecond VARCHAR(20), "
		"channelThird VARCHAR(20), " // INTEGER - целочисленный тип данных
		"channelFour VARCHAR(20));";


	if (!query.exec(db_input)) // Выполняем запрос. exec - вернёт true если успешно. Синтаксис должен отвечать запрашиваемой БД.
	{
		qDebug() << "Unable to create a table" << query.lastError(); // Возвращаем информацию о последней ошибке. При вывзове exec, получая ошибку, она помещается в lastError(). Мы можем её прочитать..
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
		QString str_t = "UPDATE userlist " // INSERT - обновляем знаачения в заданных столбцах таблицы 
			"SET xpos = %2, ypos = %3, width = %4, length = %5 " // SET - задаём список полей и их новые значения
			"WHERE name = '%1';"; // WHERE - условие
		db_input = str_t.arg(m_username)
			.arg((this->x()) + 1) // записываем последнее положение и размеры окна в базу. +1 и +30 нивелирует необоснованное смещение окна в сторону после закрытия
			.arg((this->y()) + 30) // 
			.arg(this->width())
			.arg(this->height());
		QSqlQuery query;
		if (!query.exec(db_input))
		{
			qDebug() << "Unable to insert data" << query.lastError() << " : " << query.lastQuery();
		}
	}
	mw_db.removeDatabase("DataBaseMilanRF"); // удаляет соединение с БД с именем из скобок.

	qDebug() << "MainWindow Destroyed";

	exit(0);
}


bool main_window::connectDB()
{
	mw_db = QSqlDatabase::addDatabase("QSQLITE"); // указываем какой использовать драйвер для подключения к БД и имя подключения. Если имя не задано то по умолчанию подключаемся к этой базе. Вроде так.
	mw_db.setDatabaseName("DataBaseMilanRF"); // Указываем с какой БД взаимодействовать. Если такого имени не найдёт то файл БД с указанным именем будет создан.

	if (!mw_db.open()) // открываем БД. Если не открывает то вернёт false
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
	// SELECT - оператор запроса. Пример запроса определённых столбцов (name, age) из таблицы (employees): "SELECT name, age FROM employees;".
	// Для выбора всех столбцов используется оператор *: "SELECT * FROM employees;"
	// COUNT - подсчитывает количество записей в таблице.
	// WHERE - позволит задать условие если дописать его в конце

	QString str_t = "SELECT COUNT(*) "
		"FROM userlist;";

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
		qDebug() << "Was user_counter = " << user_counter;
	}


	user_counter++;
	qDebug() << "Now user_counter = " << user_counter << Qt::endl;

	// INSERT + VALUES - определяет одинокую строку со значениями описанными в VALUES. INTO позволяет указать в какую именно таблицу произвести запись
	str_t = "INSERT INTO channelTable(number, channelFirst, channelSecond, channelThird, channelFour)"
		"VALUES(%1, '%2', '%3', %4, %5);"; // VALUES - определяет те значения которые будут записаниы в строку



	db_input = str_t.arg(user_counter) // .arg - заменяет маркер места с наименьшим номером строкой или значением.

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












