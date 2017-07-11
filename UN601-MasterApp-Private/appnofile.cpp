#include "appnofile.h"

#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
 #include<fcntl.h>

AppNoFile::AppNoFile()
{
    memset(mCurrAppNoFilePath, 0, sizeof(mCurrAppNoFilePath));
   strcpy(mCurrAppNoFilePath, "/home/root/masterapp/CurrAppNo.dat");
}

AppNoFile::~AppNoFile()
{

}

 int AppNoFile::writeCurrAppNo(unsigned char byAppNo)
{
    if(0 != access("/home/root/masterapp",F_OK))
    {
         if( system("mkdir /home/root/masterapp") < 0)
         {
            return -1;
         }
    }

    int fd = open(mCurrAppNoFilePath, O_CREAT|O_RDWR,0644);
    if(fd < 0)
        return -2;

    lseek(fd,0,SEEK_SET);

    write(fd,&byAppNo,1);
    fsync(fd);
    close(fd);
    return 0;
}

 int AppNoFile::readCurrAppNo(unsigned char *byAppNo)
{
    if(0 != access(mCurrAppNoFilePath,F_OK))
    {
        return -1;
    }

    int fd = open(mCurrAppNoFilePath,O_RDONLY);
    if(fd<0) return -2;
    lseek(fd,0,SEEK_SET);

    int iRet = -1;
   iRet = read(fd,byAppNo, 1);
   close(fd);
    if(1 != iRet)
    {
        return -3;
    }

    return 0;
}

