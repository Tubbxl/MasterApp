#include "ethconnect.h"
#include <QDebug>
extern "C" {
#include "dll_api.h"
}
ethconnect::ethconnect(QObject *parent) : QThread(parent)
{

}

void ethconnect::run()
{
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
}

