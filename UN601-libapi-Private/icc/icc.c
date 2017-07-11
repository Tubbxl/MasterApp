#include <stdio.h>
#include<string.h>

#include "user.h"
#include "Dll_api.h"

extern BYTE g_CurAppNo;

int DLL_IccOpen(BYTE slot, BYTE vccMode, BYTE *ATR)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
	
	wLen = 2+1;
	g_abySendBuff[0] = ICC_MODULE;
	g_abySendBuff[1] = ICC_OPEN;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = slot;
	g_abySendBuff[6] = vccMode;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	memcpy(ATR,g_abyRecvBuff+2,recvLen-2);
	
	return iRet;	
}

int DLL_IccClose(BYTE slot)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 1+1;
	g_abySendBuff[0] = ICC_MODULE;
	g_abySendBuff[1] = ICC_CLOSE;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = slot;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_IccCommand(BYTE slot, APDU_SEND * apduSend,APDU_RECV * apduRecv)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
	wLen = 1+sizeof(apduSend->Command)+2+apduSend->Lc+2+1;
	g_abySendBuff[0] = ICC_MODULE;
	g_abySendBuff[1] = ICC_COMMAND;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = slot;
	memcpy(g_abySendBuff+6,apduSend->Command,sizeof(apduSend->Command));
	g_abySendBuff[6+sizeof(apduSend->Command)] = (apduSend->Lc)/256;
	g_abySendBuff[6+sizeof(apduSend->Command)+1] = (apduSend->Lc)%256;
	memcpy(g_abySendBuff+6+sizeof(apduSend->Command)+1+1,apduSend->DataIn,apduSend->Lc);
	g_abySendBuff[6+sizeof(apduSend->Command)+1+1+apduSend->Lc] = (apduSend->Le)/256;
	g_abySendBuff[6+sizeof(apduSend->Command)+1+1+apduSend->Lc+1] = (apduSend->Le)%256;
	

	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	if(0==iRet)
	{
		apduRecv->LenOut = recvLen-4;
		memcpy(apduRecv->DataOut,g_abyRecvBuff+2,apduRecv->LenOut);
		apduRecv->SWA = g_abyRecvBuff[2+apduRecv->LenOut];
		apduRecv->SWB = g_abyRecvBuff[2+apduRecv->LenOut+1];
	}
	
	return iRet;	
}



int DLL_IccCheck(BYTE slot)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 1+1;
	g_abySendBuff[0] = ICC_MODULE;
	g_abySendBuff[1] = ICC_CHECK;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = slot;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_IccSelectEtu(BYTE slot, BYTE mode)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 2+1;
	g_abySendBuff[0] = ICC_MODULE;
	g_abySendBuff[1] = ICC_SELETU;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = slot;
	g_abySendBuff[6] = mode;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}



