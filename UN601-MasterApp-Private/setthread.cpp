#include "setthread.h"


SetThread::SetThread(QObject *parent) : QThread(parent)
{

}


void SetThread::run()
{
   // while(true==iswifiFlag)
 /*   {
        qDebug()<<"scan wifi ="<<endl;

        int i = 0;
        QString str;
        int temp;
        DLL_WifiOpen();
        num = DLL_WifiScan(network);
        temp = num;
        qDebug()<<"num:"<<num<<endl;
        if(num>0)
        while(num--)
        {

           str = QString(QLatin1String((unsigned char*)network[i++].SSID));
           qDebug()<<"SSID:"<<str<<"  "<<i<<endl;

         }
        num = temp;
        emit sendwifi(network,&num);
        iswifiFlag = false;
        //msleep(1);
    }
*/
}

void SetThread::connectwifi(unsigned char *id, unsigned char *mm)
{
    ID=id;
    MM =mm;
}
