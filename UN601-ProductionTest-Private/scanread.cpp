#include "scanread.h"
extern"C"{

#include "dll_api.h"
}
scanread::scanread(QObject *parent) : QThread(parent)
{
 memset(readdata,0x00,sizeof(readdata));
}

void scanread::run()
{
    int len = 0;
     len  = DLL_ScanRead(readdata);
     qDebug("in thread");
     printf(" in thread readdata = %s  %d\n",readdata,len);
     emit sendreaddate(&len , (unsigned char *)readdata);
}

