
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
#include <fcntl.h>
#include <time.h>
#include <dirent.h>
#include <termios.h>
#include <sys/file.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>

extern "C" {
 #include "include/dll_api.h"
}

#include "user.h"

#include <QDebug>




static int com_fd[12]={0};
static int com_flag[12]={0};
unsigned char g_abyRecvBuff[1024];


int COMMAND_COM = PINPAD_COM;

#define ERR_DEVICE 101
#define RECV_PACKET_ERROR 102
#define CRC_CHECK_ERROR 103
#define  SUCCESS					0

#define DEVICE_OPEN 1

#define  COM_TX_WRITE_DEV_ERROR		(-6509)

#define  COM_OPEN_DEV_ERROR		 	(-6510)
#define  COM_READ_DEV_ERROR			(-6511)
#define  COM_WRITE_DEV_ERROR		(-6512)
#define  COM_SET_PARA_ERROR	 		(-6513)

#define  COM_CREAT_THREAD_ERROR	 	(-6514)
#define  COM_SELECT_MODULE_ERROR	(-6515)
#define  COM_PORT_IN_USE            (-6516)

int Lib_UartInit(unsigned char ComID,unsigned int Boudrate)
{
  	int status;
   char com_path[20];
  	struct termios   options;

  	if (com_flag[ComID] == DEVICE_OPEN) 
  	{
		return SUCCESS;
  	}
	
	memset(com_path, 0, 20);

	if(ComID == WIIPAY500)
		sprintf(com_path, "/dev/ttySAC1");
	else if(ComID == QCOM0)
		sprintf(com_path, "/dev/ttyHSL0");
	else if(ComID == QCOM1)
		sprintf(com_path, "/dev/ttyHSL1");
	else if(ComID == IMX6UL_COM0)
		sprintf(com_path, "/dev/ttymxc0");
	else if(ComID == IMX6UL_COM1)
		sprintf(com_path, "/dev/ttymxc1");
	else if(ComID == IMX6UL_COM2)
		sprintf(com_path, "/dev/ttymxc2");

	printf("%s port = %d com = %s \n", __FUNCTION__, ComID, com_path);

	printf("com_path : %s\n", com_path);
  	if ((com_fd[ComID] = open(com_path, O_RDWR | O_NDELAY)) == -1) 
	{
		if(ComID==WIIPAY500)
		{
			sprintf(com_path, "/dev/ttyHSL1");
		}
		if ((com_fd[ComID] = open(com_path, O_RDWR | O_NDELAY)) == -1) {
        	printf("uart init ERR_DEVICE\n");
            return ERR_DEVICE;
		}
  	}
	

	com_flag[ComID] = DEVICE_OPEN;
	
  	tcflush(com_fd[ComID],TCIOFLUSH);
  	tcgetattr(com_fd[ComID], &options);
  	  	
  	switch(Boudrate)
  	{
  		case 9600:
  	       options.c_cflag = B9600|CS8|CLOCAL|CREAD;
  	       break;
  	  	case 115200:
  	  	   options.c_cflag = B115200|CS8|CLOCAL|CREAD;
		   break;
		case 230400:
			options.c_cflag = B230400|CS8|CLOCAL|CREAD;
			break;
  	       break;
  	}
	


	options.c_iflag = IGNPAR;
	options.c_lflag = 0;
	options.c_oflag = 0;
  	
  	options.c_cc[VTIME]=3; 
  	options.c_cc[VMIN] =200;


	//options.c_cflag |= PARENB;
	//options.c_cflag |= PARODD;
	//options.c_iflag |= (INPCK | ISTRIP);

	

  	status = tcsetattr(com_fd[ComID],TCSANOW,&options);
  	if(status!=0) 
	{        
    		close(com_fd[ComID]);
    		com_flag[ComID] = -1;
            printf("uart init ERR_DEVICE");
            
    		return ERR_DEVICE;     
  	}    

  	tcflush(com_fd[ComID],TCIOFLUSH); 
  	return SUCCESS;
}


int Lib_UartClose(unsigned char ComID)
{	  	
  	if (com_flag[ComID]  != DEVICE_OPEN) 
  	{
  		return SUCCESS;
  	}

  	close(com_fd[ComID]);
  	com_flag[ComID] = 0;
  	
  	return SUCCESS;
}


int Lib_UartReset(unsigned char ComID)
{	  	
  	if (com_flag[ComID]  != DEVICE_OPEN) 
  	{
  		  return ERR_DEVICE;
  	}

  	tcflush(com_fd[ComID],TCIOFLUSH);

  	
  	return SUCCESS;
}



//int Lib_UartRecv(unsigned char ComID,unsigned char *strBuf, unsigned short *p_read_len, unsigned short timeout,unsigned short nbyte)
//{
//  	int  nleft, nread, nRet;
//  	fd_set rfds;
//  	struct timeval tv;
	  	
//  	if (com_flag[ComID]  != DEVICE_OPEN)
//  	{
//  		  return 101;
//  	}

//  	nleft = nbyte;

// 	FD_ZERO(&rfds);
//  	FD_SET(com_fd[ComID], &rfds);
//  	tv.tv_sec = timeout/1000;
//  	tv.tv_usec = timeout %1000*1000;

//    *p_read_len = 0;
//  	while(nleft > 0)
//  	{
//		nRet = select(com_fd[ComID]+1, &rfds, NULL, NULL, &tv);
//		if (nRet > 0)
//	    {
//  			nread = read(com_fd[ComID], strBuf, nleft);
//  			if (nread <= 0)
//  			{
//				printf("nread = %d : %s\n", nread, strerror(errno));
//    			return 101;
//  			}
//  			nleft -= nread;
//  			strBuf += nread;
//		}
//		else if (nRet == 0)
//		{
//  			if(tv.tv_sec == 0 && tv.tv_usec == 0)
//			{
//				*p_read_len = (nbyte-nleft);
//				if(*p_read_len==0) return 102;
//		        return 0;
//		    }
//		}
//		else
//		{
//		      return 101;
//		}
//  	}
//  	*p_read_len = (nbyte-nleft);
//	return 0;
//}


//int Lib_UartSend(unsigned char ComID, unsigned char *strBuf, unsigned short nByte)
//{
//  	int  nLeft, nwrite, nRet;
//  	fd_set wfds;
//  	struct timeval tv;
//    int nTimeOut = 3;
    	
//  	if (com_flag[ComID] != DEVICE_OPEN)
//  	{
//        return ERR_DEVICE;
//  	}
  	
//  	FD_ZERO(&wfds);
//  	FD_SET(com_fd[ComID], &wfds);
//  	tv.tv_sec = nTimeOut;
//  	tv.tv_usec = 0;

//  	nLeft = nByte;

//  	while (nLeft > 0)
//  	{
//		nRet = select(com_fd[ComID]+1, NULL, &wfds, NULL, &tv);
//		if (nRet > 0)
//	    {
//  			nwrite = write(com_fd[ComID], strBuf, nLeft);
//            //printf("nwrite = %d\n", nwrite);
//  			if (nwrite <= 0)
//  			{
//                return (nByte - nLeft);
//  			}
//  			nLeft -= nwrite;
//  			strBuf += nwrite;
//		}
//  	}
//  	return (nByte - nLeft);
//}

#define COM_PACKET_DEBUG

int Crc16SelectCom(unsigned char ComID)
{
	COMMAND_COM = ComID;
	return 0;
}

void Crc16OpenCom()
{
	Lib_UartInit(COMMAND_COM,115200);
	Lib_UartReset(COMMAND_COM);
}
void Crc16CloseCom()
{
	Lib_UartClose(COMMAND_COM);
}

WORD g_awhalfCrc16CCITT[16]={  
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
};
void Crc16CCITT(BYTE *pbyDataIn, DWORD dwDataLen, BYTE abyCrcOut[2])
{
    WORD wCrc = 0;
    BYTE byTemp;
    while (dwDataLen-- != 0)
    {
        byTemp = ((BYTE)(wCrc>>8))>>4;
        wCrc <<= 4; 
        wCrc ^= g_awhalfCrc16CCITT[byTemp^(*pbyDataIn/16)]; 
        byTemp = ((BYTE)(wCrc>>8))>>4;
        wCrc <<= 4;
        wCrc ^= g_awhalfCrc16CCITT[byTemp^(*pbyDataIn&0x0f)]; 
        pbyDataIn++; 
    }
    abyCrcOut[0] = wCrc/256;
    abyCrcOut[1] = wCrc%256;    
}

COM_PACKET_DEBUG

int Crc16SendPacket(unsigned char *pbySendData, unsigned short wDataLen, unsigned short wCmd)
{
    unsigned char abyBuff[1024];
    int iRet;
    int i;

    abyBuff[0] = 0x02;
    abyBuff[1] = (unsigned char)(wCmd/256);
    abyBuff[2] = (unsigned char)(wCmd%256);
    abyBuff[3] = wDataLen/256;
    abyBuff[4] = wDataLen%256;
    memcpy(&abyBuff[5], pbySendData, wDataLen);
    Crc16CCITT(&abyBuff[1], wDataLen+4, &abyBuff[wDataLen+5]);

	//if(com_flag[COMMAND_COM]!=DEVICE_OPEN) Lib_UartInit(COMMAND_COM,115200);
	
#ifdef COM_PACKET_DEBUG
    for(i=0;i<wDataLen+7;i++) qDebug("%02x ",abyBuff[i]);
#endif
	//Lib_UartReset(COMMAND_COM);
//    iRet = Lib_UartSend(COMMAND_COM,abyBuff,wDataLen+7);



     iRet = DLL_SerialSend(abyBuff,wDataLen+7);


    if (iRet<0)
    {
        return 101;
    }
    return 0;
}


int Crc16RecvPacket(unsigned char *pbyRecvData, unsigned short *pwPacketetLen,unsigned short *pwCmd, unsigned long dwTimeoutMs)
{
//	unsigned char buff[100];
    unsigned char abyCrc[2];
    int i,iRet;
    unsigned short wPacketLen = 0;
    //unsigned long  dwBegintick, dwCurtick;
 	unsigned short recv_len;

	
    while (1)
    {
  //      iRet = Lib_UartRecv(COMMAND_COM,pbyRecvData,&recv_len,dwTimeoutMs,1);
         iRet = DLL_SerialRecv(pbyRecvData,&recv_len,dwTimeoutMs,1);
        if ((0==iRet) && (0x02==pbyRecvData[0]))
        {
            break;
        }
		
		if(iRet != 0) return 1;
    }
	

    //iRet = Lib_UartRecv(COMMAND_COM,pbyRecvData+1,&recv_len,dwTimeoutMs,4);
     iRet = DLL_SerialRecv(pbyRecvData+1,&recv_len,dwTimeoutMs,4);
    if (0 != iRet)
    {
        return RECV_PACKET_ERROR;
    }


	*pwCmd = pbyRecvData[1]*256 + pbyRecvData[2];
    wPacketLen = pbyRecvData[3]*256 + pbyRecvData[4];
    *pwPacketetLen = wPacketLen;

    for (i=0; i<wPacketLen+2; i++)
    {
      //  iRet = Lib_UartRecv(COMMAND_COM,pbyRecvData+i+5,&recv_len,dwTimeoutMs,1);
         iRet = DLL_SerialRecv(pbyRecvData+i+5,&recv_len,dwTimeoutMs,1);
        if (0 != iRet)
        {
            return RECV_PACKET_ERROR;
        }
    }


    Crc16CCITT(&pbyRecvData[1], wPacketLen+4, abyCrc);
    if (0 != memcmp(abyCrc, &pbyRecvData[5+wPacketLen], 2))
    {
        return CRC_CHECK_ERROR;
    }

    return 0;
}



const DWORD gc_dwCrc32Table[256]={
    0x00000000,0x77073096,0xee0e612c,0x990951ba,
    0x076dc419,0x706af48f,0xe963a535,0x9e6495a3,
    0x0edb8832,0x79dcb8a4,0xe0d5e91e,0x97d2d988,
    0x09b64c2b,0x7eb17cbd,0xe7b82d07,0x90bf1d91,
    0x1db71064,0x6ab020f2,0xf3b97148,0x84be41de,
    0x1adad47d,0x6ddde4eb,0xf4d4b551,0x83d385c7,
    0x136c9856,0x646ba8c0,0xfd62f97a,0x8a65c9ec,
    0x14015c4f,0x63066cd9,0xfa0f3d63,0x8d080df5,
    0x3b6e20c8,0x4c69105e,0xd56041e4,0xa2677172,
    0x3c03e4d1,0x4b04d447,0xd20d85fd,0xa50ab56b,
    0x35b5a8fa,0x42b2986c,0xdbbbc9d6,0xacbcf940,
    0x32d86ce3,0x45df5c75,0xdcd60dcf,0xabd13d59,
    0x26d930ac,0x51de003a,0xc8d75180,0xbfd06116,
    0x21b4f4b5,0x56b3c423,0xcfba9599,0xb8bda50f,
    0x2802b89e,0x5f058808,0xc60cd9b2,0xb10be924,
    0x2f6f7c87,0x58684c11,0xc1611dab,0xb6662d3d,
    0x76dc4190,0x01db7106,0x98d220bc,0xefd5102a,
    0x71b18589,0x06b6b51f,0x9fbfe4a5,0xe8b8d433,
    0x7807c9a2,0x0f00f934,0x9609a88e,0xe10e9818,
    0x7f6a0dbb,0x086d3d2d,0x91646c97,0xe6635c01,
    0x6b6b51f4,0x1c6c6162,0x856530d8,0xf262004e,
    0x6c0695ed,0x1b01a57b,0x8208f4c1,0xf50fc457,
    0x65b0d9c6,0x12b7e950,0x8bbeb8ea,0xfcb9887c,
    0x62dd1ddf,0x15da2d49,0x8cd37cf3,0xfbd44c65,
    0x4db26158,0x3ab551ce,0xa3bc0074,0xd4bb30e2,
    0x4adfa541,0x3dd895d7,0xa4d1c46d,0xd3d6f4fb,
    0x4369e96a,0x346ed9fc,0xad678846,0xda60b8d0,
    0x44042d73,0x33031de5,0xaa0a4c5f,0xdd0d7cc9,
    0x5005713c,0x270241aa,0xbe0b1010,0xc90c2086,
    0x5768b525,0x206f85b3,0xb966d409,0xce61e49f,
    0x5edef90e,0x29d9c998,0xb0d09822,0xc7d7a8b4,
    0x59b33d17,0x2eb40d81,0xb7bd5c3b,0xc0ba6cad,
    0xedb88320,0x9abfb3b6,0x03b6e20c,0x74b1d29a,
    0xead54739,0x9dd277af,0x04db2615,0x73dc1683,
    0xe3630b12,0x94643b84,0x0d6d6a3e,0x7a6a5aa8,
    0xe40ecf0b,0x9309ff9d,0x0a00ae27,0x7d079eb1,
    0xf00f9344,0x8708a3d2,0x1e01f268,0x6906c2fe,
    0xf762575d,0x806567cb,0x196c3671,0x6e6b06e7,
    0xfed41b76,0x89d32be0,0x10da7a5a,0x67dd4acc,
    0xf9b9df6f,0x8ebeeff9,0x17b7be43,0x60b08ed5,
    0xd6d6a3e8,0xa1d1937e,0x38d8c2c4,0x4fdff252,
    0xd1bb67f1,0xa6bc5767,0x3fb506dd,0x48b2364b,
    0xd80d2bda,0xaf0a1b4c,0x36034af6,0x41047a60,
    0xdf60efc3,0xa867df55,0x316e8eef,0x4669be79,
    0xcb61b38c,0xbc66831a,0x256fd2a0,0x5268e236,
    0xcc0c7795,0xbb0b4703,0x220216b9,0x5505262f,
    0xc5ba3bbe,0xb2bd0b28,0x2bb45a92,0x5cb36a04,
    0xc2d7ffa7,0xb5d0cf31,0x2cd99e8b,0x5bdeae1d,
    0x9b64c2b0,0xec63f226,0x756aa39c,0x026d930a,
    0x9c0906a9,0xeb0e363f,0x72076785,0x05005713,
    0x95bf4a82,0xe2b87a14,0x7bb12bae,0x0cb61b38,
    0x92d28e9b,0xe5d5be0d,0x7cdcefb7,0x0bdbdf21,
    0x86d3d2d4,0xf1d4e242,0x68ddb3f8,0x1fda836e,
    0x81be16cd,0xf6b9265b,0x6fb077e1,0x18b74777,
    0x88085ae6,0xff0f6a70,0x66063bca,0x11010b5c,
    0x8f659eff,0xf862ae69,0x616bffd3,0x166ccf45,
    0xa00ae278,0xd70dd2ee,0x4e048354,0x3903b3c2,
    0xa7672661,0xd06016f7,0x4969474d,0x3e6e77db,
    0xaed16a4a,0xd9d65adc,0x40df0b66,0x37d83bf0,
    0xa9bcae53,0xdebb9ec5,0x47b2cf7f,0x30b5ffe9,
    0xbdbdf21c,0xcabac28a,0x53b39330,0x24b4a3a6,
    0xbad03605,0xcdd70693,0x54de5729,0x23d967bf,
    0xb3667a2e,0xc4614ab8,0x5d681b02,0x2a6f2b94,
    0xb40bbe37,0xc30c8ea1,0x5a05df1b,0x2d02ef8d,
};

void QuickCrc32(BYTE *pbyDataIn, DWORD dwDataLen, BYTE abyCrcOut[4])
{
    DWORD dwCrc;
    DWORD i;
    dwCrc = 0xffffffff;
    for (i = 0; i < dwDataLen; i++)
    {
        dwCrc = gc_dwCrc32Table[((BYTE)dwCrc^pbyDataIn[i])&0xff] ^ (dwCrc>>8);
    }
    dwCrc = dwCrc^0xffffffff;
    abyCrcOut[0] = (BYTE )(dwCrc>>24);
    abyCrcOut[1] = (BYTE )(dwCrc>>16);
    abyCrcOut[2] = (BYTE )(dwCrc>>8);
    abyCrcOut[3] = (BYTE )(dwCrc);
}
void PortSend(unsigned char ch)
{
    DLL_SerialSend(&ch,1);
}
int PortRecv(unsigned char * ch,WORD ms)
{
	unsigned short len;
    DLL_SerialRecv(ch,&len,ms,1);
	if(len==1) 
		return 0;
	return 1;
}

int RecvPack(BYTE *pbyRecvBuff, DWORD *pdwRecvLen)
{
    DWORD i;
    BYTE abyCrc32[5];
    //BYTE ch;

    while (1)
    {
        if (0 != PortRecv(pbyRecvBuff,10000))//10000
        {
            return 1;
        }
        if (0x02 == pbyRecvBuff[0])
        {
            break;
        }
    }

    for (i=1; i<4; i++)
    {
        if (0 != PortRecv(pbyRecvBuff+i,10000))
        {
            return 2;
        }
    }
    *pdwRecvLen = ((DWORD)pbyRecvBuff[2])*256 + (DWORD)pbyRecvBuff[3];
    if (*pdwRecvLen > 1024)
    {
        return 5;
    }

    for (i=0; i<*pdwRecvLen+4; i++)
    {
        if(0 != PortRecv(pbyRecvBuff+i+4,10000))
        {
            return 4;
        }
    }

    QuickCrc32(&pbyRecvBuff[1], *pdwRecvLen+3, abyCrc32);
    if (0 != memcmp(&pbyRecvBuff[*pdwRecvLen+4],abyCrc32,4))
    {
        return 3;
    }
    return 0;
}

int SendPack(BYTE cmd, BYTE *SendBuff, int SndLen)
{
    //int i;
	unsigned char g_pbySendBuff[530];

    g_pbySendBuff[0] = 0x02;
    g_pbySendBuff[1] = cmd;
    g_pbySendBuff[2] = (BYTE)(SndLen/256);
    g_pbySendBuff[3] = (BYTE)(SndLen%256);
    memcpy(&g_pbySendBuff[4], SendBuff, SndLen);
	
    QuickCrc32(&g_pbySendBuff[1], SndLen+3, &g_pbySendBuff[SndLen+4]);
    SndLen += 8;
	
//	Lib_UartSend(COMMAND_COM,g_pbySendBuff,SndLen);
    DLL_SerialSend(g_pbySendBuff,SndLen);

	/*
    for (i=0; i<SndLen; i++)
    {
        (void)PortSend(g_pbySendBuff[i]);
    }*/
    return 0;
}


