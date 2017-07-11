#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include "Dll_api.h"


#define GPRS_PATH                              "/dev/gprs"
#define AT_CMD_SEND_PATH			"/dev/ttyUSB2"
#define PPP_OPTIONS_PATH			"/etc/ppp/peers/ppp-options"

#ifndef _IO
# define  _IO(x,y)	(((x)<<8)|y)
#endif

#define GPRS_NO    		_IO('0xBB',0) 		//power on
#define GPRS_OFF          	_IO('0xBB',1)		//power off
#define GPRS_RST    	   	_IO('0xBB',2) 		//RST
#define GPRS_SLEEP    	_IO('0xBB',3) 		//SLEEP
#define GPRS_WAKEUP    	_IO('0xBB',4) 		//WAKEUP
	
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
		if((*(strBuf-1)=='\r')||(*(strBuf)==0x00)) 
		{
			//break;
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
  	if((fd = open(AT_CMD_SEND_PATH, O_RDWR | O_NDELAY|O_NOCTTY)) == -1)
		return -1;
	//printf("cmd=[%s],len=%d",cmd,strlen(cmd));
	ret=ComInit(fd ,115200);
	if(ret == -1)
	{
		ret=ComInit(fd ,115200);
		if(ret == -1)
		{
			printf("init com failed\n");
			close(fd);
			return -1;
		}
	}
	send_cmd_ack(fd, cmd, strlen(cmd));
	ret = Read_cmd_ack(fd, buf, &len, 50,100);
	//printf("read=[%S]\n",buf);
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

int DLL_PppDial( const unsigned char *apn,const unsigned char *username, const unsigned char *password,const unsigned char *tel)
{
    	int fp = -1;
	char cmd[256]={0};
	printf("%s,%s,%s,%s\n",apn,username,password,tel);
	if(apn!=NULL)
	{
		sprintf(cmd, "cat /etc/ppp/peers/ppp-chat-connect > /etc/ppp/peers/ppp-chat-connect.temp");
	      if (system((const char *)cmd) != 0)
	      {
	          return PPP_DIAL_FAILED;
	      }
		memset(cmd,0x00,sizeof(cmd));
		sprintf(cmd, "echo OK  \\\\rAT+CGDCONT=1,\\\"IP\\\",\\\"%s\\\",,0,0 >> /etc/ppp/peers/ppp-chat-connect.temp",apn);
	      if (system((const char *)cmd) != 0)
	      {
	          return PPP_DIAL_FAILED;
	      }
		  
		memset(cmd,0x00,sizeof(cmd));
		if(tel!=NULL)
			sprintf(cmd, "echo OK-AT-OK ATDT%s  >> /etc/ppp/peers/ppp-chat-connect.temp",tel);
		else
			sprintf(cmd, "echo OK-AT-OK ATDT*99#  >> /etc/ppp/peers/ppp-chat-connect.temp");
	      if (system((const char *)cmd) != 0)
	      {
	          return PPP_DIAL_FAILED;
	      }
		  
		memset(cmd,0x00,sizeof(cmd));
		sprintf(cmd, "echo CONNECT \\\\d\\\\c >> /etc/ppp/peers/ppp-chat-connect.temp");
	      if (system((const char *)cmd) != 0)
	      {
	          return PPP_DIAL_FAILED;
	      }	
	}
	else
	{
		sprintf(cmd, "cat /etc/ppp/peers/ppp-chat-connect > /etc/ppp/peers/ppp-chat-connect.temp");
	      if (system((const char *)cmd) != 0)
	      {
	          return PPP_DIAL_FAILED;
	      }
		memset(cmd,0x00,sizeof(cmd));
		sprintf(cmd, "echo OK  \\\\rAT+CGDCONT=1,\\\"IP\\\",\\\"uninet\\\",,0,0 >> /etc/ppp/peers/ppp-chat-connect.temp");
	      if (system((const char *)cmd) != 0)
	      {
	          return PPP_DIAL_FAILED;
	      }
		memset(cmd,0x00,sizeof(cmd));
		if(tel!=NULL)
			sprintf(cmd, "echo OK-AT-OK ATDT%s  >> /etc/ppp/peers/ppp-chat-connect.temp",tel);
		else
			sprintf(cmd, "echo OK-AT-OK ATDT*99#  >> /etc/ppp/peers/ppp-chat-connect.temp");
	      if (system((const char *)cmd) != 0)
	      {
	          return PPP_DIAL_FAILED;
	      }
		memset(cmd,0x00,sizeof(cmd));
		sprintf(cmd, "echo CONNECT \\\\d\\\\c >> /etc/ppp/peers/ppp-chat-connect.temp");
	      if (system((const char *)cmd) != 0)
	      {
	          return PPP_DIAL_FAILED;
	      }	
	}
	
	if((username!=NULL)&&(password!=NULL))
	{
		sprintf(cmd, "cat /etc/ppp/peers/ppp-options > /etc/ppp/peers/ppp-options.temp");
	      if (system((const char *)cmd) != 0)
	      {
	          return PPP_DIAL_FAILED;
	      }
		memset(cmd,0x00,sizeof(cmd));
		sprintf(cmd, "echo user  %s >> /etc/ppp/peers/ppp-options.temp",username);
	      if (system((const char *)cmd) != 0)
	      {
	          return PPP_DIAL_FAILED;
	      }
		memset(cmd,0x00,sizeof(cmd));
		sprintf(cmd, "echo password  %s >> /etc/ppp/peers/ppp-options.temp",password);
	      if (system((const char *)cmd) != 0)
	      {
	          return PPP_DIAL_FAILED;
	      }
	}
	else
	{
		sprintf(cmd, "cat /etc/ppp/peers/ppp-options > /etc/ppp/peers/ppp-options.temp");
	      if (system((const char *)cmd) != 0)
	      {
	          return PPP_DIAL_FAILED;
	      }
	}
	
	memset(cmd,0x00,sizeof(cmd));
	sprintf(cmd, "pppd call ppp-options.temp &");
      if (system((const char *)cmd) != 0)
      {
          return PPP_DIAL_FAILED;
      }
	memset(cmd,0x00,sizeof(cmd));
	sprintf(cmd, "cat /etc/ppp/resolv.conf >> /etc/resolv.conf");
      if (system((const char *)cmd) != 0)
      {
          return PPP_DIAL_FAILED;
      }
	memset(cmd,0x00,sizeof(cmd));
	//sprintf(cmd, "rm /etc/ppp/peers/ppp-options ");
	//system((const char *)cmd);
		
   	return PPP_DIAL_SUCCESS;
}

int DLL_PppCheckSim(void)
{

	int i = 3;
	int ret=0;
	char *str = NULL;
	char readbuf[100]={0};
	while(i)
	{
		send_command("AT+CPIN?\r",readbuf);
		if(str=strstr(readbuf,"+CPIN: "))
		{
			if(strstr(str, "READY"))
				return PPP_SIM_INSERT;
			else
				return PPP_NO_SIM;
		}
		else if(strstr(readbuf,"OK ")!=NULL)
		{
			return PPP_SIM_INSERT;
		}
		else if(strstr(readbuf,"+CPIN: READY")!=NULL)
		{
			return PPP_SIM_INSERT;
		}
		i--;
	}
	return PPP_CHECK_SIM_FAILED;
}

int DLL_PppCheckCsq(void)
{
	char readbuf[200]={0};
	int csq = 0;
	char *p = NULL;
	send_command("AT+CSQ\r",readbuf);
	
	if(strstr(readbuf,"+CME ERROR: ")!=NULL)
	{
		return PPP_CHECK_CSQ_FAILED;
	}
	else if(strstr(readbuf,"+CSQ: ")!=NULL)
	{
		p=strstr(readbuf,"+CSQ: ");
		if((*(p+6))>0x34)
			csq = (*(p+6)-0x30);
		else
			csq = (*(p+6)-0x30)*10+(*(p+7)-0x30);
		return csq;
	}
	else
	{
		return PPP_CHECK_CSQ_FAILED;
	}
	
}

int DLL_PppReset(void)
{
	char readbuf[100]={0};
	int csq = 0;
	send_command("AT+CFUN=1,1\r",readbuf);
	//send_command("AT+QPOWD\r",readbuf);
	if(strstr(readbuf,"OK")!=NULL)
	{
		return PPP_RESET_SUCCESS;
	}
	else 
	{
		return PPP_RESET_FAILED;
	}
	
}

int DLL_GprsReset(void)
{
	int fd = -1;
	int ret;
	fd = open("/dev/gprs", O_RDWR | O_NDELAY);
	if(fd < 0)
	{
		printf("open error \n");
		return GPRS_RESET_FAILED;		
	}
	ret =  ioctl(fd,GPRS_OFF);	
	if(ret<0)
	{
        	printf("The ioctl error!\n");
            	return GPRS_RESET_FAILED;
	}
	return GPRS_RESET_SUCCESS;
}

int DLL_PppStatus(void)
{
	char buf[1000]={0};
	FILE *fp;
	int ret = -1;
	sprintf(buf,"ifconfig >/tmp/ppp.dat");
	system(buf);
	fp=fopen("/tmp/ppp.dat","r");	
	while(fscanf(fp,"%s",buf)!=EOF)
	{
		
		if(buf[0]==0)continue;
		if(memcmp(buf,"ppp0",4)==0)
		{
			ret = 0;
		}
	}
	fclose(fp);
	sprintf(buf,"rm /tmp/ppp.dat");
	system(buf);
	return ret;
	
}

int DLL_PppSetSlow(char mode)
{
	char readbuf[100]={0};
	char cmd[50]={0};
	int csq = 0;
	if(mode ==1)
		sprintf(cmd,"AT+QSCLK=1\r");
	else
		sprintf(cmd,"AT+QSCLK=0\r");
	send_command(cmd,readbuf);
	if(strstr(readbuf,"OK")!=NULL)
	{
		return PPP_CALL_SUCCESS;
	}
	else
	{
		return -1;
	}
}

int DLL_PppGetIMEI(char *IMEI)
{
	char readbuf[200]={0};
	char tempbuf[20]={0};
	int i=100;
	int j=0;
	memcpy(tempbuf,"AT+GSN\r",7);
	//printf("{%s}\n",tempbuf);
	while(i--)
	{
	
		send_command(tempbuf,readbuf);
		//printf("%s,%d",readbuf,strlen(readbuf));
		if(readbuf[10]!=0x00)
		{
			//printf("readbuf=[%s]\n",readbuf);
			memcpy(IMEI,readbuf+9,15);
			//printf("IMEI=[%s]\n",IMEI);
			return 0;
		}
	}
	return PPP_GET_IMEI_FAILED;
}

int DLL_PppCall(char *tel)
{
	char readbuf[100]={0};
	char cmd[50]={0};
	int csq = 0;
	sprintf(cmd,"ATD%s;\r",tel);
	send_command(cmd,readbuf);
	if(strstr(readbuf,"OK")!=NULL)
	{
		return PPP_CALL_SUCCESS;
	}
	else if(strstr(readbuf,"NO CARRIER")!=NULL)
	{
		return PPP_CALL_FAILED;
	}
	else 
	{
		return PPP_CALL_FAILED;
	}
}

void DLL_Pppdisconnect()
{
	BYTE buf[100];
	BYTE pid[50];
	FILE *fp;
	int flag=0;
	sprintf(buf,"ps  | grep pppd >/tmp/dat.dat");
	system(buf);
	flag=0;
	fp=fopen("/tmp/dat.dat","r");
	while(fscanf(fp,"%s",pid)!=EOF)
	{
		if(pid[0]==0) continue;
		
		if(flag==0) sprintf(buf,"kill %s",pid);

		flag++;
	}
	system(buf);
}

int DLL_PppGetNetPara(unsigned char * IP,unsigned char * Mask,unsigned char * Gateway)
{
	char buf[1000]={0};
	FILE *fp;
	int ret = -1;
	sprintf(buf,"ifconfig ppp0 >/tmp/ppp0.dat");
	system(buf);
	fp=fopen("/tmp/ppp0.dat","r");	
	while(fscanf(fp,"%s",buf)!=EOF)
	{
		if(memcmp(buf,"addr",4)==0)
		{
			memcpy(IP,buf+5,strlen(buf)-5);
		}
		else if(memcmp(buf,"P-t-P:",6)==0)
		{
			memcpy(Mask,buf+6,strlen(buf)-6);
		}
		else if(memcmp(buf,"Mask",4)==0)
		{
			memcpy(Gateway,buf+5,strlen(buf)-5);
		}
		memset(buf,0x00,sizeof(buf));
	}
	fclose(fp);
	sprintf(buf,"rm /tmp/ppp0.dat");
	system(buf);
	return ret;
	
}

#if 0
int main()
{
	int ret=-1;
	int csq=0;
	int i;
	char buf[20]={0};
	printf("star \n");
	ret = DLL_PppGetIMEI(buf);
	printf("%s   %d\n",buf ,ret);
}

#endif
