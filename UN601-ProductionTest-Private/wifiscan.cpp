#include "wifiscan.h"

wifiscan::wifiscan(QObject *parent) : QThread(parent)
{

}

wifiscan::~wifiscan()
{

}

void wifiscan::run()
{
    {
        qDebug()<<"scan wifi start... ="<<endl;

        int i = 0;
        QString str;
        int temp;
        DLL_WifiOpen();
        for(int x = 0;x<100;x++)
        {
            num = DLL_WifiScan(network);
            if(num>0)
                break;
        }
        temp = num;
        qDebug()<<"num:"<<num<<endl;
        if(num>0)
        while(num--)
        {

           str = QString(QLatin1String((char*)network[i++].ssid));
           qDebug()<<"SSID:"<<str<<"  "<<i<<endl;

         }
        num = temp;
        emit sendwifiret(network,&num);
     }
}

