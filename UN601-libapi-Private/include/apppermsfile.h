#ifndef APPPERMSFILE_H
#define APPPERMSFILE_H

#include "mappdefines.h"


int readAppPermNumsFromFile(unsigned char * byAppNums);
int readAppPermsFromFile(unsigned char (*arrAppPerms)[APP_PERM_ITEM_LEN] );
int authCheck(unsigned char appNo, unsigned char moduleType);



#define APP_PERM_FILE_LEN  1024*6
#define APP_PERM_ITEM_LEN  260
#define APP_PERM_PKG_NAME_LEN  255
#define APP_PERM_ITEM_FLAG_POS  255
#define APP_PERM_ITEM_DELETE_FLAG  0x00
#define APP_PERM_ITEM_LEN  37
#define SUB_APP_NAME_LEN  32

#endif // APPPERMSFILE_H
