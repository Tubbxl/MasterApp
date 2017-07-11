#include "apppermsfile.h"

#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
 #include<fcntl.h>

AppPermsFile::AppPermsFile()
{
            memset(mAppPermsFilePath, 0, sizeof(mAppPermsFilePath));
           strcpy(mAppPermsFilePath, "/home/root/masterapp/AppPerm.dat");
}

AppPermsFile::~AppPermsFile()
{

}

bool AppPermsFile::authCheck(unsigned char appNo, unsigned char moduleType)
    {
        int iRet = -1;
         unsigned char byAppNums = 0;

         qDebug("authCheck---flow");

         iRet = readAppPermNumsFromFile(&byAppNums);
         qDebug("authCheck---ReadAppPermNumsFromFile iRet: [%d]", iRet);
         qDebug("authCheck---ReadAppPermNumsFromFile byAppNums: [%d]", byAppNums);
        if(0 != iRet)
            return true;

        int iAppNums = byAppNums;
        unsigned char arrAppPerms[iAppNums][APP_PERM_ITEM_LEN] = {0};
        iRet = readAppPermsFromFile(arrAppPerms);
        qDebug("authCheck---readAppPermsFromFile iRet: [%d]", iRet);
        if(0 != iRet)
            return true;

        qDebug("authCheck---ReadAppPermNumsFromFile iAppNums:  [%d]", iAppNums);

        char szAppName[SUB_APP_NAME_LEN+1] = {0};
         iRet = mpUpdateSubApps->getAppName(appNo, szAppName);
         if(0 != iRet)
             return true;

        int i = 0;
        for(i = 0; i < iAppNums; i++)
        {

           if(0 == memcmp(szAppName, arrAppPerms[i], strlen(szAppName)))
                   break;
        }

        qDebug("authCheck---ReadAppPermNumsFromFile i: [%d]",  i);

        if(i >= iAppNums)
            return true;

        unsigned char appPermBuff[4];
        memcpy(appPermBuff, &arrAppPerms[i][APP_PERM_ITEM_FLAG_POS], 4);

//        qDebug("authCheck---appPermBuff: " + ByteUtil.bytearrayToHexString(appPermBuff, 4));

        switch(moduleType)
        {
        case MODULE_TYPE_ICC:
        case MODULE_TYPE_MCR:
        case MODULE_TYPE_PICC:
        case MODULE_TYPE_PRINTER:
        case MODULE_TYPE_SCAN:
        case MODULE_TYPE_LCM:
        case MODULE_TYPE_FINGERPRINT:
        case MODULE_TYPE_PCI:
            if((0x01 << moduleType) == (appPermBuff[0] & (0x01 << moduleType)))
            {
                return false;
            }
            break;
        case MODULE_TYPE_SYSTEM:
        case MODULE_TYPE_EXTERNALSERIAL:
            qDebug("authCheck---MODULE_TYPE_SYSTEM");

            if(0x01 == (appPermBuff[1] & 0x01))
            {
                return false;
            }
            break;
        default:
            return true;
        }

        qDebug("authCheck---false");
        return true;
    }

int AppPermsFile::setAppPermsItem(char* appName, unsigned char * appPerms, unsigned char *appPermsItem)
{
    unsigned char byTempAppPermInfo[APP_PERM_ITEM_LEN] = {0};

    memcpy(byTempAppPermInfo, appName, strlen(appName));

    byTempAppPermInfo[APP_PERM_ITEM_FLAG_POS] = APP_PERM_ITEM_NORMAL_FLAG;

    memcpy(&byTempAppPermInfo[APP_PERM_ITEM_FLAG_POS+1], appPerms, APP_PERM_BYTE_LEN);

    memcpy(appPermsItem, byTempAppPermInfo, APP_PERM_ITEM_LEN);

    return 0;
}

int AppPermsFile::writeAppPermssion2File(unsigned char *byAppPerm)
{
            unsigned char byAllAppPerms[APP_PERM_FILE_LEN] = {0};
            int fd  = -1;
               int iRet = -1;

            if(0 == access(mAppPermsFilePath,F_OK))
            {
                 fd = open(mAppPermsFilePath,O_RDONLY);
                if(fd<0) return -1;
                lseek(fd,0,SEEK_SET);


               iRet = read(fd,byAllAppPerms, sizeof(byAllAppPerms));
               close(fd);
                if(iRet < 0)
                {
                    return -2;
                }

                //app perm file structure:
                  //total nums + use nums + app name + perm byte: 4 bytes, flag: 1 byte(app name+ flag + perm byte is fix len = 37 bytes)...
                  //app perm nums
                  int totalNums = byAllAppPerms[0];
                  int useNums = byAllAppPerms[1];

                  //
                  int index = 2;
                  unsigned char byTempAppPermInfo[APP_PERM_ITEM_LEN] = {0};
                  int i = 0;

                  for(i = 0; i < totalNums; i++)
                  {

                      memcpy(byTempAppPermInfo, &byAllAppPerms[index], APP_PERM_ITEM_LEN);


                      if(0 == memcmp(byTempAppPermInfo, byAppPerm, APP_PERM_APP_NAME_LEN))
                      {
                          memcpy(&byAllAppPerms[index], byAppPerm, APP_PERM_ITEM_LEN);
                          break;
                      }

                      index += APP_PERM_ITEM_LEN;

                      memset(byTempAppPermInfo, 0x00, sizeof(byTempAppPermInfo));
                  }

                  if(useNums < totalNums)
                  {
                      if(i >= totalNums)
                      {
                          index = 2;
                          for(i = 0; i < totalNums; i++)
                          {
                                memcpy(byTempAppPermInfo, &byAllAppPerms[index], APP_PERM_ITEM_LEN);

                              if(APP_PERM_ITEM_DELETE_FLAG == byTempAppPermInfo[APP_PERM_ITEM_FLAG_POS])
                              {
                                  memcpy(&byAllAppPerms[index], byAppPerm, APP_PERM_ITEM_LEN);

                                  break;
                              }

                              index += APP_PERM_ITEM_LEN;
                              memset(byTempAppPermInfo, 0x00, sizeof(byTempAppPermInfo));
                          }

                      }
            }
               else
                {
                    if(i >= totalNums)
                    {
                        // app perm num +1
                        byAllAppPerms[0] += 1;
                        byAllAppPerms[1] += 1;

                         memcpy(&byAllAppPerms[index], byAppPerm, APP_PERM_ITEM_LEN);
                    }
                }

            }
            else
            {
                // app perm num +1
                byAllAppPerms[0] += 1;
                byAllAppPerms[1] += 1;
                 memcpy(&byAllAppPerms[2], byAppPerm, APP_PERM_ITEM_LEN);
            }

            fd = open(mAppPermsFilePath, O_CREAT|O_RDWR,0644);
            if(fd < 0)
                return -3;

            lseek(fd,0,SEEK_SET);

            write(fd,byAllAppPerms, sizeof(byAllAppPerms));
            fsync(fd);
            close(fd);
            return 0;
}

int AppPermsFile::deleteAppPerm2File(unsigned char* byAppPerm)
{
    unsigned char byAllAppPerms[APP_PERM_FILE_LEN] = {0};
    int fd  = -1;
       int iRet = -1;

         fd = open(mAppPermsFilePath,O_RDONLY);
        if(fd<0) return -1;
        lseek(fd,0,SEEK_SET);


       iRet = read(fd,byAllAppPerms, sizeof(byAllAppPerms));
       close(fd);
        if(iRet < 0)
        {
            return -2;
        }

            //app perm file structure:
            //num + app name + perm byte: 4 bytes(pkg name+ perm byte is fix len = 37 bytes)...
            //app perm nums
            //flag 1 normal, 0 delete
            //appperms nums
            int totalNums = byAllAppPerms[0];
   //         int useNums = byAllAppPerms[1];

            //
            int index = 2;
            unsigned char byTempAppPermInfo[APP_PERM_ITEM_LEN] = {0};
            int i = 0;

            for(i = 0; i < totalNums; i++)
            {

                memcpy(byTempAppPermInfo, &byAllAppPerms[index], APP_PERM_ITEM_LEN);

                if(0 == memcmp(byTempAppPermInfo, byAppPerm, APP_PERM_APP_NAME_LEN))
                {
                    byAllAppPerms[index+APP_PERM_APP_NAME_LEN] = APP_PERM_ITEM_DELETE_FLAG;
                    break;
                }

                index += APP_PERM_ITEM_LEN;
                memset(byTempAppPermInfo, 0x00, sizeof(byTempAppPermInfo));
            }

            if(i >= totalNums)
            {
                // don't find app perm info
                return -3;

            }

            fd = open(mAppPermsFilePath, O_RDWR,0644);
            if(fd < 0)
                return -4;

            lseek(fd,0,SEEK_SET);

            write(fd,byAllAppPerms, sizeof(byAllAppPerms));
            fsync(fd);
            close(fd);
            return 0;
}

int AppPermsFile::delAppPermRecord(char *appName)
{
    unsigned char byAppPerm[APP_PERM_ITEM_LEN] = {0};

    memcpy(byAppPerm, appName, strlen(appName));

    return deleteAppPerm2File(byAppPerm);
}

int AppPermsFile::readAppPermNumsFromFile(unsigned char * byAppNums)
    {
    int fd  = -1;
       int iRet = -1;

        fd = open(mAppPermsFilePath,O_RDONLY);
        if(fd<0) return -1;
        lseek(fd,0,SEEK_SET);


       iRet = read(fd, &byAppNums, 1);
       close(fd);
        if(iRet < 0)
        {
            return -2;
        }

        return 0;
    }

int AppPermsFile::readAppPermsFromFile(unsigned char (*arrAppPerms)[APP_PERM_ITEM_LEN] )
{
    unsigned char byAllAppPerms[APP_PERM_FILE_LEN] = {0};

    int fd  = -1;
       int iRet = -1;

        fd = open(mAppPermsFilePath,O_RDONLY);
        if(fd<0) return -1;
        lseek(fd,0,SEEK_SET);


       iRet = read(fd,byAllAppPerms, sizeof(byAllAppPerms));
       close(fd);
        if(iRet < 0)
        {
            return -2;
        }

    //app perm file structure:
    //num + pkg name + perm byte: 4 bytes, flag : 1 byte (pkg name + flag + perm byte is fix len = 260 bytes)...
    //app perm nums
    //flag 1 normal, 0 delete
    //appperms nums
    int totalNums = byAllAppPerms[0];
   // int useNums = byAllAppPerms[1];

    //
    int index = 2;
    unsigned char byTempAppPermInfo[APP_PERM_ITEM_LEN] = {0};
    int i = 0;

    int j = 0;
    for(i = 0; i < totalNums; i++)
    {

          memcpy(byTempAppPermInfo, &byAllAppPerms[index], APP_PERM_ITEM_LEN);

        //delete flag
        if(APP_PERM_ITEM_DELETE_FLAG == byTempAppPermInfo[APP_PERM_ITEM_FLAG_POS])
        {
            continue;
        }

         memcpy(arrAppPerms[j], byTempAppPermInfo, APP_PERM_ITEM_LEN);

        j++;
        index += APP_PERM_ITEM_LEN;
        memset(byTempAppPermInfo, 0x00, sizeof(byTempAppPermInfo));
    }

    return 0;
}

