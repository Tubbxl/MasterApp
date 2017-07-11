#ifndef WIFICONNECTTH_H
#define WIFICONNECTTH_H

#include <QObject>
#include <QThread>
extern "C" {
#include "include//dll_api.h"
}
class wificonnectth : public QThread
{
    Q_OBJECT
public:
    explicit wificonnectth(QObject *parent = 0);
    wificonnectth(T_WLANDEV network, unsigned char *mm);
    ~wificonnectth();

protected:
    void run();

signals:
    void connectend(int ret);
public slots:


private:
    unsigned char ID[50];
    char MM[50];
    T_WLANDEV cntwork;
};

#endif // WIFICONNECTTH_H
