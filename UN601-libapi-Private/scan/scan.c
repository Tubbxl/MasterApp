#include <stdio.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "user.h"
#include "Dll_api.h"

extern BYTE g_CurAppNo;

int DLL_ScanOpen()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
	wLen = 0+1;
	g_abySendBuff[0] = SCAN_MODULE;
	g_abySendBuff[1] = SCAN_OPEN;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_ScanClose()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = SCAN_MODULE;
	g_abySendBuff[1] = SCAN_CLOSE;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;
	
	g_abySendBuff[4] = g_CurAppNo;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_ScanModeSet(BYTE mode,BYTE *prefix, BYTE *suffix)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;
	unsigned short prefixlen=0;
	unsigned short suffixlen=0;
	

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

	if(prefix!=NULL)
		prefixlen = strlen(prefix);
	if(suffix!=NULL)
		suffixlen = strlen(suffix);
	wLen = 4+ prefixlen + suffixlen;
	g_abySendBuff[0] = SCAN_MODULE;
	g_abySendBuff[1] = SCAN_MODESET;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = mode;

	if(prefix!=NULL)
	{
		g_abySendBuff[6] = prefixlen;
		strcpy(&g_abySendBuff[7],prefix);
	}
	else
	{
		g_abySendBuff[6] = 0;
		
	}
	if(suffix!=NULL)
	{
		g_abySendBuff[7+prefixlen] = suffixlen;
		strcpy(&g_abySendBuff[8+prefixlen],suffix);
	}
	else
	{
		g_abySendBuff[7+prefixlen] = 0;
	}
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_ScanTrigger()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
	wLen = 0+1;
	g_abySendBuff[0] = SCAN_MODULE;
	g_abySendBuff[1] = SCAN_TRIGGER;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_ScanRead(char *data)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = SCAN_MODULE;
	g_abySendBuff[1] = SCAN_READ;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;

	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	if(iRet>0) memcpy(data,g_abyRecvBuff+2,recvLen-2);


	
	return iRet;	
}

int DLL_ScanKeySet(BYTE mode)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 1+1;
	g_abySendBuff[0] = SCAN_MODULE;
	g_abySendBuff[1] = SCAN_KEYSET;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;
	
	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = mode;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

