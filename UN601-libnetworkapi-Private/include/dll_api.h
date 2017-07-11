#ifndef __DLL_API_H__
#define __DLL_API_H__

#ifndef ushort
#define ushort unsigned short
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef WORD
#define WORD unsigned short
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef DWORD
#define DWORD  unsigned long
#endif

//WIFI return value
#define  		WIFI_CONNECT_SUCCESS       		 0
#define  		WIFI_CONNECT_FAILED			-5500
#define  		WIFI_GET_DHCP_SUCCESS		0
#define  		WIFI_GET_DHCP_FAILED			-5501
#define		WIFI_KEYERR						-9500
#define		WIFI_NOUER						-9501
#define		WIFI_TIMEOUT					-9502
#define		WIFI_DHCPERR					-9503
#define		WIFI_AUTHENTICATING			-9504

//ethernet return value
#define  		ETH_OPEN_SUCCESS       		 0
#define  		ETH_ALREADY_OPEN			-5600

// 3G/4G PPP call
#define  		PPP_DIAL_SUCCESS       		 0
#define  		PPP_DIAL_FAILED       		 -5700
#define  		PPP_SIM_INSERT       		 	0
#define  		PPP_NO_SIM					-5701
#define  		PPP_CHECK_SIM_FAILED		-5702
#define  		PPP_CHECK_CSQ_FAILED		-5703
#define  		PPP_CALL_SUCCESS			0
#define  		PPP_CALL_FAILED			-5704
#define		GPRS_RESET_SUCCESS		0
#define		GPRS_RESET_FAILED			-5705
#define		PPP_RESET_SUCCESS		      0
#define		PPP_RESET_FAILED			-5706
#define  		PPP_GET_IMEI_SUCCESS       		 0
#define		PPP_GET_IMEI_FAILED			-5707

//WIFI module API
typedef struct WLANDEV {
	    unsigned char bssid[20];
	    unsigned char frequency[10];
	    unsigned char signal[5];
	    unsigned char flags[256];
	    unsigned char ssid[256];
}T_WLANDEV;

typedef struct WLANSTATE {
	    unsigned char ssid[256];
	    unsigned char ip[20];
	    unsigned char key_mgmt[30];
	    unsigned char wpa_state[30];
	    unsigned char mac_address[20];
}T_WLANSTATE, *PT_WLANSTATE;

typedef struct WLANLIST {
	    unsigned char id[5];
	    unsigned char ssid[256];
}T_WLANLIST, *PT_WLANLIST;

int DLL_WifiOpen();
int DLL_WifiClose();
int DLL_WifiGetMacAddr(unsigned char * mac);
int DLL_WifiScan(T_WLANDEV * gWlanData);
int DLL_WifiConnectNetwork(T_WLANDEV aWlanDev, char *keyBuf);
int DLL_WifiSetIP(unsigned char *ip,unsigned char *mask,unsigned char *gateway);
int DLL_WifiDhcp();
int DLL_WifiLockSSID(T_WLANDEV *LockAP);

int DLL_WifiGetNetworkStatus(T_WLANSTATE *paWlanStatus);
int DLL_WifiDisConnectNetwork();
int DLL_WifiGetNetPara(unsigned char * IP,unsigned char * Mask,unsigned char * Gateway);


//ethernet module API

int DLL_EthInit();
int DLL_EthSetLocalIP(unsigned char *ip);
int DLL_EthSetLocalIPDhcp();
int DLL_EthSetNetMask(unsigned char *mask);
int DLL_EthSetMacAddr(unsigned char *macAddr);
int DLL_EthSetGateway(unsigned char *gateway);
int DLL_EthGetMacAddr(unsigned char * mac);
int DLL_EthGetNetPara(unsigned char * IP,unsigned char * Mask,unsigned char * Gateway);

int DLL_EthClose(void);

// 3/4G module API

int DLL_PppStatus();
int DLL_PppReset();
int DLL_GprsReset(void);
int DLL_PppCheckCsq();
int DLL_PppCheckSim();
int DLL_PppGetIMEI(char *IMEI);
int DLL_PppDial( const unsigned char *apn,const unsigned char *username, const unsigned char *password,const unsigned char *tel);
void DLL_Pppdisconnect();
int DLL_PppGetNetPara(unsigned char * IP,unsigned char * Mask,unsigned char * Gateway);


#endif

