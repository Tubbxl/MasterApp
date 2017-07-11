#include <stdio.h>
#include<string.h>

#include "user.h"
#include "Dll_api.h"

extern BYTE CurAppNo;

int DLL_McrOpen()
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_MCR))
    		return App_Permissions_Denial;

		
	wLen = 0;
	wCmd = MAKEWORD(MSR_MODULE,MSR_OPEN);
	
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

int DLL_McrClose()
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_MCR))
    		return App_Permissions_Denial;


	wLen = 0;
	wCmd = MAKEWORD(MSR_MODULE,MSR_CLOSE);
	
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

int DLL_McrReset()
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_MCR))
    		return App_Permissions_Denial;


	wLen = 0;
	wCmd = MAKEWORD(MSR_MODULE,MSR_RESET);
	
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

int DLL_McrCheck()
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_MCR))
    		return App_Permissions_Denial;


	wLen = 0;
	wCmd = MAKEWORD(MSR_MODULE,MSR_CHECK);
	
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


int DLL_McrRead(BYTE * track1Len,BYTE *track1, BYTE *track2Len,  BYTE *track2, BYTE *track3Len,  BYTE *track3)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_MCR))
    		return App_Permissions_Denial;

	
	wLen = 0;
	wCmd = MAKEWORD(MSR_MODULE,MSR_READ);
	
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
	iRet = MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	track1Len[0] = g_abyRecvBuff[7];
	track2Len[0] = g_abyRecvBuff[8];
	track3Len[0] = g_abyRecvBuff[9];
	if(g_abyRecvBuff[7]!=0) memcpy(track1,g_abyRecvBuff+10,g_abyRecvBuff[7]);
	if(g_abyRecvBuff[8]!=0) memcpy(track2,g_abyRecvBuff+10+g_abyRecvBuff[7],g_abyRecvBuff[8]);
	if(g_abyRecvBuff[9]!=0) memcpy(track3,g_abyRecvBuff+10+g_abyRecvBuff[7]+g_abyRecvBuff[8],g_abyRecvBuff[9]);
	
	Crc16CloseCom();
	return iRet;	
}



int DLL_McrReadVersion(BYTE *version)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_MCR))
    		return App_Permissions_Denial;

	
	wLen = 0;
	wCmd = MAKEWORD(MSR_MODULE,MSR_READ_VERSION);
	
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
	if(0==iRet) memcpy(version,g_abyRecvBuff+7,wLen-2);
	
	Crc16CloseCom();
	return iRet;	
}



