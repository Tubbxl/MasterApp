#include <stdio.h>
#include<string.h>

#include "user.h"
#include "Dll_api.h"


extern BYTE g_CurAppNo;

int DLL_SerialSend(unsigned char *strBuf, unsigned short nByte)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = nByte+2+1;
	g_abySendBuff[0] = SERIAL_MODULE;
	g_abySendBuff[1] = SERIAL_SEND;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = nByte/256;
	g_abySendBuff[6] = nByte%256;
	memcpy(g_abySendBuff+7,strBuf,nByte);
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_SerialRecv(unsigned char *strBuf, unsigned short *p_read_len, unsigned short timeout,unsigned short nbyte)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 4+1;
	g_abySendBuff[0] = SERIAL_MODULE;
	g_abySendBuff[1] = SERIAL_RECV;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = nbyte/256;
	g_abySendBuff[6] = nbyte%256;
	g_abySendBuff[7] = timeout/256;
	g_abySendBuff[8] = timeout%256;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	if(0==iRet)
	{
		*p_read_len = recvLen-2;
		memcpy(strBuf,g_abyRecvBuff+2,*p_read_len);
	}
	
	return iRet;	
}

int DLL_DownlaodApp(unsigned char *strPath)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	printf("strPath1 is %s\r\n",strPath);
	wLen = strlen(strPath)+1;

	g_abySendBuff[0] = DOWNLOAD_MODULE;
	g_abySendBuff[1] = DOWNLOAD_APP;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	memcpy(g_abySendBuff+5,strPath,strlen(strPath));
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0- MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}




