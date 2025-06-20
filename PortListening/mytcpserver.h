#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>


#include "SQLiteDB.h"

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(int any, QObject* parent = 0);

    SQLiteDB* returnPtrDb();
    QString converFuncString(QString& any);

    QString crc16Modbus(const QByteArray& data);
    QByteArray serialArrayRotate(QByteArray testNumber);

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();

signals:
    void messegeLog(const QString&);


private:
    QTcpServer* mTcpServer = nullptr;
    QTcpSocket* mTcpSocket = nullptr;
    SQLiteDB* dataWrite = nullptr;
    int port;


    QByteArray testArray;

    int counter = 0;

    bool startSession = false;
    bool listen = false;
    int countMessege = 0;


};

#endif // MYTCPSERVER_H