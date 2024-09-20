#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>


#include <SQLiteDB.h>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject* parent = 0);

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();

private:
    QTcpServer* mTcpServer = nullptr;
    QTcpSocket* mTcpSocket = nullptr;
    SQLiteDB* dataWrite = nullptr;
};

#endif // MYTCPSERVER_H