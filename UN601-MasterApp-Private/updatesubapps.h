#ifndef UPDATESUBAPPS_H
#define UPDATESUBAPPS_H

#include <QString>

#include "mappdefines.h"


class UpdateSubApps
{
public:
    UpdateSubApps();
    ~UpdateSubApps();
    int initSubAppDir(int index);
    int initSubAppsFileDir();

     int  hex2str(unsigned char *in, int inlen, char *out);

    int getAppNo(char *appName);
    int getAppName(int appNo, char* appName);
    void saveAppNo(char *appName, int appNo);
    int delAppNo(int appNo);
    int delAppName(char* appName);

     int readAppNumsFromFile(int *byAppNums);
     int readAppInfosFromFile(unsigned char (*arrAppInfos)[SUB_APP_NAME_LEN+1] );

     int readAppfileVerInfo(char * ptrAppName, unsigned char *ptrVerData);

    unsigned long getFileSize(const char *filePath);
    int getAppPerms(char* filePath, unsigned char * appPerms);

    int loadAppsList(QString *appsList);


};

#endif // UPDATESUBAPPS_H
