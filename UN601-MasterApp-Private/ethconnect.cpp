#include "ethconnect.h"
#include <QDebug>
extern "C" {
#include "include/dll_api.h"
}
ethconnect::ethconnect(QObject *parent) : QThread(parent)
{

}

void ethconnect::run()
{
   #if 0
    unsigned char IP[14]={0};
    unsigned char mask[13];
    unsigned char mac[17];
    unsigned char gateway[13];
    memcpy(IP,"192.168.1.166",13);
    memcpy(mask,"255.255.255.0",13);
    memcpy(mac,"1B:2B:3B:4B:5B:6B",17);
    memcpy(gateway,"192.168.1.255",13);
    qDebug()<<"set ip satrt";
   #endif

#if 0
    DLL_EthInit();
    DLL_EthSetLocalIP(IP);
    qDebug()<<"set ip successful";
#endif

#if 1
    DLL_EthInit();
    msleep(3000);
    if(0==system("udhcpc -i eth0 -n"))
    {
        emit ethconnetret(0);
        qDebug()<<"ethconnetret successful";

    }else{
            emit ethconnetret(1);
            qDebug()<<"ethconnetret failed";
    }
 #endif

}

