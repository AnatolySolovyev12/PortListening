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

    bool connectDB(); // �������� ������ ����������� � ��

    QString db_input; // ������ ��� �������� ������� � ��

    QSqlDatabase mw_db; // ��������� ����������� � ��

    int user_counter = 0; // ������� �������������
    bool m_loginSuccesfull = false; // ���� �������� �����������

    int myTest = 0;


private:

    void writeData();

};
