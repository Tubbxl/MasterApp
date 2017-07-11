#include "apppermsfile.h"

#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "user.h"

extern BYTE CurAppNo;


#define mAppPermsFilePath "/home/root/masterapp/AppPerm.dat"

int getAppName(int appNo, char* appName)
{
    unsigned char tempBuf[SUB_APP_NAME_LEN*SUB_APP_MAX_NUMS+1]={0};

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


int authCheck(unsigned char appNo, unsigned char moduleType)
{
	int iRet = -1;
	unsigned char byAppNums[2];
	unsigned char arrAppPerms[24][APP_PERM_ITEM_LEN] = {0};
	char szAppName[SUB_APP_NAME_LEN+1] = {0};
	int i = 0;
	unsigned char appPermBuff[4],tempbuf[33];
        int iAppNums = 0;
		
        printf("authCheck---flow\r\n");
	 printf("appNo is %02x\r\n",appNo);
	 if(appNo==0xff) return 0;
	 if(appNo == 0xfe)
	 {
	 	CurAppNo = 0;
		return 0;
	 }

         iRet = readAppPermNumsFromFile(byAppNums);
         printf("authCheck---ReadAppPermNumsFromFile iRet: [%d]\r\n", iRet);
         printf("authCheck---ReadAppPermNumsFromFile byAppNums: [%02x]\r\n", byAppNums[0]);
         if(0 != iRet)
            return 1;

	iAppNums = byAppNums[0];	
        iRet = readAppPermsFromFile(arrAppPerms);
        printf("authCheck---readAppPermsFromFile iRet: [%d]\r\n", iRet);
        if(0 != iRet)
            return 1;

        printf("authCheck---ReadAppPermNumsFromFile iAppNums:  [%02x\r\n", iAppNums);
		
         iRet = getAppName(appNo, szAppName);
	   printf("getAppName is %d\r\n",iRet);
	   printf("szAppname is %s\r\n",szAppName);
         if(0 != iRet)
             return 1;

	
        for(i = 0; i < iAppNums; i++)
        {
		memcpy(tempbuf,arrAppPerms[i],32);
 	    printf("tempbuf is %s\r\n",tempbuf);
		printf("strlen is %d\r\n",strlen(szAppName));
          if(0 == memcmp(szAppName, arrAppPerms[i], strlen(szAppName)))
                   break;
        }

        printf("authCheck---ReadAppPermNumsFromFile i: [%d]\r\n",  i);

        if(i >= iAppNums)
            return 1;

       memcpy(appPermBuff, &arrAppPerms[i][APP_PERM_ITEM_FLAG_POS+1], 4);

	printf("appPermBuff is %02x %02x %02x %02x\r\n",appPermBuff[0],appPermBuff[1],appPermBuff[2],appPermBuff[3]);
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
                return 0;
            }
            break;
        case MODULE_TYPE_SYSTEM:
        case MODULE_TYPE_EXTERNALSERIAL:
            printf("authCheck---MODULE_TYPE_SYSTEM\r\n");

            if(0x01 == (appPermBuff[1] & 0x01))
            {
                return 0;
            }
            break;
        default:
            return 1;
        }

        return 1;
}

int readAppPermNumsFromFile(unsigned char * byAppNums)
{
    int fd  = -1;
       int iRet = -1;

	printf("mAppPermsFilePath is %s\r\n",mAppPermsFilePath);
        fd = open(mAppPermsFilePath,O_RDONLY);
		printf("fd is %d\r\n",fd);
        if(fd<0) return -1;
        lseek(fd,0,SEEK_SET);

       iRet = read(fd, byAppNums, 1);
	printf("read is %d,byAppNums is %02x\r\n",iRet,byAppNums[0]);
	   
       close(fd);
        if(iRet < 0)
        {
            return -2;
        }

        return 0;
}

int readAppPermsFromFile(unsigned char (*arrAppPerms)[APP_PERM_ITEM_LEN] )
{
	unsigned char byAllAppPerms[APP_PERM_FILE_LEN] = {0};
	int fd  = -1;
	int iRet = -1;
	int index ;
	unsigned char byTempAppPermInfo[APP_PERM_ITEM_LEN] = {0};
	int i = 0;
	int j = 0;
	int totalNums = 0;

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
    totalNums = byAllAppPerms[0];
	index = 2;
	printf("totalNums is %d\r\n",totalNums);
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

