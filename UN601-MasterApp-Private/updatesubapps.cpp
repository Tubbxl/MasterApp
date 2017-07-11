#include "updatesubapps.h"

#include <QString>
#include "mappdefines.h"

#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
 #include<fcntl.h>

UpdateSubApps::UpdateSubApps()
{

}

UpdateSubApps::~UpdateSubApps()
{

}

int UpdateSubApps::initSubAppDir(int index)
{
     int iRet = -1;
     char szCmdBuf[256];

    memset(szCmdBuf, 0x00, sizeof(szCmdBuf));
    sprintf(szCmdBuf, "useradd subapp%d", index);
    iRet = system(szCmdBuf);
    if(iRet < 0)
    {
        return iRet;
    }

    memset(szCmdBuf, 0x00, sizeof(szCmdBuf));
     sprintf(szCmdBuf, "passwd  -d subapp%d", index);
     iRet = system(szCmdBuf);
     if(iRet < 0)
     {
         return iRet;
     }

     memset(szCmdBuf, 0x00, sizeof(szCmdBuf));
      sprintf(szCmdBuf, "mkdir /home/subapps/subapp%d", index);
      iRet = system(szCmdBuf);
      if(iRet < 0)
      {
          return iRet;
      }

      memset(szCmdBuf, 0x00, sizeof(szCmdBuf));
       sprintf(szCmdBuf, "chown subapp%d:subapp%d /home/subapps/subapp%d", index, index, index);
       iRet = system(szCmdBuf);
       qDebug("system1111, iRet:[%d]", iRet);
       if(iRet < 0)
       {
           return iRet;
       }

       memset(szCmdBuf, 0x00, sizeof(szCmdBuf));
        sprintf(szCmdBuf, "chmod 700 /home/subapps/subapp%d", index);
        iRet = system(szCmdBuf);
          qDebug("system2222 iRet:[%d]", iRet);
        return iRet;
}


int  UpdateSubApps::initSubAppsFileDir()
{
     int iRet = -1;

     if(0 == access("/home/subapps/subapp0",F_OK))
     {
         return 0;
     }

     if(0 != access("/home/subapps",F_OK))
     {
          iRet = system("mkdir /home/subapps");
          if(iRet < 0)
          {
              return iRet;
          }
     }

     for(int i = 0; i <  SUB_APP_MAX_NUMS; i++)
     {
         iRet = initSubAppDir(i);
         if(iRet < 0)
         {
             return iRet;
         }
     }

     return iRet;
}

int UpdateSubApps::loadAppsList(QString *appsList)
{
    int iRet = -1;

    unsigned char tempBuf[SUB_APP_NAME_LEN+1]={0};

    int fd = open("/home/root/masterapp/SubAppsInfo.dat",O_RDONLY);
    if(fd<0)
    {
        return iRet;
    }

    lseek(fd,0,SEEK_SET);

    int iCnt = 0;
    char appName[48] = {0};
    while(1)
    {
        memset(tempBuf, 0x00, sizeof(tempBuf));
        if(read(fd,tempBuf,SUB_APP_NAME_LEN+1)!=(SUB_APP_NAME_LEN+1))
        {
            break;
        }

        memset(appName, 0x00, sizeof(appName));
        memcpy(appName, tempBuf, SUB_APP_NAME_LEN);

        appsList[iCnt] = QString(appName);

        iCnt ++;

    }

    close(fd);

   return 0;
}

int UpdateSubApps::getAppNo(char *appName)
{
    unsigned char tempBuf[SUB_APP_NAME_LEN+1]={0};

    int fd = open("/home/root/masterapp/SubAppsInfo.dat",O_RDONLY);
    if(fd<0) return 0;
    lseek(fd,0,SEEK_SET);

    unsigned char appNoUsed[SUB_APP_MAX_NUMS+1]={0};
    int appNo = -1;

    while(1)
    {
        memset(tempBuf, 0x00, sizeof(tempBuf));
        if(read(fd,tempBuf,SUB_APP_NAME_LEN+1)!=SUB_APP_NAME_LEN+1)
        {
            break;
        }

    //     qDebug("tempBuf: [%s]", tempBuf);

        appNo = tempBuf[SUB_APP_NAME_LEN];
        appNoUsed[appNo] = 1;

   //     qDebug("appNo: [%d]", appNo);

        if(memcmp(appName,(char*)tempBuf,SUB_APP_NAME_LEN)==0)
        {
            close(fd);
            return tempBuf[SUB_APP_NAME_LEN];
        }

    }

    close(fd);

    int i = 0;
    for(i = 0; i < SUB_APP_MAX_NUMS; i++)
    {
       if(0 == appNoUsed[i])
          break;
    }

    return i;
}

int UpdateSubApps::getAppName(int appNo, char* appName)
{
    unsigned char tempBuf[SUB_APP_NAME_LEN+1]={0};

    int fd = open("/home/root/masterapp/SubAppsInfo.dat",O_RDONLY);
    if(fd<0) return -1;
    lseek(fd,0,SEEK_SET);

    while(1)
    {
        memset(tempBuf, 0x00, sizeof(tempBuf));
        if(read(fd,tempBuf,SUB_APP_NAME_LEN+1)!=SUB_APP_NAME_LEN+1)
        {
            break;
        }

    //     qDebug("tempBuf: [%s]", tempBuf);
   //     qDebug("appNo: [%d]", appNo);

        if(appNo == tempBuf[SUB_APP_NAME_LEN])
        {
            close(fd);
            memcpy(appName, tempBuf, SUB_APP_NAME_LEN);
            return  0;
        }

    }

    close(fd);
    return -2;
}

void UpdateSubApps::saveAppNo(char *appName, int appNo)
{
    unsigned char tempBuf[SUB_APP_NAME_LEN+1]={0};


    if(0 != access("/home/root/masterapp",F_OK))
    {
         if( system("mkdir /home/root/masterapp") < 0)
         {
            return ;
         }
    }

    int fd = open("/home/root/masterapp/SubAppsInfo.dat", O_CREAT|O_RDWR,0644);

    lseek(fd,0,SEEK_SET);

    while(1)
    {
        memset(tempBuf, 0x00, sizeof(tempBuf));
        if(read(fd,tempBuf,SUB_APP_NAME_LEN+1)!= SUB_APP_NAME_LEN+1)
        {
            break;
        }

        if(memcmp(appName,tempBuf, SUB_APP_NAME_LEN)==0)
        {

            close(fd);
            return;
        }
    }

    memcpy(tempBuf,appName,SUB_APP_NAME_LEN);
    tempBuf[SUB_APP_NAME_LEN]   =   appNo&0xff;
    write(fd,tempBuf,SUB_APP_NAME_LEN+1);
    fsync(fd);
    close(fd);
}

int UpdateSubApps::delAppNo(int appNo)
{
   unsigned char srcBuf[1024]={0};
   unsigned char destBuf[1024] = {0};
   unsigned char tempBuf[SUB_APP_NAME_LEN+1] = {0};


    int fd = open("/home/root/masterapp/SubAppsInfo.dat", O_RDWR,0644);
    if(fd<0)
        return -1;

    lseek(fd,0,SEEK_SET);


    int iret = -1;
    iret = read(fd,	srcBuf, 1024);
    if(iret < 0)
    {
        close(fd);
        return -2;
    }

    int srcLen = iret;

 //   LOGD("DelAppIndex appsinfo.dat: ");

    int i = 0;
    int j = 0;
    for(i = 0; i < srcLen; i +=  (SUB_APP_NAME_LEN+1))
    {
        memset(tempBuf, 0x00, sizeof(tempBuf));
        memcpy(tempBuf, &srcBuf[i], SUB_APP_NAME_LEN+1);

    //	LOGD("DelAppIndex tempBuf i : [%d]", i);

        if(appNo != tempBuf[SUB_APP_NAME_LEN])
        {

    //	    LOGD("DelAppIndex destBuf j: [%d]", j);

            memcpy(&destBuf[j*(SUB_APP_NAME_LEN+1)], tempBuf, SUB_APP_NAME_LEN+1);
            j++;
        }
    }

    int destLen = j*(SUB_APP_NAME_LEN+1);

//	LOGD("DelAppIndex destLen: [%d]", destLen);

    ftruncate(fd, 0);
    lseek(fd,0,SEEK_SET);


//	LOGD("DelAppIndex destBuf : ");


    iret = write(fd,destBuf, destLen);
    fsync(fd);
    close(fd);

    if(iret != destLen)
    {
        return -3;
    }


    return 0;
}

int UpdateSubApps::delAppName(char* appName)
{
    unsigned char srcBuf[1024]={0};
    unsigned char destBuf[1024] = {0};
    unsigned char tempBuf[SUB_APP_NAME_LEN+1] = {0};


     int fd = open("/home/root/masterapp/SubAppsInfo.dat", O_RDWR,0644);
     if(fd<0)
         return -1;

     lseek(fd,0,SEEK_SET);


     int iret = -1;
     iret = read(fd,	srcBuf, 1024);
     if(iret < 0)
     {
         close(fd);
         return -2;
     }

     int srcLen = iret;

  //   LOGD("DelAppIndex appsinfo.dat: ");

     int i = 0;
     int j = 0;
     for(i = 0; i < srcLen; i +=  (SUB_APP_NAME_LEN+1))
     {
         memset(tempBuf, 0x00, sizeof(tempBuf));
         memcpy(tempBuf, &srcBuf[i], SUB_APP_NAME_LEN+1);

     //	LOGD("DelAppIndex tempBuf i : [%d]", i);

             if(0 != memcmp(appName, tempBuf, strlen(appName)))
         {

     //	    LOGD("DelAppIndex destBuf j: [%d]", j);

             memcpy(&destBuf[j*(SUB_APP_NAME_LEN+1)], tempBuf, SUB_APP_NAME_LEN+1);
             j++;
         }
     }

     int destLen = j*(SUB_APP_NAME_LEN+1);

 //	LOGD("DelAppIndex destLen: [%d]", destLen);

     ftruncate(fd, 0);
     lseek(fd,0,SEEK_SET);


 //	LOGD("DelAppIndex destBuf : ");


     iret = write(fd,destBuf, destLen);
     fsync(fd);
     close(fd);

     if(iret != destLen)
     {
         return -3;
     }


     return 0;
}

 int UpdateSubApps::readAppNumsFromFile(int *byAppNums)
{

 int fileSize = -1;
 fileSize = getFileSize("/home/root/masterapp/SubAppsInfo.dat");
 if(fileSize < (SUB_APP_NAME_LEN+1))
     return -1;

 *byAppNums = fileSize/(SUB_APP_NAME_LEN+1);

    return 0;
}

 int  UpdateSubApps::hex2str(unsigned char *in, int inlen, char *out)
 {
     int i;
     char szbuf[1024];
     if(in == NULL || out == NULL)
         return -1;

     memset(szbuf, 0, sizeof(szbuf));

     for (i=0; i<inlen; i++)
         sprintf(szbuf+strlen(szbuf), "%02x", in[i]);

     strcpy((char *)out, szbuf);

     return 0;
 }

int UpdateSubApps::readAppInfosFromFile(unsigned char (*arrAppInfos)[SUB_APP_NAME_LEN+1] )
 {
    unsigned char byAllAppInfos[1024] = {0};

    int fd  = -1;
    int iRet = -1;

     int appNums = 0;
      iRet = readAppNumsFromFile(&appNums);
      if(0 != iRet)
          return -1;

        fd = open("/home/root/masterapp/SubAppsInfo.dat",O_RDONLY);
        if(fd<0) return -2;
        lseek(fd,0,SEEK_SET);


       iRet = read(fd,byAllAppInfos, sizeof(byAllAppInfos));
       close(fd);
        if(iRet < 0)
        {
            return -3;
        }

    //app info file structure:
    //app name + app no: (app name + app no is fix len = 33 bytes)...

    int i = 0;
    int j = 0;
    int index = 0;

//    unsigned char szTemp[1024] = {0};
//    hex2str(byAllAppInfos, appNums*33, szTemp);

//    qDebug("UpdateSubApps->readAppNumsFromFile szTemp: [%s]", szTemp);

     for(i = 0; i < appNums; i++)
     {
         memcpy(arrAppInfos[j], &byAllAppInfos[index], (SUB_APP_NAME_LEN+1));

         j++;
         index += (SUB_APP_NAME_LEN+1);

     }

    return 0;
}

unsigned long UpdateSubApps::getFileSize(const char *filePath)
{
    unsigned long fileSize = -1;
    struct stat statBuff;
    if(stat(filePath, &statBuff) < 0){
        return fileSize;
    }else{
        fileSize = statBuff.st_size;
    }
    return fileSize;

}

int UpdateSubApps::readAppfileVerInfo(char * ptrAppName, unsigned char *ptrVerData)
{
     char szAppName[SUB_APP_NAME_LEN] = {0};
     unsigned char tempBuf[SIGN_DATA_BYTE_LEN+VER_DATA_BYTE_LEN+APP_PERM_BYTE_LEN+1]={0};

    strcpy(szAppName, ptrAppName);

    int appNo = -1;
    appNo = getAppNo(szAppName);
    if(appNo < 0)
        return -1;

     char filePath[256] = {0};

    sprintf(filePath, "/home/subapps/subapp%d/", appNo);
    strcat(filePath, szAppName);

    int fileSize = -1;
    fileSize =  getFileSize(filePath);
    if(fileSize < (SIGN_DATA_BYTE_LEN + VER_DATA_BYTE_LEN+APP_PERM_BYTE_LEN))
        return -2;

    int fd = open(filePath, O_RDONLY);
    if(fd<0) return -3;

    lseek(fd,(fileSize-SIGN_DATA_BYTE_LEN-APP_PERM_BYTE_LEN-VER_DATA_BYTE_LEN),SEEK_SET);


    if((SIGN_DATA_BYTE_LEN+APP_PERM_BYTE_LEN+VER_DATA_BYTE_LEN) != read(fd,tempBuf,(SIGN_DATA_BYTE_LEN+APP_PERM_BYTE_LEN+VER_DATA_BYTE_LEN)))
    {
        close(fd);
        return -4;
    }

    memcpy(ptrVerData, tempBuf,   VER_DATA_BYTE_LEN);

    return 0;
}


int UpdateSubApps::getAppPerms(char* filePath, unsigned char * appPerms)
{
    unsigned char appPermsBuf[APP_PERM_BYTE_LEN+1]={0};

    int fileSize = -1;
    fileSize = getFileSize(filePath);
    if(fileSize < APP_SIGN_DATA_LEN)
        return -1;

    int fd = open(filePath,O_RDONLY);
    if(fd<0) return -2;

    lseek(fd,(fileSize-APP_SIGN_DATA_LEN),SEEK_SET);


    if(APP_PERM_BYTE_LEN != read(fd,appPermsBuf,APP_PERM_BYTE_LEN))
    {
        close(fd);
        return -3;
    }

    memcpy(appPerms, appPermsBuf,   APP_PERM_BYTE_LEN);

    return 0;
}

