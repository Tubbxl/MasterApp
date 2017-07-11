#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>
#include "DLL_api.h"



static unsigned char mac_addr[6] = {0};
static unsigned char have_mac_addr = 0;
static int lan_has_startup = 0;
static inline void __Eth_Change_Mac()
{
	unsigned char cmd[50]={0};
	unsigned char buf[50]={0};
	int i;
	FILE *fp=NULL;
	fp=fopen("/etc/network/mac.dat","r");
	if(fp!=NULL)
	{
		while(fscanf(fp,"%s",buf)!=EOF)
		{
			
			if(buf[0]==0)continue;
			else 
			{
				sprintf((char *)cmd,"ifconfig eth0 hw ether %s",buf);
				system(cmd);
				return;
			}
		}
	}
	mac_addr[0]=0x00;
	mac_addr[1]=0x0C;
	mac_addr[2]=0xD6;
	srand((int)time(NULL));	
	for(i = 3; i < 6; i++) 
	{
		mac_addr[i] = (unsigned char)rand();

	}	
	sprintf((char *)cmd,"ifconfig eth0 hw ether %02x:%02x:%02x:%02x:%02x:%02x",
		mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
	system(cmd);
}

int DLL_EthInit()
{
	if (lan_has_startup) return ETH_ALREADY_OPEN;
	system("ifconfig eth0 up");
	lan_has_startup = 1;
	__Eth_Change_Mac();
	
	return ETH_OPEN_SUCCESS;
}

int DLL_EthSetLocalIP(unsigned char *ip)
{
	unsigned char cmd[30];
	if(strlen(ip)==4)
	{
		sprintf((char *)cmd,"ifconfig eth0 %d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);
	}
	else 
		sprintf((char *)cmd,"ifconfig eth0 %s",ip);
	system(cmd);
	return 0;
}


int DLL_EthSetLocalIPDhcp()
{
	
	system("udhcpc -i eth0 -n");
	return 0;
}

int DLL_EthSetNetMask(unsigned char *mask)
{
	unsigned char cmd[40];
	if(strlen(mask)==4)
	{
		sprintf((char *)cmd,"ifconfig eth0 netmask %d.%d.%d.%d",mask[0],mask[1],mask[2],mask[3]);
	}
	else 
		sprintf((char *)cmd,"ifconfig eth0 netmask %s",mask);
	system(cmd);
	return 0;
}

int DLL_EthSetMacAddr(unsigned char *HWaddr)
{
	unsigned char cmd[40]={0};
	unsigned char lowbuf[20]={0};
	unsigned char upbuf[20]={0};
	char  i =0;
	char*temp=NULL;
	if(strlen(HWaddr)==6)
	{
		sprintf((char *)cmd,"ifconfig eth0 hw ether %02x:%02x:%02x:%02x:%02x:%02x",HWaddr[0],HWaddr[1],HWaddr[2],HWaddr[3],HWaddr[4],HWaddr[5]);
		system(cmd);
		memset(cmd,0x00,sizeof(cmd));
		sprintf((char *)cmd,"echo %02x:%02x:%02x:%02x:%02x:%02x > /etc/network/mac.dat",HWaddr[0],HWaddr[1],HWaddr[2],HWaddr[3],HWaddr[4],HWaddr[5]);
		system(cmd);
	}
	else 
	{
		sprintf((char *)cmd,"ifconfig eth0 hw ether %s",HWaddr);
		system(cmd);
		memset(cmd,0x00,sizeof(cmd));
		sprintf((char *)cmd,"echo %s > /etc/network/mac.dat",HWaddr);
		system(cmd);
	}
	system("sync");
	return 0;
}

int DLL_EthSetGateway(unsigned char *gateway)
{
	unsigned char cmd[40];
	if(strlen(gateway)==4)
	{
		sprintf((char *)cmd,"route add default gw %d.%d.%d.%d",gateway[0],gateway[1],gateway[2],gateway[3]);
	}
	else 
		sprintf((char *)cmd,"route add default gw %s",gateway);
	system(cmd);
	return 0;
}

int DLL_EthClose(void)
{
	system("ifconfig eth0 down");
	lan_has_startup = 0;
	return 0;
}

int DLL_EthGetMacAddr(unsigned char * mac)
{
	char buf[1000]={0};
	FILE *fp;
	int ret = -1;
	sprintf(buf,"ifconfig eth0 >/tmp/eth.dat");
	system(buf);
	fp=fopen("/tmp/eth.dat","r");	
	while(fscanf(fp,"%s",buf)!=EOF)
	{
		if((buf[2]==':')&&(buf[5]==':')&&(buf[8]==':'))
		{
			memcpy(mac,buf,18);			
			ret = 0;
			break;
		}
		memset(buf,0x00,sizeof(buf));
	}
	fclose(fp);
	sprintf(buf,"rm /tmp/eth.dat");
	system(buf);
	return ret;
	
}
int DLL_EthGetNetPara(unsigned char * IP,unsigned char * Mask,unsigned char * Gateway)
{
	char buf[1000]={0};
	FILE *fp;
	int ret = -1;
	sprintf(buf,"ifconfig eth0 >/tmp/eth.dat");
	system(buf);
	fp=fopen("/tmp/eth.dat","r");	
	while(fscanf(fp,"%s",buf)!=EOF)
	{
		if(memcmp(buf,"addr",4)==0)
		{
			memcpy(IP,buf+5,strlen(buf)-5);
		}
		else if(memcmp(buf,"Bcast",5)==0)
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
	sprintf(buf,"rm /tmp/eth.dat");
	system(buf);
	return ret;
	
}

int main()
{
	int ret = -1;
	unsigned char buf[100]={0};
	unsigned char buf1[100]={0};
	unsigned char buf2[100]={0};
	//ret = DLL_EthGetMacAddr(buf);
	DLL_EthGetNetPara(buf,buf1,buf2);
	printf("%s\n%s\n%s\n1234567",buf,buf1,buf2);
	return 0;
}
