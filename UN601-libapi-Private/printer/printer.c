#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <linux/fb.h>    
#include <sys/mman.h>    
#include <sys/ioctl.h> 
#include <pthread.h>

#include "user.h"
#include "Dll_api.h"

extern BYTE g_CurAppNo;

int DLL_PrnInit()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = PRINTER_MODULE;
	g_abySendBuff[1] = PRINTER_INIT;
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

int DLL_PrnSetSpace(BYTE x, BYTE y)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 2+1;
	g_abySendBuff[0] = PRINTER_MODULE;
	g_abySendBuff[1] = PRINTER_SETSPACE;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = x;
	g_abySendBuff[6] = y;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_PrnSetFont(BYTE asciiFontHeight, BYTE extendFontHeight, BYTE zoom)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 3+1;
	g_abySendBuff[0] = PRINTER_MODULE;
	g_abySendBuff[1] = PRINTER_SETFONT;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = asciiFontHeight;
	g_abySendBuff[6] = extendFontHeight;
	g_abySendBuff[7] = zoom;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_PrnGetFont(BYTE *asciiFontHeight, BYTE *extendFontHeight, BYTE *zoom)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = PRINTER_MODULE;
	g_abySendBuff[1] = PRINTER_GETFONT;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	if(0==iRet)
	{
		*asciiFontHeight = g_abyRecvBuff[2];
		*extendFontHeight = g_abyRecvBuff[3];
		*zoom = g_abyRecvBuff[4];
	}
	return iRet;	
}

int DLL_PrnStep(int pixel)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 4+1;
	g_abySendBuff[0] = PRINTER_MODULE;
	g_abySendBuff[1] = PRINTER_STEP;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = (BYTE)ABS(pixel);
	g_abySendBuff[6] = (BYTE)(ABS(pixel)>>8);
	g_abySendBuff[7] = (BYTE)(ABS(pixel)>>16);
	g_abySendBuff[8] = (BYTE)(ABS(pixel)>>24);


	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_PrnStr(const char *str)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = strlen(str)+1;
	g_abySendBuff[0] = PRINTER_MODULE;
	g_abySendBuff[1] = PRINTER_STR;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	memcpy(g_abySendBuff+5,str,strlen(str));

	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_PrnLogo(BYTE *logo)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	if(logo[4] == 0)  wLen = ((logo[0]*256+logo[1]+7)/8)*(logo[2]*256+logo[3])+5+1;
	else wLen = logo[5]*256+logo[6] +5+1;
	
	g_abySendBuff[0] = PRINTER_MODULE;
	g_abySendBuff[1] = PRINTER_LOGO;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	
	g_abySendBuff[4] = g_CurAppNo;
	memcpy(g_abySendBuff+5,logo,wLen);

	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}


int DLL_PrnStart()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = PRINTER_MODULE;
	g_abySendBuff[1] = PRINTER_START;
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

int DLL_PrnSetLeftIndent(int x)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 4+1;
	g_abySendBuff[0] = PRINTER_MODULE;
	g_abySendBuff[1] = PRINTER_SETLEFTINDENT;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = (BYTE)ABS(x);
	g_abySendBuff[6] = (BYTE)(ABS(x)>>8);
	g_abySendBuff[7] = (BYTE)(ABS(x)>>16);
	g_abySendBuff[8] = (BYTE)(ABS(x)>>24);


	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_PrnSetGray(BYTE nLevel)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
	wLen = 1+1;
	g_abySendBuff[0] = PRINTER_MODULE;
	g_abySendBuff[1] = PRINTER_SETGRAY;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = nLevel;


	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}


int DLL_PrnCheckStatus()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = PRINTER_MODULE;
	g_abySendBuff[1] = PRINTER_CHECKSTATUS;
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

int DLL_PrnSetInvert(BYTE fOn)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 1+1;
	g_abySendBuff[0] = PRINTER_MODULE;
	g_abySendBuff[1] = PRINTER_SETINVERT;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = fOn;


	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_PrnGetTemperature(int *tempValue)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = PRINTER_MODULE;
	g_abySendBuff[1] = PRINTER_GETTEMP;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
    	*tempValue = iRet;
	
	return iRet;	
}

int DLL_PrnSetTemperature(int tempValue)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 2+1;
	g_abySendBuff[0] = PRINTER_MODULE;
	g_abySendBuff[1] = PRINTER_SETTEMP;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = tempValue/256;
	g_abySendBuff[6] = tempValue%256;


	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_PrnSetMode(unsigned char mode)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 1+1;
	g_abySendBuff[0] = PRINTER_MODULE;
	g_abySendBuff[1] = PRINTER_SETMODE;
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


