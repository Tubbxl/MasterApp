#ifndef WIFISCAN_H
#define WIFISCAN_H

#include <QObject>
#include <QThread>
#include <QDebug>
extern "C" {
#include "dll_api.h"
}
class wifiscan : public QThread
{
    Q_OBJECT
public:
    explicit wifiscan(QObject *parent = 0);
    ~wifiscan();
protected:
    void run();

signals:
    void sendwifiret(T_WLANDEV *network,int *num);
public slots:
private:
    int num;
    T_WLANDEV network[20];
};

#endif // WIFISCAN_H
