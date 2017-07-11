#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QObject>
#include <QThread>

class Serialthread : public QThread
{
    Q_OBJECT
public:
    QByteArray requestData;
    QByteArray TxData;
    Serialthread();
    ~Serialthread();

    void setMessage(const QString &message);
    void setPortnum(const QString &num);
    void stop();
    void startCom();
    void changeTxState(bool stat);
    void changeRxState(bool stat);
    void changeComState(bool stat);

    //void ComInit(unsigned char ComID,unsigned int Boudrate);
   // int UartRecv(unsigned char ComID,unsigned char *strBuf, unsigned short *p_read_len, unsigned short timeout,unsigned short nbyte);
    //int UartSend(unsigned char ComID, unsigned char *strBuf, unsigned short nByte);



signals:
    void request(const QString &s);
    void comRecive();



protected:
    void run();

private:
    QString messageStr;
    QString portnum;
    volatile bool com_opened;
    volatile bool stopped;
    volatile bool tx_event;
    volatile bool rx_event;
    int fb;
    char path[100];
};

#endif // SERIALTHREAD_H
