#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>

#include "user.h"
#include "Dll_api.h"


#define SIG_PATH    "/home/root/signature/SignatureApp"
#define P_WAIT      0   
unsigned int offset = 0;  
int DLL_HandSignInit ()
{
	int    result;
    	//result = spawnl (P_WAIT, SIG_PATH, NULL);
    	result = execl( SIG_PATH,NULL,NULL);
    	if (result == -1)
    	{
        	printf("Error from spawnl");
        	return -1001;
    	}
	else
	return 0;
}


int DLL_HandSignGetData(uchar * data)
{
	unsigned char *databuf[8*1024] = {0};
	
	FILE *fpbmp;
       FILE *fpout;


     fpbmp= fopen("/usr/SigDir/signature.bmp", "rb");

     if (fpbmp == NULL)
     {
 		printf("Open bmp failed!!!\n");
 		return 1001;
     }
     fseek(fpbmp, 0L, SEEK_SET);
     fread(databuf, 1, 7550, fpbmp); 
     memcpy(data,databuf,7550);
     return 0;
}























