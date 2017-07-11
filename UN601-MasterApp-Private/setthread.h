#ifndef SETTHREAD_H
#define SETTHREAD_H

 #include <QThread>
#include <QDebug>
extern "C" {
#include "include/dll_api.h"
}
class SetThread : public QThread
{
    Q_OBJECT
public:
    explicit SetThread(QObject *parent = 0);
    void set_wifi_RunFlag(bool flag = false)
    {
        qDebug()<<"wifi thread";
        iswifiFlag = flag;
    }
    void set_gprs_RunFlag(bool flag = false)
    {
        qDebug()<<"wifi thread";
        isgprsFlag = flag;
    }
    void set_eth_RunFlag(bool flag = false)
    {
        qDebug()<<"wifi thread";
        isethFlag = flag;
    }
    bool get_wifi_RunFlag()
    {

       return isethFlag;
    }
protected:
    void run();

signals:
    void sendwifi(T_WLANDEV *network,int *num);
    void gprs();
    void eth();
public slots:
    void connectwifi(unsigned char *id,unsigned char *mm);
private :
    bool iswifiFlag;
    bool isgprsFlag;
    bool isethFlag;
    bool iswificontflag;
    int num;
    T_WLANDEV network[20];
    unsigned char *ID;
    unsigned char *MM;

};

#endif // THREAD_H
