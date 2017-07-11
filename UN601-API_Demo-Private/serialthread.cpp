#include "serialthread.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QByteArray>

extern "C" {
#include "dll_api.h"
}

Serialthread::Serialthread()
{
    stopped=false;
    //mComPort = *comPort;
    //qDebug("%s",path);
}

Serialthread::~Serialthread()
{

}

void Serialthread::run()
{
    unsigned short len=0;
    unsigned char tempbuf[100]={0};
    unsigned short len1=0;
    unsigned char readbuf[100]={0};
    while(!stopped)
    {
        if(stopped&&com_opened)
        {
            DLL_ExternalSerialClose();
        }
         if(!com_opened)
         {
             int iret =  DLL_ExternalSerialOpen();
             if(iret <0)
                     return;
             com_opened=true;
         }
         if(com_opened&&tx_event)
         {
              qDebug("senddata");
                tx_event = false;
                char *mm = TxData.data();
                DLL_ExternalSerialSend((unsigned char *)mm,strlen(mm));
                this->msleep(1);
                 DLL_ExternalSerialRecv(readbuf, &len1, 300,20);
                if(len1>0)
                {
                    len1 = 0;
                    qDebug("readdata %s",readbuf);
                    requestData= (char*)readbuf;
                    emit comRecive();
                }
         }
        DLL_ExternalSerialRecv(tempbuf, &len, 100,20);
        if(len>0)
        {
            qDebug("readdata %s",readbuf);
            len = 0;
            requestData= (char*)tempbuf;
            emit comRecive();
        }
    }
}



#if 0
void Serialthread::run()
{
    QSerialPort *serialport= new QSerialPort;

    while(!stopped)
    {
        if(stopped&&com_opened)
        {
            serialport->close();
            com_opened=false;

        }
        if(!com_opened)
        {
            //open com
            com_opened=true;
            serialport->setPortName(portnum);
            serialport->open(QIODevice::ReadWrite);
            serialport->setBaudRate(115200);
            serialport->setDataBits(QSerialPort::Data8);
            serialport->setParity(QSerialPort::NoParity);
            serialport->setStopBits(QSerialPort::OneStop);
            serialport->setFlowControl(QSerialPort::NoFlowControl);
            com_opened=true;
            qDebug()<<"open com port :"<<portnum ;
        }
        if(com_opened&&tx_event)
         {
                 tx_event=false;
                 serialport->clear(QSerialPort::AllDirections);
                 qDebug() <<"send data to "<<"["<<portnum<<"]";
                 qDebug() << "arr size:" <<TxData.length();
                serialport->write(TxData);
               if (serialport->waitForBytesWritten(5))
               {
                 qDebug() << "[" <<portnum<<"]"<<"send data success";
                 if (serialport->waitForReadyRead(1500))  //1s
                 {
                     requestData = serialport->readAll();
                     while (serialport->waitForReadyRead(15))
                     requestData += serialport->readAll();
                     emit comRecive();
                 }else
                 {
                         qDebug() << "wait return time out";
                 }
               }else
               {
                   qDebug() << "send time out";
               }
         }
    if (serialport->waitForReadyRead(5))
    {
        while (serialport->waitForReadyRead(5))
        this->msleep(20);
        requestData = serialport->readAll();
        emit comRecive();
    }
    if(stopped&&com_opened)
    {
        serialport->close();
        com_opened=false;
    }
    }
}
#endif
void Serialthread::setstop()
{
   stopped=true;

}
void Serialthread::startCom()
{
   stopped=false;

}
void Serialthread::changeComState(bool stat)
 {
        com_opened=stat;
}
void Serialthread::setMessage(const QString &message)
{
   messageStr = message;

}
void Serialthread::setPortnum(const QString &num)
{
    portnum=num;

}
void Serialthread:: changeTxState(bool stat)
{
        tx_event=stat;
}
void Serialthread:: changeRxState(bool stat)
{
         rx_event=stat;
}

