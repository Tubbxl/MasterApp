#include "serialthread.h"
/*
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
}*/
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QByteArray>

Serialthread::Serialthread( )
{
    stopped=false;
 /*   memset(path,0x00,sizeof(path));
    if(in_path!=NULL)
        memcpy(path,in_path,strlen(in_path));
    qDebug("%s",path);*/
}

Serialthread::~Serialthread()
{

}
/*
extern "C" {
int UartRecv(unsigned char ComID,unsigned char *strBuf, unsigned short *p_read_len, unsigned short timeout,unsigned short nbyte)
{
    int  nleft, nread, nRet;
    fd_set rfds;
    struct timeval tv;

    if (ComID< 0)
    {
        return 101;
    }

    nleft = nbyte;

    FD_ZERO(&rfds);
    FD_SET(ComID, &rfds);
    tv.tv_sec = timeout/1000;
    tv.tv_usec = timeout %1000*1000;

    *p_read_len = 0;
    while(nleft > 0)
    {
        nRet = select(ComID+1, &rfds, NULL, NULL, &tv);
        if (nRet > 0)
        {
            nread = read(ComID, strBuf, nleft);
            if (nread <= 0)
            {
                return 101;
            }
            nleft -= nread;
            strBuf += nread;
        }
        else if (nRet == 0)
        {
            if(tv.tv_sec == 0 && tv.tv_usec == 0)
            {
                *p_read_len = (nbyte-nleft);
                if(*p_read_len==0) {return 102;}
                    return 0;
            }
        }
        else
        {
              return 101;
        }
    }
    *p_read_len = (nbyte-nleft);
    return 0;
}


int UartSend(unsigned char ComID, unsigned char *strBuf, unsigned short nByte)
{
    int  nLeft, nwrite, nRet;
    fd_set wfds;
    struct timeval tv;
    int nTimeOut = 3;

    if (ComID<0)
    {
        return -1;
    }

    FD_ZERO(&wfds);
    FD_SET(ComID, &wfds);
    tv.tv_sec = nTimeOut;
    tv.tv_usec = 0;

    nLeft = nByte;

    while (nLeft > 0)
    {
        nRet = select(ComID+1, NULL, &wfds, NULL, &tv);
        if (nRet > 0)
        {
            nwrite = write(ComID, strBuf, nLeft);
            //LOGD("nwrite = %d\n", nwrite);
            if (nwrite <= 0)
            {
                return (nByte - nLeft);
            }
            nLeft -= nwrite;
            strBuf += nwrite;
        }
    }
    return (nByte - nLeft);
}
void ComInit(unsigned char ComID,unsigned int Boudrate)
{
    int status;
    struct termios   options;

    tcflush(ComID,TCIOFLUSH);
    tcgetattr(ComID, &options);

    switch(Boudrate)
    {
        case 9600:
           options.c_cflag = B9600|CS8|CLOCAL|CREAD;
           break;
        case 115200:
           options.c_cflag = B115200|CS8|CLOCAL|CREAD;
           break;
        case 230400:
            options.c_cflag = B230400|CS8|CLOCAL|CREAD;
            break;
           break;
    }

    options.c_iflag = IGNPAR;
    options.c_lflag = 0;
    options.c_oflag = 0;
    options.c_cc[VTIME]=3;
    options.c_cc[VMIN] =200;
    status = tcsetattr(ComID,TCSANOW,&options);

    if(status!=0)
    {
            close(ComID);
            printf("uart init ERR_DEVICE");
            return ;
    }

    tcflush(ComID,TCIOFLUSH);
    return ;
}
}
void Serialthread::run()
{
    while(!stopped)
    {
        if(stopped&&com_opened)
        {
            close(fb);
        }
         if(!com_opened)
         {
             if((fb = open(path, O_RDWR | O_NDELAY|O_NOCTTY)) == -1)
                 return ;
            qDebug("fb=%d",fb);
             ComInit(fb ,115200);
             com_opened=true;
         }
         if(com_opened&&tx_event)
         {
              qDebug("senddata");
                tx_event = false;
                char *mm = TxData.data();
                UartSend(fb,(unsigned char *)mm,strlen(mm));
                this->msleep(10);
                unsigned short len=0;
                unsigned char readbuf[100]={0};
                 UartRecv(fb,readbuf,&len,100,strlen(mm));
                if(len>0)
                {
                    qDebug("readdata %s",readbuf);
                    requestData= (char*)readbuf;
                }
         }

    }

}
*/


#if 1
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
void Serialthread::stop()
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

