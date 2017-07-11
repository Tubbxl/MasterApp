#include "updatesubappthread.h"

#include "mappdefines.h"

extern "C"
{
    #include "include/dll_api.h"
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
 #include<fcntl.h>

UpdateSubAppThread::UpdateSubAppThread(QString strFilePath, QObject *parent) :
    QThread(parent)
{
      mFilePath = strFilePath;

      mUpdateSubApps = new UpdateSubApps();

       mpAppPermsFile = new AppPermsFile();

       mMFile = new MFile();
}

UpdateSubAppThread::~UpdateSubAppThread()
{

}

void UpdateSubAppThread::refreshPromptAndExit(QString strPrompt)
{
     refreshPromptSignal(strPrompt);
    sleep(2);
     closePromptSignal();
}

//void* UpdateSubAppThread::getFilename(unsigned char* pathName)
//{
//    unsigned char* fileName, *pos;
//    int nameLen;
//    nameLen = strlen(pathName);
//    pos = pathName + nameLen;
//    while(*pos != '/' && pos != pathName)
//        pos --;

//    if(pos == pathName)
//    {
//        fileName = pathName+1;
//        return fileName;
//    }

//    nameLen = nameLen-(pos-pathName);
//    fileName = (unsigned char*) malloc(nameLen+1);
//    memcpy(fileName,pos+1,nameLen);
//    return fileName;
//}



void UpdateSubAppThread::run()
{
    int iRet = -1;

     refreshPromptSignal("Update App, please waiting...");

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
    system("sync");
    iRet = system(szShellCmd);
    if(0 != iRet)
    {
         refreshPromptAndExit("Copy app file to temp folder failed!");
        return ;
    }

    char filePath[256] = {0};
    memcpy(filePath, byArrFilePath.data(), strlen(byArrFilePath.data()));
    char* fileName = (char*)mMFile->getFilename(filePath);

    qDebug("UpdateSubAppThread---fileName:[%s]", fileName);

    int appNo = -1;
    char szFileName[SUB_APP_NAME_LEN+1] = {0};
    strcpy(szFileName, fileName);
    appNo = mUpdateSubApps->getAppNo(szFileName);

    if(appNo >= SUB_APP_MAX_NUMS)
    {
        refreshPromptAndExit("The number of app has reached the maximum number allowed!");
       return ;
    }

    //sign verify
    unsigned char signBuf[SIGN_DATA_BYTE_LEN+1] = {0};
     iRet = mMFile->getFileSignData(filePath, signBuf);
     if(0 != iRet)
     {
          refreshPromptAndExit("Get app sign data failed!");
         return ;
     }

     iRet = DLL_PciVerifySignature(0x01, signBuf);
     if(0 != iRet)
     {
          refreshPromptAndExit("App sign verify failed!");
         return ;
     }

    //get app perms
    unsigned char appPerms[APP_PERM_BYTE_LEN+1] = {0};
    iRet = mUpdateSubApps->getAppPerms(filePath, appPerms);
    if(0 != iRet)
    {
        refreshPromptAndExit("Get app perms failed!");
       return ;
    }

     memset(szShellCmd, 0x00, sizeof(szShellCmd));
    sprintf(szShellCmd, "cp -rf /home/root/temp/%s /home/subapps/subapp%d",szFileName, appNo);
    qDebug("UpdateSubAppThread---szShellCmd:[%s]", szShellCmd);
    iRet = system(szShellCmd);
    system("sync");
    system("sync");
    if(0 != iRet)
    {
         refreshPromptAndExit("Copy app file to dest folder failed!");
        return ;
    }

    memset(szShellCmd, 0x00, sizeof(szShellCmd));
   sprintf(szShellCmd, "chmod 700  /home/subapps/subapp%d/",  appNo);
   strcat(szShellCmd, szFileName);
   qDebug("UpdateSubAppThread---szShellCmd:[%s]", szShellCmd);
   iRet = system(szShellCmd);
   system("sync");
   if(0 != iRet)
   {
        refreshPromptAndExit("Modify app file permissions failed!");
       return ;
   }

    memset(szShellCmd, 0x00, sizeof(szShellCmd));
   sprintf(szShellCmd, "chown subapp%d:subapp%d /home/subapps/subapp%d/", appNo, appNo, appNo);
   strcat(szShellCmd, szFileName);
   qDebug("UpdateSubAppThread---szShellCmd:[%s]", szShellCmd);
   system("sync");
   iRet = system(szShellCmd);
   if(0 != iRet)
   {
        refreshPromptAndExit("Modify app file ower failed!");
       return ;
   }



    //save app perms
     unsigned char appPermsItem[APP_PERM_ITEM_LEN+1] = {0};
   mpAppPermsFile->setAppPermsItem(szFileName, appPerms, appPermsItem);
    iRet = mpAppPermsFile->writeAppPermssion2File(appPermsItem);
    if(0 != iRet)
    {
         refreshPromptAndExit("Save app perms failed!");
        return ;
    }

    //save app no
  mUpdateSubApps->saveAppNo(szFileName, appNo);

    memset(szShellCmd, 0x00, sizeof(szShellCmd));
   sprintf(szShellCmd, "rm -rf /home/root/temp/*");
   qDebug("UpdateSubAppThread---szShellCmd:[%s]", szShellCmd);
   iRet = system(szShellCmd);

     refreshPromptAndExit("Update app success!");
}

