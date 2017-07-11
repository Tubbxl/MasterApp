#include <stdio.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "user.h"
//#include "Dll_api.h"

extern "C" {
 #include "include/dll_api.h"
}

#include "loadapp.h"



#define SYS_RUNAPP 0xB907
#define SYS_DOWNLOADAPP 0xB909

#define SYS_REBOOT 0xD111

LoadApp::LoadApp()
{

}

LoadApp::~LoadApp()
{

}

int LoadApp::Dll_SysRunApp()
{
	int iRet;
	unsigned short wLen,wCmd;
//	int nTimeOut = 0;
	
    //Crc16OpenCom();
	
	iRet = Crc16SendPacket(NULL,0,SYS_RUNAPP);
	if (0 != iRet)
	{
		return 1;
	}
	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, 5000);
	if (0==iRet)
	{
	}

	
	Crc16CloseCom();
	return iRet;
}
int LoadApp::Dll_SysDownloadApp()
{
	int iRet;
	unsigned short wLen,wCmd;
//	int nTimeOut = 0;
	
//	Crc16OpenCom();
	
	iRet = Crc16SendPacket(NULL,0,SYS_DOWNLOADAPP);
	if (0 != iRet)
	{
		return 1;
	}
	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, 5000);
	if (0==iRet)
	{
	}
	//Crc16CloseCom();
	return iRet;
}

//int LoadApp::Dll_SysReboot()
//{
//	int iRet;
////	unsigned short wLen,wCmd;
//	unsigned char data[1000];
////	int nTimeOut = 0;
	
//	Crc16OpenCom();
//	data[0] = 0;
//	iRet = Crc16SendPacket(data,1,SYS_REBOOT);
//	if (0 != iRet)
//	{
//		return 1;
//	}

//	Crc16CloseCom();
//	return iRet;
//}

int LoadApp::HandShake()
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

unsigned long LoadApp::get_file_size(const char *path)
{

    unsigned long filesize = -1;
    FILE *fp;
    fp = fopen(path, "r");
    if(fp == NULL)
        return filesize;
    fseek(fp, 0L, SEEK_END);
    filesize = ftell(fp);
    fclose(fp);
    return filesize;
}


int LoadApp::DownloadFile(char* filePath)
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
	
     char szPrompt[48] = {0};
      int progress  = 0;

    while (1){
        if (dwCurrentLen >= iFileLen)
        {
            break;
        }

       progress = (dwCurrentLen*100)/iFileLen;
       memset(szPrompt, 0x00, sizeof(szPrompt));
        sprintf(szPrompt, "Progress : %d ", progress);
        strcat(szPrompt, "%");
        refreshPromptSignal(QString(QLatin1String(szPrompt)));

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
                //printf("Lib_SendPacket iRet = %d\n",iRet);
                if(iRet != 0){
                    printf("Lib_SendPacket iRet = %d\n",iRet);
                    return 10;
                }
				
                pwPacketetLen[0] = 0;

                iRet = RecvPack(abyCompress, (DWORD *)pwPacketetLen);
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
    iRet = RecvPack(abyCompress, (DWORD *)pwPacketetLen);

    SendPack(0xD2,abyTemp, (short) 0);

    printf("DownLoad Success !\n");

	free(fileBuffer);
	
    return 0;
}

int LoadApp::DownloadApp(char* filePath)
{
    int iRet = -1;

    iRet = DLL_SysSoftReset(0);
    if(0 != iRet)
        return iRet;

    sleep(2);

    iRet = DLL_PciStartDownloadOs();
    if(0 != iRet)
        return iRet;

 //   iRet = Dll_SysDownloadApp();
//    if(0 != iRet)
//        return iRet;

    iRet = HandShake();
    if(0 != iRet)
        return iRet;

    iRet = DownloadFile(filePath);
    return iRet;
	
}
