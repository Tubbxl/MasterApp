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
    DLL_EthSetLocalIPDhcp();
    emit ethconnetret(0);
}

