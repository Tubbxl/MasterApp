#include "updatebootthread.h"

extern "C"
{
#include "include/dll_api.h"
}

#include <stdio.h>

UpdateBootThread::UpdateBootThread(QString strFilePath, QObject *parent) :
    QThread(parent)
{
      mFilePath = strFilePath;
}

UpdateBootThread::~UpdateBootThread()
{

}

void UpdateBootThread::refreshPromptAndExit(QString strPrompt)
{
     refreshPromptSignal(strPrompt);
    sleep(2);
     closePromptSignal();
}


void UpdateBootThread::run()
{
    int iRet = -1;

     refreshPromptSignal("Update Boot, please waiting...");

    QString strEraseCmd = "flash_erase /dev/mtd0 0 0";
    QByteArray byArrEraseCmd = strEraseCmd.toLatin1();
    iRet = system(byArrEraseCmd.data());
    if(0 != iRet)
    {
        refreshPromptAndExit("Erase flash failed!");
        return ;
    }

    char szWriteCmd[256] = {0};
    QByteArray byArrFilePath = mFilePath.toLatin1();
    sprintf(szWriteCmd, "kobs-ng init -x -v --chip_0_device_path=/dev/mtd0 %s", byArrFilePath.data());
    qDebug("UpdateBootThread---szWriteCmd:[%s]", szWriteCmd);
    iRet = system(szWriteCmd);
    if(0 != iRet)
    {
         refreshPromptAndExit("Write flash failed!");
        return ;
    }

     refreshPromptAndExit("Update Boot finish, please reboot!");
     DLL_SysReboot();
}

