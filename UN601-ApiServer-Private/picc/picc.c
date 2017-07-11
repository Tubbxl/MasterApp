#include <stdio.h>
#include<string.h>

#include "user.h"
#include "Dll_api.h"


extern BYTE CurAppNo;

int DLL_PiccOpen()
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PICC))
    		return App_Permissions_Denial;

	wLen = 0;
	wCmd = MAKEWORD(PICC_MODULE,PICC_OPEN);
	
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

int DLL_PiccClose()
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PICC))
    		return App_Permissions_Denial;


	wLen = 0;
	wCmd = MAKEWORD(PICC_MODULE,PICC_CLOSE);
	
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

int DLL_PiccCheck(BYTE mode, BYTE *cardType, BYTE *serialNo)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

    	if(authCheck(CurAppNo, MODULE_TYPE_PICC))
    		return App_Permissions_Denial;

	g_abySendBuff[0] = mode;
	
	wLen = 1;
	wCmd = MAKEWORD(PICC_MODULE,PICC_CHECK);
	
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
		memcpy(cardType,g_abyRecvBuff+7,2);
		memcpy(serialNo,g_abyRecvBuff+7+2,10);
	}
	
	Crc16CloseCom();
	return iRet;	
}

int DLL_PiccCommand(APDU_SEND * apduSend,APDU_RECV * apduRecv)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

    	if(authCheck(CurAppNo, MODULE_TYPE_PICC))
    		return App_Permissions_Denial;

	
	memcpy(g_abySendBuff,apduSend->Command,sizeof(apduSend->Command));
	g_abySendBuff[sizeof(apduSend->Command)] = (apduSend->Lc)/256;
	g_abySendBuff[sizeof(apduSend->Command)+1] = (apduSend->Lc)%256;
	memcpy(g_abySendBuff+sizeof(apduSend->Command)+1+1,apduSend->DataIn,apduSend->Lc);
	g_abySendBuff[sizeof(apduSend->Command)+1+1+apduSend->Lc] = (apduSend->Le)/256;
	g_abySendBuff[sizeof(apduSend->Command)+1+1+apduSend->Lc+1] = (apduSend->Le)%256;
	
	wLen = sizeof(apduSend->Command)+2+apduSend->Lc+2;
	wCmd = MAKEWORD(PICC_MODULE,PICC_COMMAND);
	
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

int DLL_PiccRemove()
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PICC))
    		return App_Permissions_Denial;



	wLen = 0;
	wCmd = MAKEWORD(PICC_MODULE,PICC_REMOVE);
	
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

int DLL_PiccHalt()
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PICC))
    		return App_Permissions_Denial;


	wLen = 0;
	wCmd = MAKEWORD(PICC_MODULE,PICC_HALT);
	
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

int DLL_PiccReset()
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PICC))
    		return App_Permissions_Denial;


	wLen = 0;
	wCmd = MAKEWORD(PICC_MODULE,PICC_RESET);
	
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


int DLL_PiccM1Authority(BYTE type, BYTE blkNo, BYTE *pwd, BYTE *serialNo)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PICC))
    		return App_Permissions_Denial;

	g_abySendBuff[0] = type;
	g_abySendBuff[1] = blkNo;
	memcpy(g_abySendBuff+2,pwd,6);
	memcpy(g_abySendBuff+2+6,serialNo,3);

	wLen = 2+6+3;
	wCmd = MAKEWORD(PICC_MODULE,PICC_M1AUTHORITY);
	
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

int DLL_PiccM1ReadBlock(BYTE blkNo, BYTE *blkValue)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

    	if(authCheck(CurAppNo, MODULE_TYPE_PICC))
    		return App_Permissions_Denial;

	g_abySendBuff[0] = blkNo;

	wLen = 0;
	wCmd = MAKEWORD(PICC_MODULE,PICC_M1READBLOCK);
	
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
	if(0==iRet) memcpy(blkValue,g_abyRecvBuff+7,wLen-2);
	
	Crc16CloseCom();
	return iRet;
}

int DLL_PiccM1WriteBlock(BYTE blkNo, BYTE *blkValue)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

    	if(authCheck(CurAppNo, MODULE_TYPE_PICC))
    		return App_Permissions_Denial;

	g_abySendBuff[0] = blkNo;
	memcpy(g_abySendBuff+1,blkValue,16);

	wLen = 1+16;
	wCmd = MAKEWORD(PICC_MODULE,PICC_M1WRITEBLOCK);
	
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

int DLL_PiccM1Operate(BYTE type, BYTE blkNo, BYTE *value, BYTE updateBlkNo)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

    	if(authCheck(CurAppNo, MODULE_TYPE_PICC))
    		return App_Permissions_Denial;

	g_abySendBuff[0] = type;
	g_abySendBuff[1] = blkNo;
	memcpy(g_abySendBuff+2,value,4);
	g_abySendBuff[6] = updateBlkNo;
	
	wLen = 2+4+1;
	wCmd = MAKEWORD(PICC_MODULE,PICC_M1OPERATE);
	
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


