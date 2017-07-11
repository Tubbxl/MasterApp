#include "updateapiserversthread.h"

#include <stdio.h>

UpdateApiserversThread::UpdateApiserversThread(QString strFilePath, QObject *parent) :
    QThread(parent)
{
      mFilePath = strFilePath;
}

UpdateApiserversThread::~UpdateApiserversThread()
{

}

void UpdateApiserversThread::refreshPromptAndExit(QString strPrompt)
{
     refreshPromptSignal(strPrompt);
    sleep(2);
     closePromptSignal();
}


void UpdateApiserversThread::run()
{
    int iRet = -1;

     refreshPromptSignal("Update apiserver, please waiting...");

    char szShellCmd[256] = {0};
    QByteArray byArrFilePath = mFilePath.toLatin1();
    sprintf(szShellCmd, "cp -rf %s /home/root/apiserver", byArrFilePath.data());
    qDebug("UpdateApiserversThread---szWriteCmd:[%s]", szShellCmd);
    iRet = system(szShellCmd);
    if(0 != iRet)
    {
         refreshPromptAndExit("Update apiserver  failed!");
        return ;
    }

     refreshPromptAndExit("Update apiserver finish, please reboot!");
}

