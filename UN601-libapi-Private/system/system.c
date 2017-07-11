#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<pwd.h>
#include "user.h"
#include "Dll_api.h"

//mdata:wCmd,wLen,data.
BYTE g_CurAppNo = 0;





int DLL_ApiInit()
{
	int iRet;
	char buf[ 256 ];
	uid_t userid;
	struct passwd* pwd;
	
	iRet = MsgQueueInit();

	//masterapp:home/root/masterapp
	//ProductionTest: home/root/productiontest
	//SubApp: /home/subapps/subapp%d
	userid = getuid();
	printf("userid is %d\r\n",userid);
	pwd = getpwuid(userid);
	printf("username is %s\r\n",pwd->pw_name);
	printf("userdir is %s\r\n",pwd->pw_dir);

	if(memcmp(pwd->pw_name,"root",4)==0) 
	{
		printf("it's %s\r\n",pwd->pw_name);
		//readCurrAppNo(&g_CurAppNo);
		getcwd(buf, 256);
		printf("buf is :%s\r\n",buf);
		if (strstr(buf,"productiontest")!=NULL) {g_CurAppNo = 0xfe;}
		else g_CurAppNo = 0xff;

	}
	else if(strstr(pwd->pw_name,"subapp")!=NULL)
	{
		printf("it's %s\r\n",pwd->pw_name);
		if(pwd->pw_name[6]>=0x31&&pwd->pw_name[7]>=0x30) g_CurAppNo = (pwd->pw_name[6]-0x30)*10 + pwd->pw_name[7]-0x30;
		else g_CurAppNo = pwd->pw_name[6]-0x30;
	}
	printf("g_CurAppNo is %02x\r\n",g_CurAppNo);
	
	return iRet;
}

int DLL_SysBeep()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = SYSTEM_MODULE;
	g_abySendBuff[1] = SYS_SOUNDBEEP;
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

int DLL_SysBeef(BYTE mode, ushort DlyTimeMs)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 3+1;
	g_abySendBuff[0] = SYSTEM_MODULE;
	g_abySendBuff[1] = SYS_SOUNDBEEF;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;
	
	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = mode;
	g_abySendBuff[6] = DlyTimeMs/256;
	g_abySendBuff[7] = DlyTimeMs%256;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_SysSetDateTime(BYTE *str)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));


	wLen = 6+1;
	g_abySendBuff[0] = SYSTEM_MODULE;
	g_abySendBuff[1] = SYS_SETRTC;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	memcpy(g_abySendBuff+5,str,6);
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	return iRet;
}

int DLL_SysGetDateTime(BYTE *str)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = SYSTEM_MODULE;
	g_abySendBuff[1] = SYS_GETRTC;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	memcpy(str,g_abyRecvBuff+2,recvLen-2);
	return iRet;
}

int DLL_SysReadSecurityChipSN(BYTE *serialNo)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = SYSTEM_MODULE;
	g_abySendBuff[1] = SYS_GETSN;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;
	
	g_abySendBuff[4] = g_CurAppNo;

	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	memcpy(serialNo,g_abyRecvBuff+2,recvLen-2);
	return iRet;
}

int DLL_SysGetVersion(BYTE *version)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = SYSTEM_MODULE;
	g_abySendBuff[1] = SYS_GETVER;
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

int DLL_SysLedControl(BYTE index, BYTE mode)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 2+1;
	g_abySendBuff[0] = SYSTEM_MODULE;
	g_abySendBuff[1] = SYS_CTLLED;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = index;
	g_abySendBuff[6] = mode;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	return iRet;
}


int DLL_SysSoftReset(BYTE mode)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 1+1;
	g_abySendBuff[0] = SYSTEM_MODULE;
	g_abySendBuff[1] = SYS_SOFTRESET;
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
		
	
int DLL_SysWriteSN(BYTE snLen, BYTE *serialNo)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = snLen+1+1;
	g_abySendBuff[0] = SYSTEM_MODULE;
	g_abySendBuff[1] = SYS_WRITESN;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
	g_abySendBuff[5] = snLen;
	memcpy(g_abySendBuff+6,serialNo,snLen);
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+5);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	return iRet;
}
		
int DLL_SysReadSN(BYTE *serialNo)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = SYSTEM_MODULE;
	g_abySendBuff[1] = SYS_READSN;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;
	
	g_abySendBuff[4] = g_CurAppNo;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	memcpy(serialNo,g_abyRecvBuff+2,recvLen-2);
	return iRet;
}
		
		
int DLL_KbInputStart()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = SYSTEM_MODULE;
	g_abySendBuff[1] = KB_INPUT_START;
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
		

int DLL_KbGetKey(unsigned char *keyValue)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = SYSTEM_MODULE;
	g_abySendBuff[1] = KB_GET_KEY;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;
	
	g_abySendBuff[4] = g_CurAppNo;

	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	if(iRet == 0) keyValue[0] = g_abyRecvBuff[2];
	return iRet;

}

int DLL_KbInputStop()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = SYSTEM_MODULE;
	g_abySendBuff[1] = KB_INPUT_END;
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

int DLL_SysReboot()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = SYSTEM_MODULE;
	g_abySendBuff[1] = SYS_REBOOT;
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

