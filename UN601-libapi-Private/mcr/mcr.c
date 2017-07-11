#include <stdio.h>
#include<string.h>

#include "user.h"
#include "Dll_api.h"

extern BYTE g_CurAppNo;

int DLL_McrOpen()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));


	wLen = 0+1;
	g_abySendBuff[0] = MSR_MODULE;
	g_abySendBuff[1] = MSR_OPEN;
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

int DLL_McrClose()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));


	wLen = 0+1;
	g_abySendBuff[0] = MSR_MODULE;
	g_abySendBuff[1] = MSR_CLOSE;
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

int DLL_McrReset()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = MSR_MODULE;
	g_abySendBuff[1] = MSR_RESET;
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

int DLL_McrCheck()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = MSR_MODULE;
	g_abySendBuff[1] = MSR_CHECK;
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


int DLL_McrRead(BYTE * track1Len,BYTE *track1, BYTE *track2Len,  BYTE *track2, BYTE *track3Len,  BYTE *track3)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
	wLen = 0+1;
	g_abySendBuff[0] = MSR_MODULE;
	g_abySendBuff[1] = MSR_READ;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;
	
	g_abySendBuff[4] = g_CurAppNo;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	track1Len[0] = g_abyRecvBuff[2];
	track2Len[0] = g_abyRecvBuff[3];
	track3Len[0] = g_abyRecvBuff[4];
	if(g_abyRecvBuff[2]!=0) memcpy(track1,g_abyRecvBuff+5,g_abyRecvBuff[2]);
	if(g_abyRecvBuff[3]!=0) memcpy(track2,g_abyRecvBuff+5+g_abyRecvBuff[2],g_abyRecvBuff[3]);
	if(g_abyRecvBuff[4]!=0) memcpy(track3,g_abyRecvBuff+5+g_abyRecvBuff[2]+g_abyRecvBuff[3],g_abyRecvBuff[4]);
	return iRet;	
}



int DLL_McrReadVersion(BYTE *version)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = MSR_MODULE;
	g_abySendBuff[1] = MSR_READ_VERSION;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;
	
	g_abySendBuff[4] = g_CurAppNo;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	memcpy(version,g_abyRecvBuff+2,recvLen-2);
	
	return iRet;	
}



