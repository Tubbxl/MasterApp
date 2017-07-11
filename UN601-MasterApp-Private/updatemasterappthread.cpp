#include "updatemasterappthread.h"

#include "mfile.h"

#include "mappdefines.h"

extern "C"
{
#include "include/dll_api.h"
}

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include <stdlib.h>
#include <unistd.h>
 #include<fcntl.h>

UpdateMasterAppThread::UpdateMasterAppThread(QString strFilePath, QObject *parent) :
    QThread(parent)
{
      mFilePath = strFilePath;

       mMFile = new MFile();
}

UpdateMasterAppThread::~UpdateMasterAppThread()
{

}

void UpdateMasterAppThread::refreshPromptAndExit(QString strPrompt)
{
     refreshPromptSignal(strPrompt);
    sleep(2);
     closePromptSignal();
}


void UpdateMasterAppThread::run()
{
    int iRet = -1;

     refreshPromptSignal("Update master app, please waiting...");

    if(NULL==opendir("/home/root/masterapp"))
    {
         iRet = mkdir("/home/root/masterapp",0775);
         if(0 != iRet)
         {
             refreshPromptAndExit("Create master app folder failed!");
            return ;
         }
    }

    if(0 != access("/home/root/temp",F_OK))
    {
         iRet = system("mkdir /home/root/temp");
         if(iRet < 0)
         {
             refreshPromptAndExit("Create temp folder failed!");
            return ;
         }
    }

    char szShellCmd[256] = {0};
     QByteArray byArrFilePath = mFilePath.toLatin1();
     memset(szShellCmd, 0x00, sizeof(szShellCmd));
     sprintf(szShellCmd, "cp -rf %s /home/root/temp/", byArrFilePath.data());
     qDebug("UpdateSubAppThread---szShellCmd:[%s]", szShellCmd);
     iRet = system(szShellCmd);
       system("sync");
     if(0 != iRet)
     {
          refreshPromptAndExit("Copy master file to temp folder failed!");
         return ;
     }



     char filePath[256] = {0};
    memcpy(filePath, byArrFilePath.data(), strlen(byArrFilePath.data()));

        if(RELEASE_MODE == gb_RunMode)
         {
            //sign verify
            unsigned char signBuf[SIGN_DATA_BYTE_LEN+1] = {0};
             iRet = mMFile->getFileSignData(filePath, signBuf);
             if(0 != iRet)
             {
                  refreshPromptAndExit("Get masterapp sign data failed!");
                 return ;
             }

             iRet = DLL_PciVerifySignature(0x00, signBuf);
             if(0 != iRet)
             {
                  refreshPromptAndExit("Master app sign verify failed!");
                 return ;
             }
       }



    sprintf(szShellCmd, "rm -rf /home/root/masterapp/MasterApp");
    qDebug("UpdateMasterAppThread---szShellCmd:[%s]", szShellCmd);
    iRet = system(szShellCmd);



      qDebug("UpdateMasterAppThread---filePath:[%s]", filePath);
    sprintf(szShellCmd, "cp -rf %s /home/root/masterapp/", filePath);
    qDebug("UpdateMasterAppThread---szShellCmd:[%s]", szShellCmd);
    iRet = system(szShellCmd);
    system("sync");
    if(0 != iRet)
    {
         refreshPromptAndExit("Update master app failed!");
        return ;
    }

     refreshPromptAndExit("Update master app finish, please reboot!");

          DLL_SysReboot();
}

