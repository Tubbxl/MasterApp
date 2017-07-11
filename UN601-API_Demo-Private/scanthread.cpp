#include "scanthread.h"

extern "C" {
#include "dll_api.h"
}

ScanThread::ScanThread()
{
    stopflag = false;
}





void ScanThread::run()
{
    char str[100]={0};
    int ret = -1;

    ret=DLL_ScanOpen();
    if(ret==0)
    {
      emit  sendMessage("The scanner open successful!\n");
        ret=DLL_ScanTrigger();
    }
    if(ret == 0)
    {
        emit sendMessage("The scanner trigger successful!\n");
        emit sendMessage("Please scan code!\n");
        ret=DLL_ScanRead(str);
        printf("str is %s\r\n",str);
        if(ret == 0)
        {
            emit sendMessage("Read successful!\n");
            emit sendMessage("Data is:");
            emit sendMessage(str);

        }
    }
    DLL_ScanClose();
}
