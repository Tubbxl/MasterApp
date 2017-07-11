#ifndef APPPERMSFILE_H
#define APPPERMSFILE_H

#include "mappdefines.h"


int readAppPermNumsFromFile(unsigned char * byAppNums);
int readAppPermsFromFile(unsigned char (*arrAppPerms)[APP_PERM_ITEM_LEN] );
int authCheck(unsigned char appNo, unsigned char moduleType);




#endif // APPPERMSFILE_H
