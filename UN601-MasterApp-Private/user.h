#ifndef __USER_H__
#define __USER_H__


//#ifndef ushort
//#define ushort unsigned short
//#endif

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

//#ifndef uint
//#define uint  unsigned int
//#endif


#define MAKEWORD(a, b)      (((WORD)(a) & 0xff) | ((WORD)(b) << 8))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

#define WIIPAY500 0
#define QCOM0     1 
#define QCOM1     2
#define IMX6UL_COM0  3
#define IMX6UL_COM1  4
#define IMX6UL_COM2  5

#define PINPAD_COM IMX6UL_COM2


#define TRSM_COM QCOM1
#define COMPORT QCOM1

extern unsigned char g_abyRecvBuff[1024];

void Crc16OpenCom(); 
int Crc16SendPacket(unsigned char *pbySendData, unsigned short wDataLen, unsigned short wCmd);
int Crc16RecvPacket(unsigned char *pbyRecvData, unsigned short *pwPacketetLen,unsigned short *pwCmd, unsigned long dwTimeoutMs);
void Crc16CloseCom();

void PortSend(unsigned char ch);
int PortRecv(unsigned char * ch,WORD ms);

int SendPack(BYTE cmd, BYTE *SendBuff, int SndLen);
int RecvPack(BYTE *pbyRecvBuff, DWORD *pdwRecvLen);

int DownloadApp(char* filePath);


#endif
