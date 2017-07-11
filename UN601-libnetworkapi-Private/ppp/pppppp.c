#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "DLL_api.h"

#define AT_CMD_SEND_PATH			"/dev/ttyUSB2"
#define PPP_OPTIONS_PATH			"/etc/ppp/peers/ppp-options"

int Read_cmd_ack(unsigned char ComID,unsigned char *strBuf, unsigned short *p_read_len, unsigned short timeout,unsigned short nbyte)
{
  	int  nleft, nread, nRet;
	int cntlen=0;
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
    				return 101;
  			}
  			nleft -= nread;
			cntlen += nread;
  			strBuf += nread;
		//if(memcmp(strBuf,"OK",2)==0||memcmp(strBuf,"ERROR",5)==0) break;
		}
		else if (nRet == 0) 
		{
  			if(tv.tv_sec == 0 && tv.tv_usec == 0)
			{
				*p_read_len = (nbyte-nleft);
		        return 0;
		    }
		}
		else 
		{
		      return -1;
		}
  	}
  	*p_read_len = (nbyte-nleft);
	return 0;
}

int send_command(const unsigned char *cmd, unsigned char *buf)
{

	unsigned short len;
	int ret;
	int fd=-1;
	printf("cmd=[%s] \n",cmd);
  	if((fd = open(AT_CMD_SEND_PATH, O_RDWR | O_NDELAY)) == -1)
		return -1;
        if (write(fd, cmd, strlen(cmd)) < 0)
        {
        		close(fd);
			return -1;
        }
	//tcflush(fd,TCIOFLUSH);	
	ret = Read_cmd_ack(fd, buf, &len, 3000,40);
	tcflush(fd,TCIOFLUSH);
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
int DLL_PppDial( const unsigned char *apn,const unsigned char *username, const unsigned char *password)
{
    	int fp = -1;
	char cmd[256]={0};
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
		sprintf(cmd, "echo OK-AT-OK ATDT#777  >> /etc/ppp/peers/ppp-chat-connect.temp");
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
		sprintf(cmd, "echo OK  \\\\rAT+CGDCONT=1,\\\"IP\\\",\\\"3gnet\\\",,0,0 >> /etc/ppp/peers/ppp-chat-connect.temp");
	      if (system((const char *)cmd) != 0)
	      {
	          return PPP_DIAL_FAILED;
	      }
		memset(cmd,0x00,sizeof(cmd));
		sprintf(cmd, "echo OK-AT-OK ATDT*99# >> /etc/ppp/peers/ppp-chat-connect.temp");
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
          return -PPP_DIAL_FAILED;
      }
   	return PPP_DIAL_SUCCESS;
}

int DLL_PppCheckSim(void)
{
	char readbuf[100]={0};
	int i = 10;
	while(i--)
	{
		send_command("AT+CPIN?\r",readbuf);
		if(strstr(readbuf,"+CME ERROR: ")!=NULL)
		{
			return PPP_NO_SIM;
		}
		else if(strstr(readbuf,"+CPIN: READY")!=NULL)
		{
			return PPP_SIM_INSERT;
		}
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
	if(strstr(readbuf,"OK")!=NULL)
	{
		return 0;
	}
	else 
	{
		return -1;
	}
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
int DLL_PppCall(char *tel)
{
	char readbuf[100]={0};
	char cmd[50]={0};
	int csq = 0;
	//sprintf(cmd,"ATD%s;\r",tel);
	sprintf(cmd,"ATD%s;\r",tel);
	//printf("input=[%s]\n",cmd);
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

int main (int argc,char *argv[])
{
	int ret=-1;
	char buff[50]={0};
	if(argc<2)
	{
		printf("please input number\n");
		return -1;
	}
	ret=DLL_PppCall(argv[1]);
	if(ret==0)
	{
		printf("call successful!\n");
		sleep(20);
		send_command("ATH\r",buff);
	}
	
}

