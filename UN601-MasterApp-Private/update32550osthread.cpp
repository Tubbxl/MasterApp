#include "update32550osthread.h"

#include "mappdefines.h"

#include "user.h"

extern "C"
{
    #include "include/dll_api.h"
}

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
 #include<fcntl.h>

bool gb_RefreshMainUI = false;

Update32550OsThread::Update32550OsThread(QString strFilePath, QObject *parent) :
    QThread(parent)
{
      mFilePath = strFilePath;

       mMFile = new MFile();

 //      mpLoadApp = new LoadApp();
}

Update32550OsThread::~Update32550OsThread()
{

}

void Update32550OsThread::refreshPromptAndExit(QString strPrompt)
{
     refreshPromptSignal(strPrompt);
    sleep(2);
     closePromptSignal();
}

void Update32550OsThread::run()
{
    int iRet = -1;

    qDebug("Update32550OsThread::run");

     refreshPromptSignal("Update 32550 os, please waiting...");

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
     if(0 != iRet)
     {
          refreshPromptAndExit("Copy 32550 os file to temp folder failed!");
         return ;
     }

     char filePath[256] = {0};
    memcpy(filePath, byArrFilePath.data(), strlen(byArrFilePath.data()));
     char* fileName = (char*)mMFile->getFilename(filePath);
     memset(filePath, 0x00, sizeof(filePath));
     sprintf(filePath, "/home/root/temp/%s", fileName);
     qDebug("UpdateSubAppThread---filePath:[%s]", filePath);


     if(RELEASE_MODE == gb_RunMode)
     {
         //sign verify
         unsigned char signBuf[SIGN_DATA_BYTE_LEN+1] = {0};

          iRet = mMFile->getFileSignData(filePath, signBuf);
          qDebug("UpdateSubAppThread---getFileSignData iRet:[%d]", iRet);
          if(0 != iRet)
          {
               refreshPromptAndExit("Get masterapp sign data failed!");
              return ;
          }

          iRet = DLL_PciVerifySignature(0x00, signBuf);
            qDebug("UpdateSubAppThread---DLL_PciVerifySignature iRet:[%d]", iRet);
          if(0 != iRet)
          {
               refreshPromptAndExit("32550 os sign verify failed!");
              return ;
          }
     }


     iRet = DLL_DownlaodApp((unsigned char*)filePath);
     qDebug("UpdateSubAppThread---DownloadApp iRet:[%d]", iRet);
     if(0 != iRet)
     {
          refreshPromptAndExit("Update 32550 os failed!");
         return ;
     }

//    gb_RefreshMainUI = true;
    int schipStat = DLL_PciCheckSecurityChipStatus();
    if ((schipStat > 0) &&(SCHIP_HAVE_APP == (schipStat & SCHIP_HAVE_APP))) {
        DLL_PciJumpToOs();
    }

     refreshPromptAndExit("Update 32550 os success!");

}

