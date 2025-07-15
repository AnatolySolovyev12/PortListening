#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "SQLiteDB.h"
#include "qtimer.h"

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(int any, QObject* parent = nullptr);

    SQLiteDB* returnPtrDb();
    QString converFuncString(QString& any);
    QString crc16Modbus(const QByteArray& data);
    QByteArray serialArrayRotate(QByteArray testNumber);
    void newDayBuffer();
    void readDeviceFile();
    QString getQueueInfo();
    void checkTodayValues();
    void queueRefresh();
    QString getPort();
    QList<QByteArray> getfullSerialBuffConstant();
    void addDeviceInArray(QByteArray any);

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();

signals:
    void messegeLog(const QString&, QColor col);

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

    QString answerListMilur;
    QString zeroBuff = QString(100, '0');
    QString strZero = QString(60, '0');

    QList<QByteArray>serialBuff;
    QList<QByteArray>fullSerialBuffConstant;
    QList<QByteArray>threeFazeBuff;
    QList<QByteArray>threeFazeBuffTwoZero;

    bool TwoZero = false;
    bool treeFazeBool = false;

    QString pattern = "3335";
    QString todayDate;
    QTimer* dateTImer = nullptr;;
};

#endif // MYTCPSERVER_H