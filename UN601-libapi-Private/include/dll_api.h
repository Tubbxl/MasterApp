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

	
#define  Serial_Open_Err  -1000
#define  Packet_Send_Err  -1001
#define  Packet_Recv_Err  -1002
#define  App_Permissions_Denial  -1004

#define  Msgqueue_Open_Err  -1005
#define  Msgqueue_Send_Err  -1006
#define  Msgqueue_Recv_Err  -1007
#define  Msgqueue_Close_Err  -1008
#define  Get_AppNo_Fialed  -1009
	
//System
#define  Tel_Serial_No_Permission  -800
#define  Tel_Serial_Open_Err  -801
#define  Tel_Serial_Para_Err  -802

//KB
#define  KB_NoKey_Err -1009
#define  KB_InputPara_Err    -1010
#define  KB_InputTimeOut_Err -1011
#define  KB_UserCancel_Err   -1012
#define  KB_UserEnter_Exit   -1013
	
#define  PWD_OK   0
#define  PWD_VerifyDefaultOK   -1110
#define  PWD_DefaultVerify_Err   -1113
#define  PWD_Confirm_Err -1114
#define  PWD_Allzero_Err -1115
#define  PWD_DefaultSame_Err -1116
#define  PWD_Simple_Err -1117
#define  PWD_NotExist -1118
#define  PWD_NewVerify_Err -1119
#define  PWD_OldSame_Err -1120
#define  PWD_Save_Err -1121

	
//PCI
#define  PCI_OK  0

#define  PCI_Locked_Err  -7000
#define  PCI_KeyType_Err  -7001
#define  PCI_KeyLrc_Err  -7002
#define  PCI_KeyNo_Err  -7003	
#define  PCI_KeyLen_Err  -7004
#define  PCI_KeyMode_Err  -7005
#define  PCI_InputLen_Err  -7006
#define  PCI_InputCancel_Err  -7007	
#define  PCI_InputNotMatch_Err  -7008	
#define  PCI_InputTimeOut_Err  -7009	
#define  PCI_CallTimeInte_Err  -7010
#define  PCI_NoKey_Err  -7011
#define  PCI_WriteKey_Err  -7012
#define  PCI_ReadKey_Err  -7013	
#define  PCI_RsaKeyHash_Err  -7014
#define  PCI_DataLen_Err  -7015
#define  PCI_NoInput_Err  -7016
#define  PCI_AppNumOver_Err  -7017
#define  PCI_ReadMMK_Err  -7020	
#define  PCI_WriteMMK_Err  -7021	
#define  PCI_SignatureVerify_Err  -7030
#define  PCI_RsaKey_Err  -7031
#define  PCI_AuthTimes_Err  -7032	
#define  PCI_KeySame_Err  -7040	
	
#define  PCI_DUKPT_NoKey  -7050
#define  PCI_DUKPT_CounterOverFlow  -7051	
#define  PCI_DUKPT_NoEmptyList  -7052
#define  PCI_DUKPT_InvalidAppNo  -7053
#define  PCI_DUKPT_InvalidKeyID  -7054
#define  PCI_DUKPT_InvalidFutureKeyID  -7055
#define  PCI_DUKPT_InvalidCrc  -7056	
#define  PCI_DUKPT_InvalidBDK  -7057	
#define  PCI_DUKPT_InvalidKSN  -7058
#define  PCI_DUKPT_InvalidMode  -7059
#define  PCI_DUKPT_NotFound  -7060	
	
//Printer
#define  PRINTER_OK  0
#define  PRINTER_OUT_PAPER  -4002
#define  PRINTER_OVERHEAT  -4005	
#define  PRINTER_NOFONT  -4007
#define  PRINTER_BUFFER_OVERFLOW  -4008	
	
//MCR
#define  MCR_NOTSWIPED  -3000
#define  MCR_CallTimeInte_Err  -3010

//ICC
#define   ICC_OK                    0
#define   ICC_VCCMODEERR             -2500  
#define   ICC_INPUTSLOTERR           -2501  
#define   ICC_VCCOPENERR             -2502  
#define   ICC_ICCMESERR              -2503  

#define   ICC_T0_TIMEOUT             -2200  
#define   ICC_T0_MORESENDERR         -2201  
#define   ICC_T0_MORERECEERR         -2202  
#define   ICC_T0_PARERR              -2203  
#define   ICC_T0_INVALIDSW           -2204  

#define   ICC_DATA_LENTHERR          -2400  
#define   ICC_PARERR                 -2401  
#define   ICC_PARAMETERERR           -2402  
#define   ICC_SLOTERR                -2403  
#define   ICC_PROTOCALERR            -2404  
#define   ICC_CARD_OUT               -2405  
#define   ICC_NO_INITERR             -2406  
#define   ICC_ICCMESSOVERTIME        -2407  

#define   ICC_ATR_TSERR              -2100  
#define   ICC_ATR_TCKERR             -2101  
#define   ICC_ATR_TIMEOUT            -2102  
#define   ICC_TS_TIMEOUT             -2115  
#define   ICC_ATR_TA1ERR             -2103  
#define   ICC_ATR_TA2ERR             -2104  
#define   ICC_ATR_TA3ERR             -2105  
#define   ICC_ATR_TB1ERR             -2106  
#define   ICC_ATR_TB2ERR             -2107  
#define   ICC_ATR_TB3ERR             -2108  
#define   ICC_ATR_TC1ERR             -2109  
#define   ICC_ATR_TC2ERR             -2110  
#define   ICC_ATR_TC3ERR             -2111  
#define   ICC_ATR_TD1ERR             -2112  
#define   ICC_ATR_TD2ERR             -2113  
#define   ICC_ATR_LENGTHERR          -2114  

#define   ICC_T1_BWTERR              -2300  
#define   ICC_T1_CWTERR              -2301  
#define   ICC_T1_ABORTERR            -2302  
#define   ICC_T1_EDCERR              -2303  
#define   ICC_T1_SYNCHERR            -2304  
#define   ICC_T1_EGTERR              -2305  
#define   ICC_T1_BGTERR              -2306  
#define   ICC_T1_NADERR              -2307  
#define   ICC_T1_PCBERR              -2308  
#define   ICC_T1_LENGTHERR           -2309  
#define   ICC_T1_IFSCERR             -2310  
#define   ICC_T1_IFSDERR             -2311  
#define   ICC_T1_MOREERR             -2312  
#define   ICC_T1_PARITYERR           -2313  
#define   ICC_T1_INVALIDBLOCK        -2314  

#define   ICC_ER_DAIN                -2600  
#define   ICC_ER_DNIN                -2601  
#define   ICC_ER_NOCD                -2602  
#define   ICC_ER_SYSF                -2603  
#define   ICC_ER_TMOT                -2604  
#define   ICC_ER_AFTM                -2605  
#define   ICC_ER_INVA                -2606  
#define   ICC_ER_PAER                -2607  
#define   ICC_ER_FRAM                -2608  
#define   ICC_ER_EDCO                -2609  
#define   ICC_ER_INFR                -2610  
#define   ICC_ER_INFN                -2611  
#define   ICC_ER_INDN                -2612  
#define   ICC_ER_INPA                -2613  
#define   ICC_ER_TOPS                -2614  
#define   ICC_ER_INPS                -2615  
#define   ICC_ER_DOVR                -2616  
#define   ICC_ER_NSFN                -2617  
#define   ICC_ER_NSDN                -2618  
#define   ICC_ER_NSPR                -2619  
#define   ICC_ER_MEMF                -2620  

//WIFI return value
#define  WIFI_CONNECT_SUCCESS       		 0
#define  WIFI_CONNECT_FAILED			-5500
#define  WIFI_GET_DHCP_SUCCESS		0
#define  WIFI_GET_DHCP_FAILED			-5501

//ethernet return value
#define  ETH_OPEN_SUCCESS       		 0
#define  ETH_ALREADY_OPEN			-5600

// 3G/4G PPP call
#define  PPP_DIAL_SUCCESS       		 0
#define  PPP_DIAL_FAILED       		 -5700
#define  PPP_SIM_INSERT       		 	0
#define  PPP_NO_SIM					-5701
#define  PPP_CHECK_SIM_FAILED		-5702
#define  PPP_CHECK_CSQ_FAILED		-5703
#define  PPP_CALL_SUCCESS			0
#define  PPP_CALL_FAILED			-5704
int DLL_ApiInit();

//system module API
int DLL_SysBeep();
int DLL_SysBeef(BYTE mode, ushort DlyTimeMs);
int DLL_SysSetDateTime(BYTE *str);
int DLL_SysGetDateTime(BYTE *str);
int DLL_SysReadSecurityChipSN(BYTE *serialNo);
int DLL_SysGetVersion(BYTE *version);
int DLL_SysLedControl(BYTE index, BYTE mode);
int DLL_SysReadTamperRecord(BYTE *date, BYTE *record);
int DLL_SysGetEthMacAddr(BYTE *macAddr);
int DLL_SysSetEthMacAddr(BYTE *macAddr);
int DLL_SysGetIMEI(BYTE *imei);
int DLL_SysSoftReset(BYTE mode);
int DLL_SysShutdown();
int DLL_SysStandby();
int DLL_SysWriteSN(BYTE snLen, BYTE *serialNo);
int DLL_SysReadSN(BYTE *serialNo);
int DLL_KbInputStart();
int DLL_KbGetKey(unsigned char *keyValue);
int DLL_KbInputStop();
int DLL_SysReboot();

//ICC module API
typedef struct
{
	unsigned char   Command[4]; // CLA INS  P1 P2
	unsigned short  Lc;         // P3
	unsigned char   DataIn[512];//512
	unsigned short  Le;
} APDU_SEND;

typedef struct
{
	unsigned short  LenOut;     // length of dataout 
	unsigned char   DataOut[512];
	unsigned char   SWA;
	unsigned char   SWB;
} APDU_RECV;

int DLL_IccOpen(BYTE slot, BYTE vccMode, BYTE *ATR);
int DLL_IccClose(BYTE slot);
int DLL_IccCommand(BYTE slot, APDU_SEND * apduSend,APDU_RECV * apduRecv);
int DLL_IccCheck(BYTE slot);
int DLL_IccSelectEtu(BYTE slot, BYTE mode);


//MSR module API
int DLL_McrOpen();
int DLL_McrClose();
int DLL_McrReset();
int DLL_McrCheck();
int DLL_McrRead(BYTE * track1Len,BYTE *track1, BYTE *track2Len,  BYTE *track2, BYTE *track3Len,  BYTE *track3);
int DLL_McrReadVersion(BYTE *version);

//PICC module API
int DLL_PiccOpen();
int DLL_PiccClose();
int DLL_PiccCheck(BYTE mode, BYTE *cardType, BYTE *serialNo);
int DLL_PiccCommand(APDU_SEND * apduSend,APDU_RECV * apduRecv);
int DLL_PiccRemove();
int DLL_PiccHalt();
int DLL_PiccReset();
int DLL_PiccM1Authority(BYTE type, BYTE blkNo, BYTE *pwd, BYTE *serialNo);
int DLL_PiccM1ReadBlock(BYTE blkNo, BYTE *blkValue);
int DLL_PiccM1WriteBlock(BYTE blkNo, BYTE *blkValue);
int DLL_PiccM1Operate(BYTE type, BYTE blkNo, BYTE *value, BYTE updateBlkNo);

//printer module API
int DLL_PrnInit();
int DLL_PrnSetSpace(BYTE x, BYTE y);
int DLL_PrnSetFont(BYTE asciiFontHeight, BYTE extendFontHeight, BYTE zoom);
int DLL_PrnGetFont(BYTE *asciiFontHeight, BYTE *extendFontHeight, BYTE *zoom);
int DLL_PrnStep(int pixel);
int DLL_PrnStr(const char *str);
int DLL_PrnLogo(BYTE *logo);
int DLL_PrnStart();
int DLL_PrnSetLeftIndent(int x);
int DLL_PrnSetGray(BYTE nLevel);
int DLL_PrnCheckStatus();
int DLL_PrnSetInvert(BYTE fOn);
int DLL_PrnGetTemperature(int *tempValue);
int DLL_PrnSetTemperature(int tempValue);


//scan module API
int DLL_ScanOpen();
int DLL_ScanClose();
int DLL_ScanModeSet(BYTE mode,BYTE *prefix, BYTE *suffix);
int DLL_ScanTrigger();
int DLL_ScanRead(char *data);
int DLL_ScanKeySet(BYTE mode);

//PCI module API

typedef struct
{
	unsigned char   prompt[256]; 
	unsigned char   font[4];//R,G,B,size
	unsigned char   background[3];//R,G,B
} PinContext;

typedef struct{
	unsigned char ModulLen;          //Module length
	unsigned char Modul[248];        //Module
	unsigned char ExponentLen;       //Exponent length
	unsigned char Exponent[3];       //Exponent
}PUBLIC_KEY;

int DLL_PciGetRnd(BYTE *rnd);
int DLL_PciWriteMKey(BYTE keyType, BYTE keyNo, BYTE keyLen, BYTE *keyData, BYTE mode);
int DLL_PciWriteDukptKey(BYTE keyType, BYTE keyNo, BYTE bdkLen, BYTE *bdk, BYTE ksnLen, BYTE *ksn, BYTE mode);
int DLL_PciWriteSKey(BYTE keyType, BYTE keyNo, BYTE keyLen, BYTE *keyData, BYTE mode, BYTE mkeyNo);
int DLL_PciGetPin(BYTE keyNo,BYTE minLen,BYTE maxLen,BYTE cardNoLen, BYTE *cardNo,BYTE mode,BYTE *pinBlock ,BYTE mark,BYTE *amount,BYTE waitTimeSec,PinContext *ctx);
int DLL_PciGetMac(BYTE keyNo, int inLen, BYTE *inData, BYTE *macOut, BYTE mode);
int DLL_PciGetDes(BYTE keyNo, int inLen, BYTE *inData, BYTE *desOut, BYTE mode);
int DLL_PciGetPinDukpt( BYTE keyNo,BYTE minLen, BYTE maxLen, BYTE cardNoLen, BYTE *cardNo,BYTE mode,BYTE *pinBlock,BYTE mark, BYTE *amount, BYTE waitTimeSec,BYTE *outKsn,PinContext *ctx);
int DLL_PciGetMacDukpt( BYTE keyNo, int inLen, BYTE *inData, BYTE *macOut, BYTE mode,BYTE *outKsn);
int DLL_PciGetDesDukpt(BYTE keyNo, int inLen, BYTE *inData, BYTE *desOut, BYTE mode, BYTE *outKsn);
int DLL_PciGetPinByFixKey(BYTE keyNo,BYTE minLen,BYTE maxLen,BYTE cardNoLen, BYTE *cardNo,BYTE mode,BYTE *pinBlock ,BYTE mark,BYTE *amount,BYTE waitTimeSec,PinContext *ctx);
int DLL_PciOffLineEncPin(BYTE slot, BYTE minLen, BYTE maxLen, BYTE waitTimeSec, PUBLIC_KEY *pk, PinContext *ctx);
int DLL_PciOffLinePlainPin(BYTE slot, BYTE minLen, BYTE maxLen, BYTE waitTimeSec, PinContext *ctx);
int DLL_PciWriteSignKey(BYTE fwrType, BYTE *keyData, BYTE *signData);
int DLL_PciVerifySignature(BYTE fwrType, BYTE *verifyData);
int DLL_PciCheckSecurityChipStatus();
int DLL_PciPwdInput(BYTE pwdType, BYTE mode);
int DLL_PciJumpToOs();
int DLL_PciStartDownloadOs();
int DLL_PciClearAppkeys(BYTE appNo);
int DLL_PciClearAllAppkeys();
int DLL_PciGetKeyCheckValue(BYTE appNo,BYTE keyType,BYTE keyNo, BYTE *checkValue);
int DLL_PciGetSignKeyCheckValue(BYTE keyType, BYTE *checkValue);


//WIFI module API
typedef struct _wifi_struct{
        BYTE SSID[40];
	BYTE signal[20];
        BYTE mode;
} WIFI_STRUCT;

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


int DLL_WifiOpen();
int DLL_WifiClose();
int DLL_WifiScan(PT_WLANDEV gWlanData);
int DLL_WifiConnectNetwork(T_WLANDEV aWlanDev, char *keyBuf);
int DLL_WifiSetIP(unsigned char *ip,unsigned char *mask,unsigned char *gateway);
int DLL_WifiDhcp();
int DLL_WifiGetNetworkStatus(unsigned char *network);
int DLL_WifiDisConnectNetwork();

//ethernet module API

int DLL_EthInit();
int DLL_EthSetLocalIP(unsigned char *ip);
int DLL_EthSetNetMask(unsigned char *mask);
int DLL_EthSetMacAddr(unsigned char *macAddr);
int DLL_EthSetGateway(unsigned char *gateway);
int DLL_EthClose(void);

// 3/4G module API

int DLL_PppStatus();
int DLL_PppReset();
int DLL_PppCheckCsq();
int DLL_PppCheckSim();
int DLL_PppDial( const unsigned char *apn,const unsigned char *username, const unsigned char *password);
int DLL_PppCall(char *tel);

//Serial module API
int DLL_SerialSend(unsigned char *strBuf, unsigned short nByte);
int DLL_SerialRecv(unsigned char *strBuf, unsigned short *p_read_len, unsigned short timeout,unsigned short nbyte);

//External serial module API
int DLL_ExternalSerialOpen();
int DLL_ExternalSerialSend(unsigned char *strBuf, unsigned short nByte);
int DLL_ExternalSerialRecv(unsigned char *strBuf, unsigned short *p_read_len, unsigned short timeout,unsigned short nbyte);
int DLL_ExternalSerialClose();

//Download module API
int DLL_DownloadApp(char *strPath);

#endif
