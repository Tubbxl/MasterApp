#include "updatesyslibthread.h"

#include <stdio.h>

UpdateSysLibThread::UpdateSysLibThread(QString strFilePath, QObject *parent) :
    QThread(parent)
{
      mFilePath = strFilePath;
}

UpdateSysLibThread::~UpdateSysLibThread()
{

}

void UpdateSysLibThread::refreshPromptAndExit(QString strPrompt)
{
     refreshPromptSignal(strPrompt);
    sleep(2);
     closePromptSignal();
}


void UpdateSysLibThread::run()
{
    int iRet = -1;

     refreshPromptSignal("Update System Lib, please waiting...");

    char szShellCmd[256] = {0};
    QByteArray byArrFilePath = mFilePath.toLatin1();
    sprintf(szShellCmd, "cp -rf %s /usr/lib/", byArrFilePath.data());
    qDebug("UpdateSysLibThread---szWriteCmd:[%s]", szShellCmd);
    iRet = system(szShellCmd);
    if(0 != iRet)
    {
         refreshPromptAndExit("Update system lib failed!");
        return ;
    }

     refreshPromptAndExit("Update system lib finish, please reboot!");
}

