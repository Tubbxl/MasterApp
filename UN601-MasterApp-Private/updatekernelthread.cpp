#include "updatekernelthread.h"

extern "C"
{
#include "include/dll_api.h"
}

#include <stdio.h>


UpdateKernelThread::UpdateKernelThread(QString strFilePath, QObject *parent) :
    QThread(parent)
{
      mFilePath = strFilePath;
}

UpdateKernelThread::~UpdateKernelThread()
{

}

void UpdateKernelThread::refreshPromptAndExit(QString strPrompt)
{
     refreshPromptSignal(strPrompt);
    sleep(1);
     closePromptSignal();
}


void UpdateKernelThread::run()
{
    int iRet = -1;

     refreshPromptSignal("Update Kernel, please waiting...");

    QString strEraseCmd = "flash_erase /dev/mtd1 0 0";
    QByteArray byArrEraseCmd = strEraseCmd.toLatin1();
    iRet = system(byArrEraseCmd.data());
    if(0 != iRet)
    {
        refreshPromptAndExit("Erase flash failed!");
        return ;
    }

    char szWriteCmd[256] = {0};
    QByteArray byArrFilePath = mFilePath.toLatin1();
    sprintf(szWriteCmd, "nandwrite -p /dev/mtd1 -p %s", byArrFilePath.data());
    qDebug("UpdateLinuxKernelThread---szWriteCmd:[%s]", szWriteCmd);
    iRet = system(szWriteCmd);
    if(0 != iRet)
    {
         refreshPromptAndExit("Write flash failed!");
        return ;
    }

     refreshPromptAndExit("Update Kernel finish, please reboot!");
     DLL_SysReboot();
}

