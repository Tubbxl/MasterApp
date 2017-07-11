#include "wificonnectth.h"
#include <QDebug>

wificonnectth::wificonnectth(QObject *parent) : QThread(parent)
{



}

wificonnectth::wificonnectth(T_WLANDEV network, unsigned char *mm)
{
    cntwork = network;
    memset(MM,0x00,20);
    memcpy(MM,mm,20);

}

wificonnectth::~wificonnectth()
{

}

void wificonnectth::run()
{
    int ret = -1;
    ret = DLL_WifiConnectNetwork(cntwork,MM);
    qDebug()<<"DLL_WifiConnectNetwork:"<<ret<<endl;
    qDebug()<<"connect end:"<<ret<<endl;
    emit connectend(ret);
     qDebug()<<"emit connectend:"<<endl;
}

