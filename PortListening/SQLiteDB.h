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

    bool connectDB();

    QString db_input; 

    QSqlDatabase mw_db;

    int myTest = 0;

    void writeData(QString some);

signals:
    void messegeLog(const QString&);

private:

};
