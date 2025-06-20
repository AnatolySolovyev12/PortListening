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

    int recall = 0;

    bool listen = false;
    int countMessege = 0;
    int serialBuffPosition = 0;
    bool oldMessege = false;

    QList<QString>answerListMilur;
    QList<QString>zeroBuff;
    QString strZero = QString(176, '0');

    QList<QByteArray>serialBuff;
};

#endif // MYTCPSERVER_H