#include <stdio.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>



#include "user.h"
#include "Dll_api.h"

#include<pthread.h>

#ifndef _IO
# define _IO(x,y)	(((x)<<8)|y)
#endif

#define CMD_IN    		_IO('0xdd',5) 
#define CMD_OUT    		_IO('0xdd',6) 

extern BYTE CurAppNo;

int DLL_SysBeep()
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_SYSTEM))
    		return App_Permissions_Denial;


	wLen = 0;
	wCmd = MAKEWORD(SYSTEM_MODULE,SYS_SOUNDBEEP);
	
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

int DLL_SysBeef(BYTE mode, ushort DlyTimeMs)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_SYSTEM))
    		return App_Permissions_Denial;


	 g_abySendBuff[0] = mode;
        g_abySendBuff[1] = DlyTimeMs/256;
        g_abySendBuff[2] = DlyTimeMs%256;	


	wLen = 3;
	wCmd = MAKEWORD(SYSTEM_MODULE,SYS_SOUNDBEEF);
	
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

int DLL_SysSetDateTime(BYTE *str)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
	if(CurAppNo != 0xff && CurAppNo != 0xfe) return App_Permissions_Denial;

	memcpy(g_abySendBuff,str,6);

	wLen = 6;
	wCmd = MAKEWORD(SYSTEM_MODULE,SYS_SETRTC);
	
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



int DLL_SysGetDateTime(BYTE *str)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_SYSTEM))
    		return App_Permissions_Denial;


	wLen = 0;
	wCmd = MAKEWORD(SYSTEM_MODULE,SYS_GETRTC);
	
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
	if(0 == iRet) memcpy(str,g_abyRecvBuff+7,wLen-2);
	
	Crc16CloseCom();
	return iRet;	
}

int DLL_SysReadSecurityChipSN(BYTE *serialNo)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_SYSTEM))
    		return App_Permissions_Denial;

	wLen = 0;
	wCmd = MAKEWORD(SYSTEM_MODULE,SYS_GETSN);

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
	if(0 == iRet) memcpy(serialNo,g_abyRecvBuff+7,wLen-2);
	
	Crc16CloseCom();
	return iRet;	
}

int DLL_SysGetVersion(BYTE *version)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_SYSTEM))
    		return App_Permissions_Denial;

	wLen = 0;
	wCmd = MAKEWORD(SYSTEM_MODULE,SYS_GETVER);

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

int DLL_SysLedControl(BYTE index, BYTE mode)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_SYSTEM))
    		return App_Permissions_Denial;

	g_abySendBuff[0] = index;
	g_abySendBuff[1] = mode;
	
	wLen = 2;
	wCmd = MAKEWORD(SYSTEM_MODULE,SYS_CTLLED);

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

int DLL_SysReadTamperRecord(BYTE *date, BYTE *record)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
	
	wLen = 0;
	wCmd = MAKEWORD(SYSTEM_MODULE,SYS_GETTAMPERRECORD);

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
		memcpy(date,g_abyRecvBuff+7,6);
		memcpy(date,g_abyRecvBuff+7+6,2);
	}
	
	Crc16CloseCom();
	return iRet;
}

int DLL_SysGetEthMacAddr(BYTE *macAddr)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	//todo
	return iRet;
}


int DLL_SysSetEthMacAddr(BYTE *macAddr)
{
	int iRet=-1;

	//todo
	return iRet;
}
		
int DLL_SysGetIMEI(BYTE *imei)
{
	int iRet=-1;

	//todo
	return iRet;
}

int DLL_SysSoftReset(BYTE mode)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_SYSTEM))
    		return App_Permissions_Denial;


	g_abySendBuff[0] = mode;
	wLen = 1;
	wCmd = MAKEWORD(SYSTEM_MODULE,SYS_SOFTRESET);

	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	//todo

/*
	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	if(0==iRet) 
	{
		memcpy(date,g_abyRecvBuff+7,6);
		memcpy(date,g_abyRecvBuff+7+6,2);
	}*/
	
	Crc16CloseCom();
	iRet = 0;
	return iRet;
}
		
int DLL_SysShutdown()
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0;
	wCmd = MAKEWORD(SYSTEM_MODULE,SYS_SHUTDOWN);

	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	//todo

/*
	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	if(0==iRet) 
	{
		memcpy(date,g_abyRecvBuff+7,6);
		memcpy(date,g_abyRecvBuff+7+6,2);
	}*/
	
	Crc16CloseCom();
	iRet = 0;
	return iRet;
}
		
int DLL_SysStandby()
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0;
	wCmd = MAKEWORD(SYSTEM_MODULE,SYS_SLEEP);

	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	//todo
	//system("echo 0 > /sys/class/backlight/backlight/brightness");  //，休眠之前关闭背光，否则唤醒会闪屏
	system("echo mem > /sys/power/state");
	Crc16CloseCom();
	iRet = 0;
	return iRet;
}
			
int DLL_SysWriteSN(BYTE snLen, BYTE *serialNo)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
	if(CurAppNo != 0xff && CurAppNo != 0xfe) return App_Permissions_Denial;

	g_abySendBuff[0] = snLen;
	memcpy(g_abySendBuff+1,serialNo,snLen);
	
	wLen = snLen+1;
	wCmd = MAKEWORD(SYSTEM_MODULE,SYS_WRITESN);

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
		
int DLL_SysReadSN(BYTE *serialNo)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_SYSTEM))
    		return App_Permissions_Denial;


	wLen = 0;
	wCmd = MAKEWORD(SYSTEM_MODULE,SYS_READSN);

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
	if(0==iRet) memcpy(serialNo,g_abyRecvBuff+7,wLen-2);
	
	Crc16CloseCom();
	return iRet;
}

unsigned char keyvalue = 0xff;
static int threadStatus = 1;
 
void KeyInputthread(void)
{
	 int iRet;
	unsigned char recvData[1];
	int k_fd;  

	printf("KeyInputThread run\r\n");
	while(1)
        {

		if(threadStatus==0) 
		{
		    	threadStatus = 1;
			break;
		}
        	iRet = RecvOneByte(recvData, 100);
	    	printf("iRet is:%d\r\n", iRet);
	    	printf("recvData is: %02x\r\n" , recvData[0]);

        	if(0==iRet)
        	{
        		keyvalue = recvData[0];
        	}
        	else
        	{
        		keyvalue = 0xff;
      		
        	}
        	usleep(200000);
        }
	
}
		
int DLL_KbInputStart()
{
	int iRet=-1;
	int ret;
	unsigned short wLen=0,wCmd=0;
	pthread_t id;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
	if(CurAppNo != 0xff) return App_Permissions_Denial;

	wLen = 0;
	wCmd = MAKEWORD(SYSTEM_MODULE,KB_INPUT_START);

	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}
 //create kb get thread to get the key press value from secure chip	
	ret=pthread_create(&id,NULL,(void *) KeyInputthread,NULL);
 	printf("ret is %d, id is %d\r\n",ret,id);
	if(ret!=0){
		printf ("Create pthread error!\n");
		exit (1);
	}

	iRet = 0;
	return iRet;
}
		

int DLL_KbGetKey(unsigned char *keyValue)
{
	
	if(CurAppNo != 0xff) return App_Permissions_Denial;
	keyValue[0] = keyvalue;
	return 0;
}

int DLL_KbInputStop()
{
	int iRet = -2;
 	unsigned short wLen=0,wCmd=0;
 	if(CurAppNo != 0xff) return App_Permissions_Denial;
  
    	threadStatus = 0;
    
	if(0!=Crc16SendPacket(g_abySendBuff,wLen,wCmd))
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

int DLL_SysReboot()
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_SYSTEM))
    		return App_Permissions_Denial;

	wLen = 0;
	wCmd = MAKEWORD(SYSTEM_MODULE,SYS_REBOOT);

	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	//todo

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

int DLL_OpenBacklight()
{
	char buf[1000]={0};
	FILE *fp;
	int ret = -1;
	int cnt = 0;
	system("echo 7 > /sys/class/backlight/backlight/brightness"); //Open the backlight
	system("cat  /sys/class/backlight/backlight/brightness > /tmp/temp");
	fp=fopen("/tmp/temp","r");	
	while(1)
	{
		fscanf(fp,"%s",buf);
		if(memcmp(buf,"7",1)==0)
		{
			ret = 0;
			break;
		}
		else
		{
			fclose(fp);
			system("echo 7 > /sys/class/backlight/backlight/brightness"); 
			system("cat  /sys/class/backlight/backlight/brightness > /tmp/temp");
			fp=fopen("/tmp/temp","r");	
			
		}
		cnt ++;
		if(cnt > 10)
			return -1;
	}
	fclose(fp);
	system("rm /tmp/temp");
	return ret;
}

void send_cmd_ack(unsigned char ComID, unsigned char *strBuf, unsigned short nByte)
{
  	int  nLeft, nwrite, nRet;
  	fd_set wfds;
  	struct timeval tv;
       int nTimeOut = 3;
    	
  	FD_ZERO(&wfds);
  	FD_SET(ComID, &wfds);
  	tv.tv_sec = nTimeOut;
  	tv.tv_usec = 0;

  	nLeft = nByte;

  	while (nLeft > 0) 
  	{
		nRet = select(ComID+1, NULL, &wfds, NULL, &tv);
		if (nRet > 0)
	    {
  			nwrite = write(ComID, strBuf, nLeft);
			if((*(strBuf-1)=='\r'))
			{	
				return 0;
			}
  			if (nwrite <= 0) 
  			{ 
                		return (nByte - nLeft);
  			}
  			nLeft -= nwrite;
  			strBuf += nwrite;
		}
  	}
  	return (nByte - nLeft);
}

int Read_cmd_ack(unsigned char ComID,unsigned char *strBuf, unsigned short *p_read_len, unsigned short timeout,unsigned short nbyte)
{
  	int  nleft, nread, nRet;
  	fd_set rfds;
  	struct timeval tv;
	  	
  	nleft = nbyte;

 	FD_ZERO(&rfds);
  	FD_SET(ComID, &rfds);
  	tv.tv_sec = timeout/1000;
  	tv.tv_usec = timeout %1000*1000;

    *p_read_len = 0;
  	while(nleft > 0) 
  	{
		nRet = select(ComID+1, &rfds, NULL, NULL, &tv);
		if (nRet > 0)
	    {
  			nread = read(ComID, strBuf, nleft);
  			if (nread <= 0) 
  			{ 
				printf("read error\n");
    			       return 101;
  			}
  			nleft -= nread;
  			strBuf += nread;
		if((*(strBuf-1)=='\r')) 
		{
			break;
		}
		}
		else if (nRet == 0) 
		{
  			if(tv.tv_sec == 0 && tv.tv_usec == 0)
			{
				*p_read_len = (nbyte-nleft);
				if(*p_read_len==0) return 102;
		        return 0;
		    }
		}
		else 
		{
		      return 101;
		}
  	}
  	*p_read_len = (nbyte-nleft);
	return 0;
}

int ComInit(unsigned char ComID,unsigned int Boudrate)
{
  	int status;
  	struct termios   options;

  	tcflush(ComID,TCIOFLUSH);
  	tcgetattr(ComID, &options);
  	  	
  	switch(Boudrate)
  	{
  		case 9600:
  	       options.c_cflag = B9600|CS8|CLOCAL|CREAD;
  	       break;
  	  	case 115200:
  	  	   options.c_cflag = B115200|CS8|CLOCAL|CREAD;
		   break;
		case 230400:
			options.c_cflag = B230400|CS8|CLOCAL|CREAD;
			break;
  	       break;
  	}

	options.c_iflag = IGNPAR;
	options.c_lflag = 0;
	options.c_oflag = 0;
  	options.c_cc[VTIME]=3; 
  	options.c_cc[VMIN] =200;
  	status = tcsetattr(ComID,TCSANOW,&options);
	
  	if(status!=0) 
	{        
    		close(ComID);
            printf("uart init ERR_DEVICE");
    		return -1;     
  	}    

  	tcflush(ComID,TCIOFLUSH); 
  	return 0;
}

int send_command(const unsigned char *cmd, unsigned char *buf)
{

	unsigned short len;
	int ret;
	int fd=-1;
	int i = 0;
	int cnt = 0;
  	if((fd = open("/dev/ttyUSB2", O_RDWR | O_NDELAY|O_NOCTTY)) == -1)
		return -1;
	printf("cmd=[%s],len=%d",cmd,strlen(cmd));
	ret=ComInit(fd ,115200);
	if(ret == -1)
	{
		ret=ComInit(fd ,115200);
		if(ret == -1)
		{
			printf("init com failed\n");
			return -1;
		}
	}
	send_cmd_ack(fd, cmd, strlen(cmd));
	ret = Read_cmd_ack(fd, buf, &len, 500,100);
	if(ret<0)
	{
		close(fd);
		return -1;
	}
	else
	{
		close(fd);
		return len;
	}	
}


void DLL_GprsSetSleepMode(char mode)
{
	char readbuf[100]={0};
	char cmd[50]={0};
	if(mode ==1)
		sprintf(cmd,"AT+QSCLK=1\r");
	else
		sprintf(cmd,"AT+QSCLK=0\r");
	send_command(cmd,readbuf);
	if(strstr(readbuf,"OK")!=NULL)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}


int SetGpioInorOut(char mode)
{
	/*
	int fd = -1;
	int ret;
	printf("SetGpioInorOut \n");
	fd = open("/dev/set_gpio", O_RDWR | O_NDELAY);
	if(fd < 0)
	{
		printf("open error \n");
		return -1;		
	}
	if(mode == 1)
		ret =  ioctl(fd,CMD_OUT);	
	if(mode == 0)
		ret =  ioctl(fd,CMD_IN);
	if(ret<0)
	{
        	printf("The ioctl error!\n");
            	return -1;
	}*/
	
	return 0;
}


int DLL_GetChargingStatus(unsigned char *status)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_SYSTEM))
    		return App_Permissions_Denial;

	wLen = 0;
	wCmd = MAKEWORD(SYSTEM_MODULE,SYS_GETCHARGINGSTATUS);

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
	if(0==iRet) memcpy(status,g_abyRecvBuff+7,wLen-2);
	
	Crc16CloseCom();
	return iRet;
}


int DLL_GetBatteryStatus(unsigned char *status)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_SYSTEM))
    		return App_Permissions_Denial;

	wLen = 0;
	wCmd = MAKEWORD(SYSTEM_MODULE,SYS_GETBATTARYSTATUS);

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
	if(0==iRet) memcpy(status,g_abyRecvBuff+7,wLen-2);
	
	Crc16CloseCom();
	return iRet;
}