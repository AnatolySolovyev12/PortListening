#pragma once

#include <QtWidgets/QMainWindow>


#include <QString>

//#include <QSqlDatabase>
#include <QtSql/QtSql>

class SQLiteDB : public QObject
{
    Q_OBJECT

public:
    SQLiteDB(QObject* parent = nullptr);
    ~SQLiteDB();

    bool connectDB(); // прототип метода подключения к БД

    QString db_input; // строка для отправки запроса к БД

    QSqlDatabase mw_db; // экземпляр подключения к БД

    int user_counter = 0; // счетчик пользователей

    int myTest = 0;

    void writeData(QString some);


private:

};
