#include <QCoreApplication>
#include "mytcpserver.h"
#include <QTextStream>

QTextStream myout(stdout);

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    MyTcpServer server;

    /*
    bool ok;
    QString test = "17D90195";

    uint valTrans = test.toUInt(&ok, 16);
    qDebug() << test << " is " << valTrans;
    */
   
    /*
   
    QByteArray array = "95 01 D9 17 04 02 10 3E 03 10 25 06 10 0B 07 10 1B 31 75 37 38 00 4D 49 4C 41 4E 20 52 46 20 30 34 00 00 00 00 31 39 30 30 34 30 34 30 30 30 39 37 36 38 35 14 04 16 57 0E 00 00 00 00 4E E3 1B B6 1D 00 36 07 00 00 00 00 0C 01 01 11 01 00 EB 18 00 00 04 00 00 00 52 06 00 00 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 62 00 0A 00 60 00 5A 00 29 05 16 1D 08 18 06 00 00 05 00 05 00 00 06 13 00 9F 06 00 1F 21 00 00 D4 2D 46 52 44 4D 01 04 64 00 2C 01 64 00 96 B1";

        qDebug() << array << "\n";

        // mTcpSocket->write(array); Эхо эффект с отправкой принятого обратно сокету

        QString str = array;

        for (int i = 0; i < str.size(); i++)
        {
            if (str[i].isSpace())
                str.remove(i,1);
        }

        qDebug() << str << "\n";

        QString temporary;

        QString middleString;

        QString translate;

        int counter = 0;

        bool ok;


        QList <QString> myList;

        for (auto val : str)
        {
            ++counter;

            temporary += val;

            if (counter == 2)
            {
                myList.append(temporary);
                temporary = "";
                counter = 0;
            }
        }

        QString numberStr;

        for (int counter = 3; counter >= 0; counter--)
        {

            numberStr += myList[counter];

        }

        uint valTrans = numberStr.toUInt(&ok, 16);
        qDebug() << "Number - " << valTrans;

        QString first;
        QString two;
        QString three;
        QString four;

        for (int counter = 94; counter >= 79; --counter)
        {
            
            if (counter >= 91)
            {
                four += myList[counter];
                continue;
            }

            if (counter >= 87)
            {
                three += myList[counter];
                continue;
            }

            if (counter >= 83)
            {
                two += myList[counter];
                continue;
            }

            if (counter >= 79)
            {
                first += myList[counter];
                continue;
            }
            
        }
       // myout << Qt::endl;

         valTrans = first.toUInt(&ok, 16);
        qDebug() << "first - " << first << " - " << valTrans;

         valTrans = two.toUInt(&ok, 16);
        qDebug() << "two - " << two << " - " << valTrans;

         valTrans = three.toUInt(&ok, 16);
        qDebug() << "three - " << three << " - " << valTrans;

         valTrans = four.toUInt(&ok, 16);
        qDebug() << "four - " << four << " - " << valTrans;

*/
        









    //РАЗВОРАЧИВАЕМ МАССИВ
   /* QString str = "95 01 D9 17 04 02 10 3E 03 10 25 06 10 0B 07 10 1B 31 75 37 38 00 4D 49 4C 41 4E 20 52 46 20 30 34 00 00 00 00 31 39 30 30 34 30 34 30 30 30 39 37 36 38 35 14 04 16 5A 0E 00 00 00 00 4E E3 1B B6 1C 00 36 07 00 00 00 00 0C 01 01 11 01 00 E8 18 00 00 04 00 00 00 4F 06 00 00 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 62 00 0A 00 60 00 5A 00 39 01 16 1D 08 18 06 00 00 05 00 05 00 00 06 13 00 9F 06 00 1F 21 00 00 D4 2D 46 52 44 4D 01 04 64 00 2C 01 64 00 54 B5";

    QString middle;

    QList <QString> myList;

    for (auto& val : str)
    {
        middle += val;
        if (val.isSpace())
        {

            myList.push_front(middle);
            middle = "";

        }
        
    }

    myList.push_front(middle);

    for (auto& val : myList)
    {

        myout << val << " ";
    }

    myout << Qt::endl;

    
    */

    /*
    int test = 149;

    

    QChar someChar;
    
    someChar = static_cast<QChar>(test);
    qDebug() << someChar;

    */
    
    return a.exec();
}