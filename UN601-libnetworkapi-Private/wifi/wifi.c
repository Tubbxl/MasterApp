#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <termios.h>
#include <fcntl.h>
#include <pthread.h>
#include <syslog.h>
#include <signal.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <assert.h>
#include <stdint.h>
#include <errno.h>
#include <net/if.h>
#include <linux/if_ether.h>

#include "wifi.h"


int DLL_WifiDisConnectNetwork();

static int giWlanNetNum;

int Dll_WlanListCheck(char *NetName)
{
	FILE *fp;	
	char buf[512], ssid[256];	
	int cnt = 0;
	int id[5];
	if(Dll_WifiOnOff())
		return -1;
	system("wpa_cli -iwlan0 list_networks > ./wifitmp");	
	fp=fopen("./wifitmp","r");
	memset(id, 0, 5);
	fgets(buf,512, fp);
	while(fgets(buf,99, fp)!= NULL)
	{
		memset(ssid, 0, 256);
		sscanf(buf,"%s %s",id, ssid);
		if(strcmp(ssid, NetName) == 0)
		{
			cnt = atoi(id);
			fclose(fp);
			return cnt;
		}
	}
	fclose(fp);
	return -1;
}

unsigned long get_file_size(const char *path)  
{  
    unsigned long filesize = -1;  
    FILE *fp;  
    fp = fopen(path, "r");  
    if(fp == NULL)  
        return filesize;  
    fseek(fp, 0L, SEEK_END);  
    filesize = ftell(fp);  
    fclose(fp);  
    return filesize;  
}  

void DLL_WifiOpen(void)
{
	if(system("ps  | grep -v grep | grep wpa_supplicant > NULL") != 0)
	{
		if(get_file_size("NULL")==0)
			system("wpa_supplicant -B -iwlan0 -c /etc/wpa_supplicant.conf -Dwext");
	}
	else
	{
		system("ifconfig wlan0 up");
	}
}

void DLL_WifiClose(void)
{
	DLL_WifiDisConnectNetwork();
	system("ifconfig wlan0 down");
}

void DLL_WifiSetIP(unsigned char *ip,unsigned char *mask,unsigned char *gateway)
{
	unsigned char buf[300];

	sprintf(buf,"ifconfig wlan0 %s netmask %s",ip,mask);
	system(buf);

	sprintf(buf,"route del default");
	system(buf);

	sprintf(buf,"route add default gw %s",gateway);
	system(buf);
	
}

static void KillDhclient()
{
	unsigned char buf[100],pid[10];
	FILE *fp;
	sprintf(buf,"ps -ef | grep dhclient >/tmp/dat.dat");
	system(buf);
	fp=fopen("/tmp/dat.dat","r");
	while(fscanf(fp,"%s",pid)!=EOF)
	{
		if(pid[0]==0)continue;
		sprintf(buf,"kill %s",pid);
		break;
	}
	fclose(fp);
	system(buf);
}

static int getNetworkStatus(unsigned char *network)
{
	FILE *fp;
	char buf[100];
	int i;	
	fp=fopen("/tmp/dat.dat","r");
	while(fscanf(fp,"%s",buf)!=EOF)
	{
		if(buf[0]==0)continue;
		if( memcmp(buf,"ESSID:\"",7)==0 && buf[7]!='\"')
		{
			memcpy(network,buf+7,strlen(buf)-8);
			network[strlen(buf)-8]=0;
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return -1;
}


int Dll_GetNetworkStatus(unsigned char *network)
{
	unsigned char buf[100];
	int iret;
	sprintf(buf,"iwconfig wlan0 >/tmp/dat.dat");
	system(buf);
	iret=getNetworkStatus(buf);
	memcpy(network,buf,strlen(buf));
	sprintf(buf,"rm /tmp/dat.dat");
	system(buf);
	return iret;
}

int DLL_WifiDisConnectNetwork()
{
	unsigned char buf[100];
	unsigned char pid[50];
	FILE *fp;
	int flag=0;

	//sprintf(buf,"ps -ef | grep wpa_supplicant >/tmp/dat.dat");
	sprintf(buf,"ps  | grep wpa_supplicant >/tmp/dat.dat");
	system(buf);
	flag=0;
	fp=fopen("/tmp/dat.dat","r");
	while(fscanf(fp,"%s",pid)!=EOF)
	{
		if(pid[0]==0) continue;
		
		if(flag==0) sprintf(buf,"kill %s",pid);

		flag++;
	}
	fclose(fp);
	system(buf);
	memset(buf,0x00,sizeof(buf));
	sprintf(buf,"rm /tmp/dat.dat");
	system(buf);
	return 0;
}


int Dll_WifiOnOff(void)
{
	if(system("ifconfig | grep wlan0 > NULL") == 0)
		return 0;
	else
		return -1;
}

int UtfStringToHEX( unsigned char * src,unsigned char *des )
{	
	int len = 0;	
	int i;	
	int ret_len = 0;	
	int cnt_char = 0;	
	char temch;	
	unsigned char *p = NULL;	
	unsigned char tembuf[1024]={0x00};	
	if(src == NULL || des == NULL)		
		return -1;	
	len = strlen(src);	
	if(len<1)		
		return -1;	
	for(i=0;i<len;i++)
	{		
		if(src[i]=='\\'||src[i]=='x')			
			continue;		
		tembuf[cnt_char++] = src[i];		
		printf("%c",src[i]);
	}	
	p = tembuf;	
	while(*p!=0x00)	
	{		
		sscanf(p,"%02x",&temch);		
		des[ret_len ++] = temch;
		printf("%02x,%d\n",temch,ret_len);
		p+=2;		
		i+=2;	
	}	
	//des[ret_len-1]=0x00;	
	return ret_len;	
}

int Get_LockSSID_Flag(T_WLANCLOCK *clockdata)
{
	FILE *fp;
	int i=4;
	char buf[212]={0};
	int flag = 0;
	fp=fopen("/usr/LockSSID.dat","r");
	//while(i--)
	{
	
		if(fscanf(fp,"%s",buf)!=EOF)
		{
			if(strcmp("unlock", buf) == 0)
				return 0;
			else if(strcmp("lock", buf) == 0)
				flag = 1;
		}
		//printf("1:%s \n",buf);
		memset(buf, 0, 100);
		if(fscanf(fp,"%s",buf)!=EOF)
			strcpy(clockdata->bssid, buf);
		//printf("2:%s \n",buf);
		memset(buf, 0, 100);
		if(fscanf(fp,"%s",buf)!=EOF)
			strcpy(clockdata->ssid, buf);
		//printf("3:%s \n",buf);
	}
	fclose(fp);
	return flag;
}

int DLL_WifiScan(T_WLANDEV *gWlanData)
{
	FILE *fp;
	char buf[1024];
	char tempbuf[1024]={0x00};
	T_WLANDEV WlanData[20]={0};
	T_WLANCLOCK clock={0};
	int clockflag = 0;
	int cnt = 0;
	int start = 0;
	int i = 0;
	if(Dll_WifiOnOff())
		return -1;
	clockflag = Get_LockSSID_Flag(&clock);
	system("wpa_cli scan > ./wifitmp");
	system("wpa_cli scan_results > ./wifitmp");
	fp=fopen("./wifitmp","r");
	//printf("wpa_cli scan_results > ./wifitmp start");
	memset(buf, 0, 1024);
	while(fscanf(fp,"%s",buf)!=EOF)
	{
		if(strcmp("ssid", buf) == 0)
			break;
	}
	while(1)
	{
		memset(buf, 0, 100);
		if(fscanf(fp,"%s",buf)!=EOF)
			strcpy(WlanData[cnt].bssid, buf);
		else 
			break;
		memset(buf, 0, 100);
		if(fscanf(fp,"%s",buf)!=EOF)
			strcpy(WlanData[cnt].frequency, buf);
		else 
			break;
		memset(buf, 0, 100);
		if(fscanf(fp,"%s",buf)!=EOF)
			strcpy(WlanData[cnt].signal, buf);
		else 
			break;
		memset(buf, 0, 100);
		if(fscanf(fp,"%s",buf)!=EOF)
		{
			strcpy(WlanData[cnt].flags, buf);
		}
		else 
			break;
		memset(buf, 0, 100);
		if(fscanf(fp,"%s",buf)!=EOF)
		{
			if(buf[0]=='\\')
			{
				memset(tempbuf,0x00,sizeof(tempbuf));
				UtfStringToHEX(buf,tempbuf);
				printf("%s____\n",tempbuf);
				strcpy(WlanData[cnt].ssid,tempbuf);
			}
			else
				strcpy(WlanData[cnt].ssid,buf);
		}
		else 
			break;
		if(strstr(WlanData[cnt].flags,"WPA")!=NULL)
		cnt++;
	
		if(cnt >50)
			break;
	}
	for(i = 0; i < cnt; i++)
	{
		printf("[%d %d] %s %s %s %s\n",cnt,i ,WlanData[i].bssid ,WlanData[i].ssid, WlanData[i].signal, WlanData[i].flags);
		if(clockflag == 1)
		{
			if(memcmp(clock.bssid,WlanData[i].bssid,17)==0)
			{
				memcpy(&gWlanData[0],&WlanData[i],sizeof(T_WLANDEV));
				printf("{%s %s}\n",gWlanData[0].bssid,gWlanData[0].ssid);
				//fclose(fp);
				//return 1;
			}
		}
		else
		{
			memcpy(&gWlanData[i],&WlanData[i],sizeof(T_WLANDEV));
		}
	}
	fclose(fp);
	system("rm ./wifitmp");
	if(clockflag==1&&cnt>0)
		return 1;
	else
		return cnt;
}



int Dll_WifiLinkStatus(void)
{
	FILE *fp;	
	char buf[512], status[20];
	
	if(Dll_WifiOnOff())
		return -1;
	system("wpa_cli -iwlan0 status > ./wifitmp");

	fp=fopen("./wifitmp","r");

	memset(buf, 0, 512);
	memset(status, 0, 20);
	while(fscanf(fp,"%s",buf)!=EOF)
	{
		if(strstr(buf, "wpa_state"))
			break;
	}
	strcpy(status, &buf[10]);	
	printf("status:%s\n", status);
	fclose(fp);
	if(strcmp(status, "COMPLETED") == 0)
	{
		return 0;
	}
	else if(strcmp(status, "4WAY_HANDSHAKE") == 0)
	{
		return WIFI_KEYERR;
	}
	else if(strcmp(status, "DISCONNECTED") == 0)
	{
		return WIFI_NOUER;
	}
	else if(strcmp(status, "SCANNING") == 0)
	{
		return WIFI_TIMEOUT;
	}
	else if(strcmp(status, "AUTHENTICATING") == 0)
	{
		return WIFI_AUTHENTICATING;
	}
	return 1;
}

int DLL_WifiConnectNetwork(T_WLANDEV aWlanDev, char *keyBuf)
{	
	FILE *fp;	
	char addNet[5], wlanCmd[512], status[10];
	int ret, cnt;
	
	if(Dll_WifiOnOff())
		return -1;
	system("wpa_cli -iwlan0 disconnect");

	ret = Dll_WlanListCheck(aWlanDev.ssid);
	if(ret >= 0)
	{
		memset(wlanCmd, 0, 512);
		sprintf(wlanCmd, "wpa_cli -iwlan0 select_network %d", ret);
		printf("%s\n", wlanCmd);
		system(wlanCmd);	
		goto WlanScanStatus;
	}
	
	system("wpa_cli -iwlan0 add_network > ./wifitmp");

	fp=fopen("./wifitmp","r");
	memset(addNet, 0, 5);
	fscanf(fp,"%s",addNet);	
	fclose(fp);
	giWlanNetNum = atoi(addNet);
	memset(wlanCmd, 0, 512);
	sprintf(wlanCmd, "wpa_cli -iwlan0 set_network %d ssid '\"%s\"'", giWlanNetNum, aWlanDev.ssid);
	printf("%s\n", wlanCmd);
	system(wlanCmd);

	if (strstr(aWlanDev.flags, "WPA")||strstr(aWlanDev.flags, "WPA2"))
	{	
		memset(wlanCmd, 0, 512);
		//sprintf(wlanCmd, "wpa_cli -iwlan0 set_network %d psk '\"%s\"'  > ./wifitmp", giWlanNetNum, keyBuf);
		sprintf(wlanCmd, "wpa_cli -iwlan0 set_network %d psk '\"%s\"'", giWlanNetNum, keyBuf);
		printf("%s\n", wlanCmd);
		system(wlanCmd);
	}
	/*else if (strstr(aWlanDev.flags, "WEP"))
	{
		memset(wlanCmd, 0, 512);
		sprintf(wlanCmd, "wpa_cli -iwlan0 set_network %d key_mgmt NONE", giWlanNetNum);
		printf("%s\n", wlanCmd);
		system(wlanCmd);

		memset(wlanCmd, 0, 512);
		sprintf(wlanCmd, "wpa_cli -iwlan0 set_network %d wep_key0 '\"%s\"' > ./wifitmp", giWlanNetNum, keyBuf);
		printf("%s\n", wlanCmd);
		system(wlanCmd);

		fp=fopen("./wifitmp","r");
		memset(status, 0, 10);
		fscanf(fp,"%s",status);			
		fclose(fp);		
		if (strstr(status, "OK") == NULL)
		{
			ret = WLANKEYERR;
			goto WlanConnectErr;
		}	

		memset(wlanCmd, 0, 512);
		sprintf(wlanCmd, "wpa_cli -iwlan0 set_network %d wep_tx_keyidx 0", giWlanNetNum);
		printf("%s\n", wlanCmd);
		system(wlanCmd);
	}*/
	else
	{
		/* OPEN */
		/*
		memset(wlanCmd, 0, 512);
		sprintf(wlanCmd, "wpa_cli -iwlan0 set_network %d key_mgmt NONE", giWlanNetNum);
		printf("%s\n", wlanCmd);
		system(wlanCmd);
		*/
		goto WlanConnectErr;
	}
	memset(wlanCmd, 0, 512);
	sprintf(wlanCmd, "wpa_cli -iwlan0 select_network %d", giWlanNetNum);
	printf("%s\n", wlanCmd);
	system(wlanCmd);	

WlanScanStatus:
	cnt = 0;
	while(1)
	{
		usleep(500*1000);
		ret = Dll_WifiLinkStatus();
		if(ret == 0)
		{
			ret = DLL_WifiDhcp();
			Dll_WlanUpdateConfig();
			break;
		}
		if(cnt++ > 10)
		{
			goto WlanConnectErr;
		}
	}
	return ret;
	
WlanConnectErr:
	memset(wlanCmd, 0, 512);
	sprintf(wlanCmd, "wpa_cli -iwlan0 remove_network %d", giWlanNetNum);
	printf("%s\n", wlanCmd);
	system("wpa_cli -iwlan0 disconnect");
	system(wlanCmd);
	return ret;
	
}


int DLL_WifiGetNetworkStatus(PT_WLANSTATE paWlanStatus)
{
	FILE *fp;	
	char buf[512];	
	if(Dll_WifiOnOff())
		return -1;
	
	system("wpa_cli -iwlan0 status > ./wifitmp");
	
	fp=fopen("./wifitmp","r");
	memset(paWlanStatus, 0, sizeof(T_WLANSTATE));
	while(fscanf(fp,"%s",buf)!=EOF)
	{
		if(strstr(buf, "ssid="))
			strcpy(paWlanStatus->ssid, &buf[strlen("ssid=")]);
		else if(strstr(buf, "ip_address="))
			strcpy(paWlanStatus->ip, &buf[strlen("ip_address=")]);
		else if(strstr(buf, "key_mgmt="))
			strcpy(paWlanStatus->key_mgmt, &buf[strlen("key_mgmt=")]);
		else if(strstr(buf, "wpa_state="))
			strcpy(paWlanStatus->wpa_state, &buf[strlen("wpa_state=")]);
		else if(strstr(buf, "address="))
			strcpy(paWlanStatus->mac_address, &buf[strlen("address=")]);

		memset(buf, 0, 512);
	}	
	fclose(fp);
	return 0;
}

int DLL_WifiDhcp(void)
{
	int cnt;
	
	if(Dll_WifiOnOff())
		return -1;
	if(system("ps  | grep -v grep | grep udhcpc > NULL") == 0)
		system("killall udhcpc");	

	system("udhcpc -i wlan0 -n ");
	return 0;
}

int DLL_WifiLockSSID(T_WLANDEV *LockAP)
{
	char buf[100]={0};
	FILE *fp;
	int ret = -1;
	if(LockAP == NULL)
	{
		sprintf(buf,"echo unlock > /usr/LockSSID.dat");
		system(buf);
		return 0;
	}
	system("echo lock > /usr/LockSSID.dat");
	memset(buf,0x00,sizeof(buf));
	sprintf(buf,"echo %s >> /usr/LockSSID.dat",LockAP->bssid);
	system(buf);
	memset(buf,0x00,sizeof(buf));
	sprintf(buf,"echo %s >> /usr/LockSSID.dat",LockAP->ssid);
	system(buf);
	system("sync");
	return 0;
}

int Dll_WlanUpdateConfig(void)
{

	if(Dll_WifiOnOff())
		return -1;
	system("wpa_cli -iwlan0 save_config");	
	system("sync");
	return 0;
}

T_WLANDEV gWlanDev[50];

int DLL_WifiGetMacAddr(unsigned char * mac)
{
	char buf[1000]={0};
	FILE *fp;
	int ret = -1;
	sprintf(buf,"ifconfig wlan0 >/tmp/wlan0.dat");
	system(buf);
	fp=fopen("/tmp/wlan0.dat","r");	
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
	sprintf(buf,"rm /tmp/wlan0.dat");
	system(buf);
	return ret;
	
}

int DLL_WifiGetNetPara(unsigned char * IP,unsigned char * Mask,unsigned char * Gateway)
{
	char buf[1000]={0};
	FILE *fp;
	int ret = -1;
	sprintf(buf,"ifconfig wlan0 >/tmp/wlan.dat");
	system(buf);
	fp=fopen("/tmp/wlan.dat","r");	
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
	sprintf(buf,"rm /tmp/wlan.dat");
	system(buf);
	return ret;
	
}

#if 0
int SetWifi()
{
	int i,j=0,ret,num;
	unsigned char ucKey;
	unsigned char keyStr[20]={0};
Wifi_Scan:
	DLL_WifiOpen();

	num = DLL_WifiScan(gWlanDev);
	if(num==0) 
		goto Wifi_Scan;
	memcpy(gWlanDev[1].bssid,"00:5a:13:47:a0:84",17);
	memcpy(gWlanDev[1].flags,"[WPA-PSK-CCMP][WPA2-PSK-CCMP][WPS][ESS]",39);
	memcpy(gWlanDev[1].frequency,"2417",4);
	memcpy(gWlanDev[1].signal,"60",2);
	memcpy(gWlanDev[1].ssid,"TP_LINK-unfiou",14);
		//ret = Dll_WifiApConnect(gWlanDev[1],"unifou0618");
	return 0;
	
}
int main()
{
	SetWifi();
	return 0;
}

#endif

