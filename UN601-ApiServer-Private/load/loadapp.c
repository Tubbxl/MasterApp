#include <stdio.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "user.h"
#include "Dll_api.h"


#define SYS_RUNAPP 0xB907
#define SYS_DOWNLOADAPP 0xB909

#define SYS_REBOOT 0xD111

extern BYTE CurAppNo;
int HandShake()
{
	unsigned char abyRecvBuff[1];
        unsigned char abySendBuff[1];
        int iRet, i;

        printf("connecting...\n");
		
        //Crc16OpenCom();
		
        i = 0;

        for(;i<10;i++)
        {
            abySendBuff[0] = 'T';

            PortSend(abySendBuff[0]);

            iRet = PortRecv(abyRecvBuff, 1000);
			//printf("recv:%d %02x\n",iRet,abyRecvBuff[0]);
            if(iRet==0&&abyRecvBuff[0]=='S'){
                abySendBuff[0] = 'Q';
				
				PortSend(abySendBuff[0]);
           		iRet = PortRecv(abyRecvBuff, 1000);
				
                if(iRet==0&&abyRecvBuff[0]=='P') break;
            }
            else i--;
        }
        if(i!=10) {
            printf("Hand shack success !\n");
            return 0;
        }
        return 1;
}


int DownloadFile(char* filePath)
{

	int byteOffset = 0;
	int dwCurrentLen = 0;
	int iErrorCount = 0;
	int wPacketLen = 0;
	int iReadLen = 0;
	int iFileLen;
	short iSendPacketLen = 0;
	int iRet;
	unsigned char *fileBuffer;
	int fp;
	unsigned char abyTemp[530];
	unsigned char abyCompress[50];

    short pwPacketetLen[1];

	printf("file path is %s\r\n",filePath);
	
    dwCurrentLen = 0;
    iErrorCount = 0;
	
	iFileLen = get_file_size(filePath);
	
    printf("iFileLen = %d\n",iFileLen);
	
	if(iFileLen<10*1024)
	{
		printf("App File error, fileLen:%d\n",iFileLen);
	}
	fileBuffer = (unsigned char *)malloc(iFileLen);
	fp = open(filePath, O_RDONLY);
	if(fp<0)
	{
		free(fileBuffer);
		printf("open app file error\n");
		return -1;
	}
	read(fp, fileBuffer, iFileLen);
	close(fp);
	
	printf("begin download,please wait...\n");
	
    while (1){
        if (dwCurrentLen >= iFileLen)
        {
            break;
        }

        wPacketLen = 512;
        if ((dwCurrentLen+512) > iFileLen){
            wPacketLen = (iFileLen - dwCurrentLen);
        }

        byteOffset += iReadLen;
		
       
		memcpy(abyTemp+5,fileBuffer + byteOffset,wPacketLen);
		
        iReadLen = wPacketLen;

        if (iReadLen > 0)
        {
            abyTemp[0] = 0;
            abyTemp[1] = (BYTE)((iReadLen/65536)%256);
            abyTemp[2] = (BYTE)((iReadLen%65536)/256);
            abyTemp[3] = (BYTE)(iReadLen%256);
            abyTemp[4] = 0;  // data no zip
            iSendPacketLen = (short)(iReadLen+5);
            iErrorCount = 0;

            while(1){
                iRet = SendPack(0xb0,abyTemp, iSendPacketLen);
                if(iRet != 0){
                    printf("Lib_SendPacket iRet = %d\n",iRet);
                    return 10;
                }
				
                pwPacketetLen[0] = 0;

                iRet = RecvPack(abyCompress, pwPacketetLen);
                if ((0==iRet) && (0x0B==abyCompress[5])){
					//printf(".");
                    break;
                }
                iErrorCount++;
                if (iErrorCount > 0){
                    if (-1 == iRet){
                        printf("Receive timeout!");
                    }
                    else{
                        printf("Receive failed!");
                    }
                    return 5;
                }
            }
            dwCurrentLen += iReadLen;
        }
        else{
            break;
        }
    }

    pwPacketetLen[0] = 0;

    SendPack(0xb1, abyTemp, (short)0);
    iRet = RecvPack(abyCompress, pwPacketetLen);

    SendPack(0xD2,abyTemp, (short) 0);

    printf("DownLoad Success !\n");

	free(fileBuffer);
	
    return 0;
}
extern unsigned char GetPin_Flag;

int DownloadApp(char* filePath)
{
    int iRet = -1;

	if(CurAppNo != 0xff) return App_Permissions_Denial;

    iRet = DLL_SysSoftReset(0);
    if(0 != iRet)
        return iRet;

    sleep(2);

    iRet = DLL_PciStartDownloadOs();
    if(0 != iRet)
        return iRet;

	GetPin_Flag = 1;

    iRet = HandShake();
    if(0 != iRet)
    	{
		GetPin_Flag = 0;

       	 return iRet;
    	}
    iRet = DownloadFile(filePath);
		GetPin_Flag = 0;

    return iRet;
	
}
