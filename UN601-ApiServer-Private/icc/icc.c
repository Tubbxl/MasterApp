#include <stdio.h>
#include<string.h>

#include "user.h"
#include "Dll_api.h"


extern BYTE CurAppNo;

int DLL_IccOpen(BYTE slot, BYTE vccMode, BYTE *ATR)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
		
    	if(authCheck(CurAppNo, MODULE_TYPE_ICC))
    		return App_Permissions_Denial;
	
	if(slot > 0); //todo;

       g_abySendBuff[0] = slot;
       g_abySendBuff[1] = vccMode;
		
	wLen = 2;
	wCmd = MAKEWORD(ICC_MODULE,ICC_OPEN);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	if(0==iRet) memcpy(ATR,g_abyRecvBuff+7,wLen-2);
		
	Crc16CloseCom();
	return iRet;	
}

int DLL_IccClose(BYTE slot)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_ICC))
    		return App_Permissions_Denial;
	
	if(slot > 0) //todo;

       g_abySendBuff[0] = slot;

	wLen = 1;
	wCmd = MAKEWORD(ICC_MODULE,ICC_CLOSE);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	
	Crc16CloseCom();
	return iRet;	
}

int DLL_IccCommand(BYTE slot, APDU_SEND * apduSend,APDU_RECV * apduRecv)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_ICC))
    		return App_Permissions_Denial;
	
	if(slot > 0) ;//todo;

	g_abySendBuff[0] = slot;
	memcpy(g_abySendBuff+1,apduSend->Command,sizeof(apduSend->Command));
	g_abySendBuff[1+sizeof(apduSend->Command)] = (apduSend->Lc)/256;
	g_abySendBuff[1+sizeof(apduSend->Command)+1] = (apduSend->Lc)%256;
	memcpy(g_abySendBuff+1+sizeof(apduSend->Command)+1+1,apduSend->DataIn,apduSend->Lc);
	g_abySendBuff[1+sizeof(apduSend->Command)+1+1+apduSend->Lc] = (apduSend->Le)/256;
	g_abySendBuff[1+sizeof(apduSend->Command)+1+1+apduSend->Lc+1] = (apduSend->Le)%256;
	
	wLen = 1+sizeof(apduSend->Command)+2+apduSend->Lc+2;
	wCmd = MAKEWORD(ICC_MODULE,ICC_COMMAND);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	if(0==iRet)
	{
		apduRecv->LenOut = wLen-4;
		memcpy(apduRecv->DataOut,g_abyRecvBuff+7,apduRecv->LenOut);
		apduRecv->SWA = g_abyRecvBuff[7+apduRecv->LenOut];
		apduRecv->SWB = g_abyRecvBuff[7+apduRecv->LenOut+1];
	}

	Crc16CloseCom();
	return iRet;	
}



int DLL_IccCheck(BYTE slot)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_ICC))
    		return App_Permissions_Denial;
	
	if(slot > 0) //todo;

	g_abySendBuff[0] = slot;
	
	wLen = 1;
	wCmd = MAKEWORD(ICC_MODULE,ICC_CHECK);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	
	Crc16CloseCom();
	return iRet;	
}

int DLL_IccSelectEtu(BYTE slot, BYTE mode)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

    	if(authCheck(CurAppNo, MODULE_TYPE_ICC))
    		return App_Permissions_Denial;
	
	if(slot > 0) //todo;
	
	g_abySendBuff[0] = slot;
	g_abySendBuff[1] = mode;

	wLen = 2;
	wCmd = MAKEWORD(ICC_MODULE,ICC_SELETU);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	
	Crc16CloseCom();
	return iRet;	
}



