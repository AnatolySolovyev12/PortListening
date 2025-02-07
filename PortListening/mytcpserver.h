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
};

#endif // MYTCPSERVER_H