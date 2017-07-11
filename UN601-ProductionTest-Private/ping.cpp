#include "ping.h"

ping::ping(QObject *parent) : QThread(parent)
{

}

ping::ping(int in_flag, QObject *parent)
{
    flag = in_flag;
}

void ping::run()
{
    int ret = -1;
    if(0==system("ping www.baidu.com -c 2 "))
    {
         ret = 0;
    }
    if(flag == 0)
        emit pingresult(ret);
    else if(flag == 1)
        emit wifipingresult(ret);
    else
        emit gprspingresult(ret);
}

