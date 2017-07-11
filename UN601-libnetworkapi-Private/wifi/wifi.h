#ifndef __WIFI_H__
#define __WIFI_H__

#define		WIFI_KEYERR		-9500
#define		WIFI_NOUER			-9501
#define		WIFI_TIMEOUT		-9502
#define		WIFI_DHCPERR		-9503
#define		WIFI_AUTHENTICATING		-9504


typedef struct WLANDEV {
	unsigned char bssid[20];
	unsigned char frequency[10];
	unsigned char signal[5];
	unsigned char flags[256];	
	unsigned char ssid[256];
}T_WLANDEV, *PT_WLANDEV;

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

typedef struct WLANCLOCK {
	unsigned char bssid[20];
	unsigned char ssid[256];
}T_WLANCLOCK, *PT_WLANCLOCK;

#endif

