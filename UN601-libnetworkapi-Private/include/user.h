#ifndef __USER_H__
#define __USER_H__


#ifndef ushort
#define ushort unsigned short
#endif

#ifndef BYTE
#define BYTE unsigned char
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

#ifndef uint
#define uint  unsigned int
#endif
#define MAKEWORD(a, b)      (((WORD)(a) & 0xff) | ((WORD)(b) << 8))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))
#define 	_IO(x,y)		(((x)<<8)|y)

//scanner ioctl cmd
#define CMD_POWER_ON			_IO(0xFF,0)
#define CMD_POWER_OFF		_IO(0xFF,1)
#define CMD_CTL_START	    		_IO(0xFF,2)
#define CMD_CTL_STOP	    		_IO(0xFF,3)

#define WIIPAY500 0
#define QCOM0     1 
#define QCOM1     2
#define IMX6UL_COM0  3
#define IMX6UL_COM1  4
#define IMX6UL_COM2  5
#define IMX6UL_COM3  6
//与32550通信接口
#define PINPAD_COM IMX6UL_COM2

//与PC通信接口
#define TRSM_COM QCOM1
#define COMPORT QCOM1

unsigned char g_abySendBuff[1024];
unsigned char g_abyRecvBuff[1024];

void Crc16OpenCom(); 
int Crc16SendPacket(unsigned char *pbySendData, unsigned short wDataLen, unsigned short wCmd);
int Crc16RecvPacket(unsigned char *pbyRecvData, unsigned short *pwPacketetLen,unsigned short *pwCmd, unsigned long dwTimeoutMs);
void Crc16CloseCom();

#define  RECV_TIMEOUT 5000 //ms


//PCI module command
#define PCI_KEY_LOAD_MODULE 0xb3
#define PCI_KEY_USE_MODULE 0xb6
#define PCI_VERIFY_MODULE 0xb9
#define PCI_RND_MODULE 0xbc
//ICC module command
#define ICC_MODULE 0xc1
//MSR module command
#define MSR_MODULE 0xc2
//PICC module command
#define PICC_MODULE 0xc3
//PRINTER module command 
#define PRINTER_MODULE 0xe1
//SYSTEM module command
#define SYSTEM_MODULE 0xd1
//LCM module command
#define LCM_MODULE 0xf1


//PCI Module function
//KEY_LOAD
#define PCI_KEY_LOAD_SIGN     0x20
#define PCI_KEY_LOAD_MASTER    0x22
#define PCI_KEY_LOAD_SESSION   0x24
#define PCI_KEY_LOAD_DUKPTK    0x26

//KEY TYPE DEFINITION	
#define PIN_MK    0x00
#define MAC_MK    0x01
#define DES_MK    0x02
#define FIX_MK    0x03
#define PINK      0x04
#define MACK     0x05
#define DESK      0x06
#define DUKPTK   0x07	

//KEY_USE
#define PCI_PIN_ENC_PINK       0x01
#define PCI_MAC_ENC_MACK       0x03
#define PCI_DES_ENC_DESK       0x05
#define PCI_PIN_ENC_DUKPT      0x07
#define PCI_MAC_ENC_DUKPT      0x09
#define PCI_DES_ENC_DUKPT      0x0b
#define PCI_PIN_ENC_FIXK       0x0d
#define PCI_OFFLINE_ENC_VER    0x0f
#define PCI_OFFLINE_CLR_VER    0x11
#define PCI_CLEAR_APP_KEYS     0x13
#define PCI_GET_MKEY_CHECKVALUE 0x15
#define PCI_GET_SKEY_CHECKVALUE 0x17
#define PCI_CLEAR_ALL_APPS_KEYS 0x19

//PCI VERIFY
#define PCI_VERIFY_SIGN        0x01
#define PCI_CHECK_STATUS       0x03
#define PCI_PWD_INPUT          0x05
#define PCI_JUMPTO_OS          0x07
#define PCI_DOWNLOAD_OS        0x09
#define PCI_UNLOCK_PWD_INPUT   0x0b


//RND
#define PCI_GET_RND            0x00


//ICC Module function
#define ICC_OPEN    0x01
#define ICC_CLOSE   0x03
#define ICC_COMMAND 0x05
#define ICC_CHECK   0x07
#define ICC_SELETU  0x09

//MSR Module function
#define MSR_OPEN  0x01
#define MSR_CLOSE 0x03
#define MSR_CHECK 0x05
#define MSR_READ  0x07
#define MSR_RESET 0x09
#define MSR_READ_VERSION 0x0b

//PICC Module function
#define PICC_OPEN         0x01
#define PICC_CLOSE        0x03
#define PICC_CHECK        0x05
#define PICC_COMMAND     0x07
#define PICC_REMOVE       0x09
#define PICC_HALT         0x0b
#define PICC_RESET        0x0d
#define PICC_M1AUTHORITY  0x11
#define PICC_M1READBLOCK  0x13
#define PICC_M1WRITEBLOCK 0x15
#define PICC_M1OPERATE    0x17

//PRINTER Module function
#define PRINTER_SETLINE  0x00
#define PRINTER_SENDDOT  0x02
#define PRINTER_START    0x04
#define PRINTER_GETTEMP  0x06
#define PRINTER_SETGRAY  0x08
#define PRINTER_SETTEMP  0x0A

//SYSTEM Module function
#define SYS_SETRTC         0x01
#define SYS_GETRTC          0x03
#define SYS_GETVER          0x05
#define SYS_CTLLED          0x07
#define SYS_SOUNDBEEP       0x09
#define SYS_SOUNDBEEF       0x0b
#define SYS_GETSN           0x0d
#define SYS_SOFTRESET       0x11
#define SYS_SHUTDOWN        0x12
#define SYS_SLEEP           0x14
#define SYS_GETTAMPERRECORD 0x15
#define SYS_WRITESN         0x17
#define SYS_READSN          0x19
#define KB_TEST_START       0x1B
#define KB_TEST_END         0x1D


//LCM Module function
#define LCM_INIT   0x00
#define LCM_CLEAR  0x02
#define LCM_STRING 0x04
#define LCM_SEG    0x06
#define LCM_SETGRAY 0x08
#define LCM_CLOSE  0x0A

#endif
