#ifndef APPPERMSFILE_H
#define APPPERMSFILE_H

#include "mappdefines.h"

#include "updatesubapps.h"


class AppPermsFile
{
public:
    AppPermsFile();
    ~AppPermsFile();
    int setAppPermsItem(char* appName, unsigned char * appPerms, unsigned char *appPermsItem);
    int writeAppPermssion2File(unsigned char *byAppPerm);
    int deleteAppPerm2File(unsigned char* byAppPerm);
    int delAppPermRecord(char *appName);
    int readAppPermNumsFromFile(unsigned char * byAppNums);
    int readAppPermsFromFile(unsigned char (*arrAppPerms)[APP_PERM_ITEM_LEN] );
    bool authCheck(unsigned char appNo, unsigned char moduleType);

private:
      char mAppPermsFilePath[256];

      UpdateSubApps *mpUpdateSubApps;
};

#endif // APPPERMSFILE_H
