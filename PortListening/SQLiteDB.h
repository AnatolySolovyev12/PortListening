#pragma once

#include <QtWidgets/QMainWindow>


#include <QString>

//#include <QSqlDatabase>
#include <QtSql/QtSql>

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window(QWidget* parent = nullptr);
    ~main_window();

    bool connectDB(); // прототип метода подключения к БД

    QString db_input; // строка для отправки запроса к БД

    QSqlDatabase mw_db; // экземпляр подключения к БД

    int user_counter = 0; // счетчик пользователей
    bool m_loginSuccesfull = false; // флаг успешной авторизации

    int myTest = 0;


private:

    void writeData();

};
