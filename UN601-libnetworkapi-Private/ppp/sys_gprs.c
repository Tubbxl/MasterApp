//标准库头文件
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>
#include <termios.h>


//#include <arpa/inet.h>      
//#include <fcntl.h>      
#include <linux/fb.h>    
//#include <sys/mman.h>    
#include <sys/ioctl.h> 
#include "user_sys.h"
#include "sys_com.h"

#include "sys_gprs.h"

static int gprs_flag = 0;
static int gprs_fd, g_iGsmComFlag;
static int giGprsBusy;

#ifdef DEBUG_INFO
#define debug_log(fmt,args...) printf("%s(%d):"fmt , __FUNCTION__, __LINE__, ##args)
#define 		PRINTDEBUG		printf("in %s(%d)***\n", __FUNCTION__, __LINE__)
#else
#define debug_log(fmt,args...)
#define 		PRINTDEBUG
#endif

#define GSMMUXD

int sys_gprs_open(void)
{
	unsigned int val = 0;
	if (gprs_flag != DEVICE_OPEN)
	{
	        gprs_fd = open("/dev/gprs", O_RDWR);
	        if(gprs_fd < 0){ 
	                perror("gsm open");
	                return -1; 
	        } 
		gprs_flag = DEVICE_OPEN;	
		ioctl(gprs_fd, IOCTL_SELECT, &val);
	}
	
	return 0;

}

int sys_modem_open(void)
{
	unsigned int val = 1;
	if (gprs_flag != DEVICE_OPEN)
	{
	        gprs_fd = open("/dev/gprs", O_RDWR);
	        if(gprs_fd < 0){ 
	                perror("gsm open");
	                return -1; 
	        } 
		gprs_flag = DEVICE_OPEN;	
		ioctl(gprs_fd, IOCTL_SELECT, &val);
	}
	
	return 0;

}



int wls_open(void)
{
	unsigned int val = 0;
	if (gprs_flag != DEVICE_OPEN)
	{
	        gprs_fd = open("/dev/gprs", O_RDWR);
	        if(gprs_fd < 0){ 
	                perror("gsm open");
	                return -1; 
	        } 
		gprs_flag = DEVICE_OPEN;	
		ioctl(gprs_fd, IOCTL_SELECT, &val);
	}
	
	return 0;
}


void wls_close(void)
{
	if (gprs_flag == DEVICE_OPEN) 
	{
		close(gprs_fd);
		gprs_flag = -1;
	}
}

//0--GPRS   1--modem
void wls_enable(int val)
{
	unsigned int val2 = val;
	wls_open();
	ioctl(gprs_fd, IOCTL_SELECT, &val2);
	wls_close();

}

void sys_gprs_close(void)
{
	if (gprs_flag == DEVICE_OPEN) 
	{
		close(gprs_fd);
		gprs_flag = -1;
	}
	debug_log("app: gsm close\n");
}
void Lib_GetGsmVersion(char *str)
{
	if (gprs_flag == DEVICE_OPEN) 
	{
		ioctl(gprs_fd, CHECK_VERSION, str);
	}
	else
	{
		gprs_fd = open("/dev/gprs", O_RDWR);
		if(gprs_fd < 0){ 
			perror("gsm open");
			return; 
		} 
		ioctl(gprs_fd, CHECK_VERSION, str);
		close(gprs_fd);
		gprs_flag = 0;
	}
}

void gprs_power_on(void)
{
	if (gprs_flag == DEVICE_OPEN) 
		ioctl(gprs_fd, GPRS_ON);
	else
		printf("gprs no open\n");
}
void gprs_power_off(void)
{
	if (gprs_flag == DEVICE_OPEN)
		ioctl(gprs_fd, GPRS_OFF);
}

void gprs_reset(void)
{
	if (gprs_flag == DEVICE_OPEN) 
		ioctl(gprs_fd, IOCTL_GPRS_RESET);
	else
		printf("gprs no open\n");	
}

void cdma_power_on(void)
{
	if (gprs_flag == DEVICE_OPEN) 
		ioctl(gprs_fd, CDMA_ON);
}
void cdma_power_off(void)
{
	if (gprs_flag == DEVICE_OPEN) 
		ioctl(gprs_fd, CDMA_OFF);
}
void cdma_reset(void)
{
	if (gprs_flag == DEVICE_OPEN)
	{
		ioctl(gprs_fd, CDMA_RESET);
	}
}
int modem_reset(void)
{
	if (gprs_flag != DEVICE_OPEN)
	{
		 gprs_fd = open("/dev/gprs", O_RDWR);
	        if(gprs_fd < 0){ 
	                perror("gsm open");
	                return -1; 
	        } 
		gprs_flag = DEVICE_OPEN;	
		
	}
	ioctl(gprs_fd, MODEM_RESET);
	return 0;
}
void Mdm_HWReset(void)
{
	modem_reset();
}

int gsm_send_command(uchar COM, uchar *cmd, uchar *buf)
{
#if 0
//	uchar buf[100];
	unsigned short len;
	char *p;
	
	debug_log("send cmd : %s\n", cmd);
	Lib_UartSend(COM, cmd, strlen(cmd));
//	Lib_UartRecv(COM, buf, &len, 1, 30);
	Lib_UartRecv(COM, buf, &len, 1000, 100);

	buf[len] = 0;

	p = strstr(buf, "OK");
	debug_log("rev %s\n", buf);	
	if(p)
	{
//		debug_log("SIM card OK\n");
		return 0;
	}
	else
	{
		debug_log("no\n");
		return -1;
	}	

#else
//	uchar buf[100];
	unsigned short len;
	char *p;
	int i, j;
	int ret;
	if(Lib_UartStatus(COM) == 0)
		return -1;
	
	Lib_UartClearBuffer(COM);
	debug_log("send cmd : %s\n", cmd);
	Lib_UartSend(COM, cmd, strlen(cmd));
#if 0
	Lib_UartRecv(COM, buf, &len, 1000, 100);
	buf[len] = 0;
#else
	memset(buf, 0, 300);
	i = 0;
	j = 0;
	ret = Lib_UartRecv(COM, &buf[i], &len, 3000, 1);
	while(ret == 0)
	{
		i++;
		if(i > 299)
		{
			i =0;
			j++;
		}
		ret = Lib_UartRecv(COM, &buf[i], &len, 300, 1);
	}
#endif

	p = strstr(buf, "OK");
	debug_log("len:<%d>rev %s\n", i+j*300, buf);	
	if(p)
	{
		return 0;
	}
	else
	{
		return -1;
	}	
#endif
}

int gprs_send_command(uchar *cmd, uchar *buf)
{
//	uchar buf[100];
	unsigned short len;
	char *p;
	
	debug_log("send cmd : %s\n", cmd);
	Lib_UartSend(GPRS_COM, cmd, strlen(cmd));
//	Lib_UartRecv(GPRS_COM, buf, &len, 1, 30);
	Lib_UartRecv(GPRS_COM, buf, &len, 1000, 100);

	buf[len] = 0;

	p = strstr(buf, "OK");
	debug_log("rev %s\n", buf);	
	if(p)
	{
//		debug_log("SIM card OK\n");
		return 0;
	}
	else
	{
		debug_log("no\n");
		return -1;
	}	
}
int gprs_MuxReset(void)
{
	int signal, ret, len;
	int i;
	uchar buf[300];
	printf("GPRS RESET...\n");
	wls_open();
	
	Com_Close(COMMUX0);
	system("/etc/ppp/ppp-off");
	system("killall gsmMuxd");
	gprs_reset();
	sleep(1);
	system("gsmMuxd -p /dev/ttyS2 -b 115200 -s /dev/mux -w /dev/ptmx /dev/ptmx");
	sleep(2);
	ret = Com_Open(COMMUX0, "115200,8,n,1");
	if(ret)
	{
		printf("no mux\n");
		return -1;
	}
	gsm_send_command(COMMUX0, "AT\r", buf);
	gsm_send_command(COMMUX0, "ATE0\r", buf);
	wls_close();
	return 0;
}
int Wls_NetworkStatus(void)
{
	int signal, ret, len;	
	uchar buf[300], revBuf[128];
	char *str;	
	if(Lib_UartStatus(COMMUX0) == 0)
		return -1;
//	gsm_send_command(COMMUX0, "AT\r", buf);	
	gsm_send_command(COMMUX0, "AT+CREG?\r", buf);
	if(str = strstr(buf, "+CREG:"))
	{
		memset(revBuf, 0, 128);
		sscanf(str, "%*s%s", revBuf);
		if(revBuf[2] == '1')
			return 0;
		else
			return -2;
	}
	return -2;
}
int Wls_NetworkAttached(void)
{
	int signal, ret, len;	
	uchar buf[300], revBuf[128];
	char *str;	
	if(Lib_UartStatus(COMMUX0) == 0)
		return -1;
	gsm_send_command(COMMUX0, "AT+CGATT?\r", buf);
	if(str = strstr(buf, "+CGATT:"))
	{
		memset(revBuf, 0, 128);
		sscanf(str, "%*s%s", revBuf);
		if(revBuf[0] == '1')
			return 0;
		else
			return -2;
	}
	return -2;
}
void Wls_RFRestart(void)
{
	int ret;
	uchar buf[300];
		
	if(Lib_UartStatus(COMMUX0) == 0)
		return -1;
	
	ret = gsm_send_command(COMMUX0, "AT+CFUN=0\r", buf);
	if(ret == -1)
		Lib_sdelay(3);
	Lib_sdelay(1);
	gsm_send_command(COMMUX0, "AT+CFUN=1\r", buf);	
	sleep(8);
}


#define	WLS_NOSIM			-5501
#define	WLS_NONET			-5502
#define	WLS_NOSINGLE		-5503
#define	WLS_DIALFAILED		-5504
#define	WLS_NOATTACH		-5505

int Wls_Init(const unsigned char *apn,const unsigned char *username,const unsigned char *password)
{
	uchar buf[300];
	int dialFlag = 0, signal;
	int ret = 0, i, j;
	int csq, net_flag;
	static int frist_flag = 0;	
	giGprsBusy = 1;

	ret = Lib_UartInit(COMMUX0,EM_232_115200BPS, 0, 1);
	if(ret)
	{
		sleep(1);
		ret = Lib_UartInit(COMMUX0,EM_232_115200BPS, 0, 1);
	}

	if(ret)
	{
		ret = gprs_MuxReset();		
		if(ret < 0)
			goto wlsInitOut;	
	}
wlsRedail:	
	gsm_send_command(COMMUX0, "AT\r", buf);
	if(frist_flag == 0)
	{
		frist_flag = 1;
		gsm_send_command(COMMUX0, "ATE0\r", buf);
	}
	if(Wls_CheckSim() < 0)
	{
		ret = WLS_NOSIM;
		goto wlsInitErr;
	}
	if(Wls_NetworkStatus() < 0)
	{
		ret = WLS_NONET;
		goto wlsInitErr;
	}
	if(Wls_NetworkAttached()<0)
	{
		ret = WLS_NOATTACH;
		goto wlsInitErr;
	}
	if(Wls_CheckSignal(&signal) < 10)
	{
		ret = WLS_NOSINGLE;
		goto wlsInitErr;
	}		

	if(system("ifconfig | grep ppp0") == 0)
	{
		printf("gprs is connected\n");
		ret = 0;
		goto wlsInitOut;
	}
	printf("***ppp-on ...\n");
	if(apn == NULL)
	{
		memset(buf, 0, 300);
		strcpy(buf, "cmnet");
		sys_ppp_on(buf, username, password);		
	}
	else
		sys_ppp_on(apn, username, password);
	i = 0;
	while(1)
	{
		if(system("ifconfig | grep ppp0") == 0)
		{
			ret = 0;
			debug_log("ppp ok....\n");
			break;
		}		
		if(i++ > 8)
		{
			ret = WLS_DIALFAILED;		
			goto wlsInitErr;
		}
		sleep(1);
	}	
//	printf("time i = %d\n", i);
wlsInitOut:

	giGprsBusy = 0;	
	return ret;
wlsInitErr:
	system("/etc/ppp/ppp-off");
	sleep(1);
	return ret;
	
}

#if 0
int Wls_Init(uchar *pin)
{
	uchar buf[300], buf2[5], addr[20];
	unsigned short len;
	int param;
	int nCmd;
	int ret = 0, i, j;
	int csq, net_flag;
	static int frist_flag = 0;
//	s_TimerCtl(0);

	giGprsBusy = 1;
#ifdef GSMMUXD
	if(frist_flag == 0)
	{
		ret = Lib_UartInit(COMMUX0,EM_232_115200BPS, 0, 1);
		if(ret)
		{
			sleep(1);
			ret = Lib_UartInit(COMMUX0,EM_232_115200BPS, 0, 1);
		}
		frist_flag = 1;
	}
#else
	ret = Lib_UartInit(GPRS_COM,EM_232_115200BPS, 0, 1);
#endif /* GSMMUXD */
	if(ret)
	{
		ret = gprs_MuxReset();		
		if(ret < 0)
			goto wlsInitOut;
		gsm_send_command(COMMUX0, "AT\r", buf);
	}
	if(system("ifconfig | grep ppp0") == 0)
	{
		printf("gprs is connected\n");
		ret = 0;
		goto wlsInitOut;
	}
#if 0
	system("sh /etc/ppp/gprs_com_unlock");
	sleep(1);
#endif
	
#ifdef GSMMUXD
	ret = gsm_send_command(COMMUX0, "AT\r", buf);
	if(ret)
		gsm_send_command(COMMUX0, "AT\r", buf);
	gsm_send_command(COMMUX0, "ATE0\r", buf);
	gsm_send_command(COMMUX0, "AT+IPR=115200\r", buf);
//	gsm_send_command(COMMUX0, "AT+GSV\r", buf);
	
//	gsm_send_command(COMMUX0, "AT+CENG=2\r", buf);
//	gsm_send_command(COMMUX0, "AT+CENG?\r", buf);
	ret = Wls_CheckSim();
	if(ret)
	{
		printf("no sim card!!!\n");
		system("killall redial.sh");
		ret = -2;
		goto wlsInitOut;
	}	


	if(system("ifconfig | grep ppp0") != 0)
	{
		debug_log("ppp on....\n");
		if(access("/etc/ppp/peers/gsmppp", 0) == 0)
		{
			printf("The PPP began to dial\n");
			printf("pppd call gsmppp\n");
			system("pppd call gsmppp");
		}
		else
		{
			printf("The PPP began to dial\n");
			printf("ppp-on\n");
			system("/etc/ppp/ppp-on");
		}
	}
#else
	ret = gprs_send_command("AT\r", buf);
	if(ret)
	{
		debug_log("gprs init...\n");
		gprs_power_off();
		sleep(1);
		gprs_power_on(); 
		sleep(5);
	}

	gprs_send_command("AT\r", buf);
	gprs_send_command("AT+IPR=115200\r", buf);
	ret = Wls_CheckSim();
	if(ret)
	{
		printf("no sim card!!!\n");
		ret = -2;
		goto wlsInitOut;
	}

	if(system("ifconfig | grep ppp0") != 0)
	{
		debug_log("ppp on....\n");
		if(access("/etc/ppp/peers/gsmppp", 0) == 0)
			system("pppd call gsmppp");
		else
			system("/etc/ppp/ppp-on");
	}	
#endif /* GSMMUXD */

		
	i = 0;
	while(1)
	{
		if(system("ifconfig | grep ppp0") == 0)
		{
			ret = 0;
			debug_log("ppp ok....\n");
			break;
		}
		
		if(i++ > 15)
		{
			ret = -1;
			debug_log("ppp failed....\n");
			break;
		}
		sleep(1);
	}
	gsm_send_command(COMMUX0, "AT+CREG=2\r", buf);

wlsInitOut:
	giGprsBusy = 0;

	return ret;

}
#endif

int Wls_CheckSignal(int * pSignalLevel)
{
	int ret, cnt, csq;
	char *str;
	int i;
	uchar buf[300], revBuf[128];;

	if(Lib_UartStatus(COMMUX0) == 0)
		return -1;	
	ret = Wls_NetworkStatus();
	if(ret < 0)
		csq = 1;
	else
	{
		for(i = 0; i < 3; i++)
		{
			ret = gsm_send_command(COMMUX0, "AT+CSQ\r", buf);
			if(str = strstr(buf, "+CSQ:"))
			{
				memset(revBuf, 0, 128);
				sscanf(str, "%*s%s", revBuf);
				cnt = 0;
				while(revBuf[cnt] != ',')
					cnt++;
				revBuf[cnt] = 0;
				csq = atoi(revBuf);
			}
			if(!(csq == 0 || csq == 99))
				break;
		}
	}
	if(csq == 0 || csq == 99)
		*pSignalLevel = 0x05; 
	else if(csq >= 1 && csq <= 7)
		*pSignalLevel = 0x04; 
	else if(csq >= 8 && csq <= 13)
		*pSignalLevel = 0x03; 
	else if(csq >= 14 && csq <= 19)
		*pSignalLevel = 0x02; 
	else if(csq >= 20 && csq <= 25)
		*pSignalLevel = 0x01; 
	else
		*pSignalLevel = 0x00; 
	return csq;
}

int Wls_CheckSignal2(int * pSignalLevel)
{
	int csq,i;
	int ret;
	
	unsigned char buf[300];
	while(giGprsBusy)
	{
		sleep(1);
	}
#ifdef GSMMUXD
//	gsm_send_command(COMMUX0, "AT\r", buf);
	ret = gsm_send_command(COMMUX0, "AT+CSQ\r", buf);
	if(ret < 0)
		ret = gsm_send_command(COMMUX0, "AT+CSQ\r", buf);
#else
	gprs_send_command("AT+CSQ\r", buf);
#endif /* GSMMUXD */
	csq = get_CSQ(buf);
	i=0;
	if(csq == 0 || csq == 99)
	{
		gsm_send_command(COMMUX0, "AT+CSQ\r", buf);
		csq = get_CSQ(buf);
	}
	if(csq == 0 || csq == 99)
		*pSignalLevel = 0x05; 
	else if(csq >= 1 && csq <= 7)
		*pSignalLevel = 0x04; 
	else if(csq >= 8 && csq <= 13)
		*pSignalLevel = 0x03; 
	else if(csq >= 14 && csq <= 19)
		*pSignalLevel = 0x02; 
	else if(csq >= 20 && csq <= 25)
		*pSignalLevel = 0x01; 
	else if(csq >25 && csq!= 99)
		*pSignalLevel = 0x00; 

	printf("csq = %d\n", csq);
	return csq;
}

/*
	return 0  ok
	       -1 simcard erro

*/

int Wls_CheckSim(void)
{
	int ret;
	char *str;
	uchar buf[300];

	if(Lib_UartStatus(COMMUX0) == 0)
		return -1;	
//	gsm_send_command(COMMUX0, "AT\r", buf);	
	ret = gsm_send_command(COMMUX0, "AT+CPIN?\r", buf);
	if(str = strstr(buf, "+CPIN:"))
	{
		if(strstr(str, "READY"))
			return 0;
		else
			return -2;
	}	
	return -2;

}


int get_CSQ(uchar *buf)
{
	int i, j;
	int csq;
	uchar buf2[3];
	
	i = 0;
	while(buf[i])
	{
		i++;
		if(buf[i-1] == ':')
			break;
		
	}
	j = 0;
	while(buf[i])
	{
		if(buf[i] == ',')
			break;
		else
		{
			buf2[j] = buf[i];
			j++;
			i++;
		}
	}
	buf2[j] = 0;
	csq = atoi(buf2);
	return csq;
}





int gprs_test(void)
{
	uchar buf[100], buf2[5];
	unsigned short len;
	int param;
	int nCmd;
	int ret, i, j;
	int csq;

	  sys_gprs_open();	
//	PRINTF("in:%s %d\n",__FUNCTION__,__LINE__); 
	if(Lib_UartInit(GPRS_COM, EM_232_115200BPS, 0, 1))
	{
		debug_log("uart open failed\n");
		return -1;
	}
	
	system("sh /etc/ppp/gprs_com_unlock");
	
	ret = gprs_send_command("AT\r", buf);
//	if(ret)
	{
		debug_log("gprs init...\n");
		gprs_power_off();
		sleep(1);
		gprs_power_on(); 
		sleep(5);
	}
	gprs_send_command("AT\r", buf);
	gprs_send_command("AT+IPR=115200\r", buf);
	gprs_send_command("ATE1\r", buf);
//	gprs_send_command("AT+FLO=0\r");
//	gprs_send_command("AT+GMI\r", buf);
//	gprs_send_command("AT+GMM\r", buf);
	ret = gprs_send_command("AT+CPIN?\r", buf);
	if(ret)
	{
		debug_log("SIM CARD ERROR\n");
	}
	else
	{
		debug_log("SIM CARD OK\n");
//		debug_log("GPRS start ... \n");
//		system("/etc/ppp/ppp-on");
	}
//	gprs_send_command("AT#QSS?\r");
	gprs_send_command("AT+CSQ\r", buf);
	csq = get_CSQ(buf);

	debug_log("csq = %d\n", csq);
	gprs_send_command("ATD 15813812446;\r", buf);

//	gprs_send_command("AT+CGDCONT=1,\"IP\",\"CMNET\"\r");
//	gprs_send_command("AT+CGCLASS=\"B\"\r");
//	gprs_send_command("AT+CGACT=1\r");
//	gprs_send_command("ATDT*99***1#\r");
//	gprs_send_command("ATE0\r");
//	gprs_send_command("ATE0\r");



	sys_gprs_close();
	Lib_UartClose(COM3);
//	PRINTF("in:%s %d\n",__FUNCTION__,__LINE__); 
	
	return ret;
}

