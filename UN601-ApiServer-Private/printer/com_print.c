#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>

#include "user.h"
#include "dll_api.h"


#define CMD_PRN_SETLINE    0xe100
#define CMD_PRN_PUSHFIFO   0xe102
#define CMD_PRN_START      0xe104
#define CMD_PRN_STATUS     0xe106
#define CMD_PRNSETGRAY     0xe108


int Prn_SetLine(int dotline)
{
	int iRet;
	unsigned short wLen;
	int nTimeOut = 0;
	unsigned short cmd;

	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	g_abyRecvBuff[0]=(dotline>>24)&0xff;
	g_abyRecvBuff[1]=(dotline>>16)&0xff;
	g_abyRecvBuff[2]=(dotline>>8)&0xff;
	g_abyRecvBuff[3]=(dotline>>0)&0xff;
	Crc16OpenCom();
	
	iRet = Crc16SendPacket(g_abyRecvBuff, 4, CMD_PRN_SETLINE);

	if (0 != iRet)
	{
		return 1;
	}
	for (;;)
	{
		if (nTimeOut >= 1)
		{
			break;
		}
		iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen,&cmd, 500);
		if ((0==iRet) && (13==wLen))
		{
			return 0;
		}
		else
		{
			nTimeOut++;
		}
	}
	return g_abyRecvBuff[5];

}

int Prn_PushFifo(int index,unsigned char *data,unsigned short len)
{
	int iRet;
	unsigned short wLen;
	int nTimeOut = 0;
	unsigned short cmd;

	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

	memcpy(g_abyRecvBuff+3,data,len);
	
	iRet = Crc16SendPacket(g_abyRecvBuff, 3+len, CMD_PRN_PUSHFIFO);

	if (0 != iRet)
	{
		return 1;
	}
	for (;;)
	{
		if (nTimeOut >= 1)
		{
			break;
		}
		iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen,&cmd, 5000);
		if (0==iRet)
		{
			if(g_abyRecvBuff[2]==0x0C)
			{
				if(g_abyRecvBuff[6]==0x01)//Ã»Ö½
					return 11;
				if(g_abyRecvBuff[6]==0x02)//¹ýÈÈ
					return 12;
			}

			return 0;
		}
		else
		{
			nTimeOut++;
		}
	}
	return g_abyRecvBuff[5];
}
int firstSend_flag;

int Prn_Start()
{
	int iRet;
	unsigned short wLen;
	int nTimeOut = 0;
	unsigned short cmd;

	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

	
	iRet = Crc16SendPacket(g_abyRecvBuff, 0, CMD_PRN_START);
	firstSend_flag = 1;

	if (0 != iRet)
	{
		return 1;
	}
	for (;;)
	{
		if (nTimeOut >= 1)
		{
			break;
		}
		iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen,&cmd, 500);
		if ((0==iRet) && (13==wLen))
		{
			return 0;
		}
		else
		{
			nTimeOut++;
		}
	}
	return g_abyRecvBuff[5];

}
int Prn_Status()
{
}
int Prn_SetGray(unsigned char gray)
{
	return 0;
}


int wait_prn_finish()
{
	int iRet;
	unsigned short wLen;
	unsigned short cmd;
	for (;;)
	{
		iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen,&cmd, 5000);

		Crc16CloseCom();
		return g_abyRecvBuff[6];

	}
	return g_abyRecvBuff[5];

}


