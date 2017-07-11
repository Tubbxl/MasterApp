#ifndef __SERIAL_H__
#define __SERIAL_H__

#define COM0 		0
#define COM1 		1
#define COM2 		2
#define COM3		3
#define COMS1 		4
#define COMS2		5
#define COMU0 		6
#define COMU1 		7
#define COMU2 		8
#define COMU3 		9
#define COMMUX0 	10
#define COMMUX1 	11

#define EM_232_9600BPS   0
#define EM_232_115200BPS 1

#define  COM_INVALID_PARA     (-6501)
#define  COM_NOT_OPEN         (-6502)
#define  COM_INVALID_PORTNO   (-6503)
#define  COM_RX_TIMEOUT       (-6504)
#define  COM_TX_TIMEOUT       (-6505)
#define  COM_TX_MEMOVER       (-6506)
#define  COM_NO_EMPTYPORT     (-6507)
int Lib_UartInit(unsigned char ComID,unsigned int Boudrate);

int Lib_UartClose(unsigned char ComID);

int Lib_UartReset(unsigned char ComID);
//从串口读取nbyte个字节,延时单位秒
int Lib_UartRecv(unsigned char ComID,unsigned char *strBuf, unsigned short *p_read_len,unsigned short timeout,unsigned short nbyte);

int Lib_UartSend(unsigned char ComID, unsigned char *strBuf, unsigned short nByte);

#endif