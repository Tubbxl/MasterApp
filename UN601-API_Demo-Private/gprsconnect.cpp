#include "gprsconnect.h"
#include <QDebug>
extern "C" {
#include "dll_api.h"
}
gprsconnect::gprsconnect(QObject *parent) : QThread(parent)
{

}

gprsconnect::gprsconnect(unsigned char *input_apn, unsigned char *input_name,  unsigned char *input_pswd,  unsigned char *input_tel)
{
if(input_apn!=NULL)
    memcpy(apn,input_apn,strlen((char *)input_apn));
    if(input_name!=NULL)
    memcpy(name,input_name,strlen((char *)input_name));
    if(input_pswd!=NULL)
    memcpy(pswd,input_name,strlen((char *)input_pswd));
if(input_tel!=NULL)
    memcpy(tel,input_tel,strlen((char *)input_tel));
    printf("input_apn:%s,input_tel:%s\n",input_apn,input_tel);
}

gprsconnect::~gprsconnect()
{
}

void gprsconnect::run()
{
    int ret = -1;
    qDebug()<<"statr dial"<<endl;
    printf("%s %s\n ",apn,tel);
    DLL_PppDial(apn,name,pswd,tel);
    QThread::sleep(6);
    ret=DLL_PppStatus();
    qDebug()<<"DLL_PppStatus"<< ret<<endl;
    emit gprscntret(ret);
    qDebug()<<"gprscntret(&ret);"<< ret<<endl;

}

