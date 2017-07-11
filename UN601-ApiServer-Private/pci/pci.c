#include <stdio.h>
#include<string.h>
#include "msgqueue.h"
#include <stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "user.h"
#include "Dll_api.h"

extern unsigned char GetPin_Flag;

BYTE CurAppNo = 0;

#define MAX_APP_NUM   10
#define mCurrAppNoFilePath "/home/root/masterapp/CurrAppNo.dat"
int readCurrAppNo(unsigned char *byAppNo)
{
    if(0 != access(mCurrAppNoFilePath,F_OK))
    {
        return -1;
    }

    int fd = open(mCurrAppNoFilePath,O_RDONLY);
    if(fd<0) return -2;
    lseek(fd,0,SEEK_SET);

    int iRet = -1;
   iRet = read(fd,byAppNo, 1);
   close(fd);
    if(1 != iRet)
    {
        return -3;
    }

    return 0;
}

int DLL_PciGetRnd(BYTE *rnd)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PCI))
    		return App_Permissions_Denial;

	wLen = 0;
	wCmd = MAKEWORD(PCI_RND_MODULE,PCI_GET_RND);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	if(0==iRet) memcpy(rnd,g_abyRecvBuff+7,wLen-2);
	
	Crc16CloseCom();
	return iRet;	
}

int DLL_PciWriteMKey(BYTE keyType, BYTE keyNo, BYTE keyLen, BYTE *keyData, BYTE mode)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));


    	if(authCheck(CurAppNo, MODULE_TYPE_PCI))
    		return App_Permissions_Denial;

	if(CurAppNo==0xff) 
	{
		iRet = readCurrAppNo(&CurAppNo);
		printf("readCurrAppNo is %d\r\n",iRet);
		printf("the cur app no is %02x\r\n",CurAppNo);
		if(iRet !=0) return PCI_AppNumOver_Err;
	}


	 g_abySendBuff[0] = CurAppNo;
	 g_abySendBuff[1] = keyType;
        g_abySendBuff[2] = keyNo;
        g_abySendBuff[3] = keyLen;	
        g_abySendBuff[4] = mode;	       
	 memcpy(g_abySendBuff+5,keyData, keyLen);

	wLen = 5+keyLen;
	wCmd = MAKEWORD(PCI_KEY_LOAD_MODULE,PCI_KEY_LOAD_MASTER);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	
	Crc16CloseCom();
	return iRet;	
}

int DLL_PciWriteDukptKey(BYTE keyType, BYTE keyNo, BYTE bdkLen, BYTE *bdk, BYTE ksnLen, BYTE *ksn, BYTE mode)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PCI))
    		return App_Permissions_Denial;

	if(CurAppNo==0xff) 
	{
		iRet = readCurrAppNo(&CurAppNo);
		printf("readCurrAppNo is %d\r\n",iRet);
		printf("the cur app no is %02x\r\n",CurAppNo);
		if(iRet !=0) return PCI_AppNumOver_Err;
	}

        g_abySendBuff[0] = CurAppNo; 
        g_abySendBuff[1] = keyType;
        g_abySendBuff[2] = keyNo;
        g_abySendBuff[3] = bdkLen;
        g_abySendBuff[4] = mode;	        
        memcpy(g_abySendBuff+5,bdk,bdkLen);
        g_abySendBuff[5+bdkLen] = ksnLen;
        memcpy(g_abySendBuff+5+bdkLen+1,ksn,ksnLen);

	wLen = bdkLen+ksnLen+6;
	wCmd = MAKEWORD(PCI_KEY_LOAD_MODULE,PCI_KEY_LOAD_DUKPTK);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT*2);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	
	Crc16CloseCom();
	return iRet;	
}

int DLL_PciWriteSKey(BYTE keyType, BYTE keyNo, BYTE keyLen, BYTE *keyData, BYTE mode, BYTE mkeyNo)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PCI))
    		return App_Permissions_Denial;

	if(CurAppNo==0xff) 
	{
		iRet = readCurrAppNo(&CurAppNo);
		printf("readCurrAppNo is %d\r\n",iRet);
		printf("the cur app no is %02x\r\n",CurAppNo);
		if(iRet !=0) return PCI_AppNumOver_Err;
	}

        g_abySendBuff[0] = CurAppNo; 
        g_abySendBuff[1] = keyType;
        g_abySendBuff[2] = keyNo;
        g_abySendBuff[3] = keyLen;
        g_abySendBuff[4] = mkeyNo;
        g_abySendBuff[5] = mode;	        
        memcpy(g_abySendBuff+6,keyData, keyLen);
	        		
	wLen = keyLen+6;
	wCmd = MAKEWORD(PCI_KEY_LOAD_MODULE,PCI_KEY_LOAD_SESSION);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	
	Crc16CloseCom();
	return iRet;	
}


int pinmsgid = -1;
msg_t msgToPinEntry;  
unsigned char msgBuf[1024];
int msgLen = 0;

int sendPinEntryPromptMsg(int type, int subtype, unsigned char * amount, PinContext *ctx)
{
	int i =0;
	//创建消息队列  
	pinmsgid = msgget((key_t)128L, 0666 | IPC_CREAT);  
	printf("pinmsgid is %d\r\n",pinmsgid);
	if (pinmsgid < 0)  
	{  
	    puts("Create msg queue error!\n") ;  
	    exit(0) ;  
	}  

	memset(msgBuf,0,sizeof(msgBuf));
	msgBuf[0] = type;
	msgBuf[1] = subtype;
	if(subtype == 0)
	{
		memcpy(msgBuf+2,amount,14);
		memcpy(msgBuf+2+14,ctx->prompt,strlen(ctx->prompt));
		memcpy(msgBuf+2+14+strlen(ctx->prompt),ctx->font,4);
		memcpy(msgBuf+2+14+strlen(ctx->prompt)+4,ctx->background,3);
		msgLen = 2+14+strlen(ctx->prompt)+4+3;
	}
	else msgLen = 2;
  
	//向消息队列中写消息 
	msgToPinEntry.mtype= 3;    
	memcpy(msgToPinEntry.mdata, msgBuf,msgLen);  

	printf("send data to PinEntry:\r\n");
	for( i = 0 ; i < msgLen; i++) printf("%02x ",msgBuf[i]);
	printf("\r\n");
	//向队列发送数据  
	if(msgsnd(pinmsgid, (void*)&msgToPinEntry, msgLen, 0) == -1)  
	{  
	    	fprintf(stderr, "msgsnd failed\n");  
	 	return Msgqueue_Send_Err;
	}  
	return 0;

}

int sendPinEntryMsg(int type,int subtype,int pinLen,int count)
{
	int i = 0;
	//创建消息队列  
	pinmsgid = msgget((key_t)128L, 0666 | IPC_CREAT);  
	printf("pinmsgid is %d\r\n",pinmsgid);
	if (pinmsgid < 0)  
	{  
	    puts("Create msg queue error!\n") ;  
	    exit(0) ;  
	}  

	memset(msgBuf,0,sizeof(msgBuf));
	msgBuf[0] = type;
	msgBuf[1] = subtype;
	msgBuf[2] = pinLen;
	msgBuf[3] = count;
	msgLen = 4;
  
	//向消息队列中写消息 
	msgToPinEntry.mtype= 3;    
	memcpy(msgToPinEntry.mdata, msgBuf,msgLen);  

	printf("send data:\r\n");
	for( i = 0 ; i < msgLen; i++) printf("%02x ",msgBuf[i]);
	printf("\r\n");
	//向队列发送数据  
	if(msgsnd(pinmsgid, (void*)&msgToPinEntry, msgLen, 0) == -1)  
	{  
	    	fprintf(stderr, "msgsnd failed\n");  
	 	return Msgqueue_Send_Err;
	}  
	return 0;

}

int DLL_PciGetPin(BYTE keyNo,BYTE minLen,BYTE maxLen,BYTE cardNoLen, BYTE *cardNo,BYTE mode,BYTE *pinBlock ,BYTE mark,BYTE *amount,BYTE waitTimeSec,PinContext *ctx)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;
       int pinEntryStatus = 0;
	BYTE recvData[1];
	int iPinMaxLenCount = 0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PCI))
    		return App_Permissions_Denial;
	if(CurAppNo==0xff) 
	{
		iRet = readCurrAppNo(&CurAppNo);
		printf("readCurrAppNo is %d\r\n",iRet);
		printf("the cur app no is %02x\r\n",CurAppNo);
		if(iRet !=0) return PCI_AppNumOver_Err;
	}
	        /////
        g_abySendBuff[0] = CurAppNo;
        g_abySendBuff[1] = keyNo; 
        g_abySendBuff[2] = minLen;
        g_abySendBuff[3] = maxLen;
        g_abySendBuff[4] = cardNoLen;
        memcpy(g_abySendBuff+5,cardNo,cardNoLen);
        g_abySendBuff[5+cardNoLen] = mode;
        g_abySendBuff[6+cardNoLen] = waitTimeSec;


	wLen = cardNoLen+7;
	wCmd = MAKEWORD(PCI_KEY_USE_MODULE,PCI_PIN_ENC_PINK);
	
	GetPin_Flag = 1;
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}


//////
	while(1)
        {
        	iRet = RecvOneByte(recvData, 100);
		printf("RecvOneByte is :%d\r\n",iRet);
 		printf("recvData is :%02x\r\n",recvData[0]);
       	if(0==iRet && recvData[0] == 0x3c)
        	{
        		pinEntryStatus = 0;
			GetPin_Flag = 0;
        		break;
        	}
        	if(0==iRet && recvData[0] == 0x3E)
        	{	               
        		pinEntryStatus = 1;
        		break;
        	}
        }
        
        if(pinEntryStatus==0)
        {
	        if(Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT*2)==0) {
			iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	            	if(0 != iRet) {Crc16CloseCom(); return iRet;}
	        }
	        else 
	        {
			Crc16CloseCom();
			return Packet_Recv_Err;
	        }
        }

	sendPinEntryPromptMsg(1,0,amount,ctx);

        while(1)
        {
        	iRet = RecvOneByte(recvData, 100);
        	if(0==iRet && recvData[0] == 0x3b)
        	{
        		iPinMaxLenCount = 0;
    	        	sendPinEntryMsg(1,7,100,0);
 			GetPin_Flag = 0;
       		break;
        	}
        	if(0==iRet && recvData[0] == 0x00)
        	{    		
        		iPinMaxLenCount = 0;
    	        	sendPinEntryMsg(1,7,0,0);	               
        		continue;
        	}
        	if(0==iRet && (recvData[0] <= maxLen && recvData[0] >=1))
        	{
    	        if(recvData[0] == maxLen) iPinMaxLenCount++;
    	        if(iPinMaxLenCount < 2) sendPinEntryMsg(1,7,recvData[0],0);
        		continue;
        	}
        }        	

//////

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	if(0 == iRet) memcpy(pinBlock,g_abyRecvBuff+7,wLen-2);
	
	Crc16CloseCom();
	return iRet;	
}

int DLL_PciGetMac(BYTE keyNo, int inLen, BYTE *inData, BYTE *macOut, BYTE mode)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PCI))
    		return App_Permissions_Denial;
	if(CurAppNo==0xff) 
	{
		iRet = readCurrAppNo(&CurAppNo);
		printf("readCurrAppNo is %d\r\n",iRet);
		printf("the cur app no is %02x\r\n",CurAppNo);
		if(iRet !=0) return PCI_AppNumOver_Err;
	}
	
        g_abySendBuff[0] = CurAppNo;
        g_abySendBuff[1] = keyNo; 
        g_abySendBuff[2] = (inLen/256);
        g_abySendBuff[3] = (inLen%256);
        if(inLen > 1024) return PCI_DataLen_Err;
        memcpy(g_abySendBuff+4,inData, inLen);
        g_abySendBuff[4+inLen] = mode;

	wLen = inLen+5;
	wCmd = MAKEWORD(PCI_KEY_USE_MODULE,PCI_MAC_ENC_MACK);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	if(0==iRet) memcpy(macOut,g_abyRecvBuff+7,wLen-2);
	
	Crc16CloseCom();
	return iRet;
}

int DLL_PciGetDes(BYTE keyNo, int inLen, BYTE *inData, BYTE *desOut, BYTE mode)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PCI))
    		return App_Permissions_Denial;
	if(CurAppNo==0xff) 
	{
		iRet = readCurrAppNo(&CurAppNo);
		printf("readCurrAppNo is %d\r\n",iRet);
		printf("the cur app no is %02x\r\n",CurAppNo);
		if(iRet !=0) return PCI_AppNumOver_Err;
	}

        g_abySendBuff[0] = CurAppNo;
        g_abySendBuff[1] = keyNo; 
        g_abySendBuff[2] = (inLen/256);
        g_abySendBuff[3] = (inLen%256);
        if(inLen > 1024) return PCI_DataLen_Err;
        memcpy(g_abySendBuff+4,inData, inLen);
        g_abySendBuff[4+inLen] = mode;
	
	wLen = inLen+5;
	wCmd = MAKEWORD(PCI_KEY_USE_MODULE,PCI_DES_ENC_DESK);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	if(0==iRet) memcpy(desOut,g_abyRecvBuff+7,wLen-2);
	
	Crc16CloseCom();
	return iRet;
}

int DLL_PciGetPinDukpt( BYTE keyNo,BYTE minLen, BYTE maxLen, BYTE cardNoLen, BYTE *cardNo,BYTE mode,BYTE *pinBlock,BYTE mark, BYTE *amount, BYTE waitTimeSec,BYTE *outKsn,PinContext *ctx)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;
       int pinEntryStatus = 0;
	BYTE recvData[1];
	int iPinMaxLenCount = 0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PCI))
    		return App_Permissions_Denial;
	if(CurAppNo==0xff) 
	{
		iRet = readCurrAppNo(&CurAppNo);
		printf("readCurrAppNo is %d\r\n",iRet);
		printf("the cur app no is %02x\r\n",CurAppNo);
		if(iRet !=0) return PCI_AppNumOver_Err;
	}

       g_abySendBuff[0] = CurAppNo;
        g_abySendBuff[1] = keyNo; 
        g_abySendBuff[2] = minLen;
        g_abySendBuff[3] = maxLen;
        g_abySendBuff[4] = cardNoLen;
        memcpy(g_abySendBuff+5,cardNo,cardNoLen);
        g_abySendBuff[5+cardNoLen] = mode;
        g_abySendBuff[6+cardNoLen] = waitTimeSec;

	
	wLen = cardNoLen+7;
	wCmd = MAKEWORD(PCI_KEY_USE_MODULE,PCI_PIN_ENC_DUKPT);
	
	GetPin_Flag = 1;
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}
///////


	while(1)
        {
        	iRet = RecvOneByte(recvData, 100);
		printf("RecvOneByte is :%d\r\n",iRet);
 		printf("recvData is :%02x\r\n",recvData[0]);
       	if(0==iRet && recvData[0] == 0x3c)
        	{
        		pinEntryStatus = 0;
 			GetPin_Flag = 0;
       		break;
        	}
        	if(0==iRet && recvData[0] == 0x3E)
        	{	               
        		pinEntryStatus = 1;
        		break;
        	}
        }
        
        if(pinEntryStatus==0)
        {
	        if(Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT*2)==0) {
			iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	            	if(0 != iRet) {Crc16CloseCom(); return iRet;}
	        }
	        else 
	        {
			Crc16CloseCom();
			return Packet_Recv_Err;
	        }
        }

	sendPinEntryPromptMsg(1,0,amount,ctx);

        while(1)
        {
        	iRet = RecvOneByte(recvData, 100);
        	if(0==iRet && recvData[0] == 0x3b)
        	{
        		iPinMaxLenCount = 0;
    	        	sendPinEntryMsg(1,7,100,0);
 			GetPin_Flag = 0;
       		break;
        	}
        	if(0==iRet && recvData[0] == 0x00)
        	{    		
        		iPinMaxLenCount = 0;
    	        	sendPinEntryMsg(1,7,0,0);	               
        		continue;
        	}
        	if(0==iRet && (recvData[0] <= maxLen && recvData[0] >=1))
        	{
    	        if(recvData[0] == maxLen) iPinMaxLenCount++;
    	        if(iPinMaxLenCount < 2) sendPinEntryMsg(1,7,recvData[0],0);
        		continue;
        	}
        }        	


///////
	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	if(0==iRet) 
	{
		memcpy(pinBlock,g_abyRecvBuff+7,8);
		memcpy(outKsn,g_abyRecvBuff+7+8,10);
	}
	
	Crc16CloseCom();
	return iRet;
}

int DLL_PciGetMacDukpt( BYTE keyNo, int inLen, BYTE *inData, BYTE *macOut, BYTE mode,BYTE *outKsn)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

    	if(authCheck(CurAppNo, MODULE_TYPE_PCI))
    		return App_Permissions_Denial;
	if(CurAppNo==0xff) 
	{
		iRet = readCurrAppNo(&CurAppNo);
		printf("readCurrAppNo is %d\r\n",iRet);
		printf("the cur app no is %02x\r\n",CurAppNo);
		if(iRet !=0) return PCI_AppNumOver_Err;
	}
	
        g_abySendBuff[0] = CurAppNo;
        g_abySendBuff[1] = keyNo; 
        g_abySendBuff[2] = (inLen/256);
        g_abySendBuff[3] = (inLen%256);
        if(inLen > 1024) return PCI_DataLen_Err;
        memcpy(g_abySendBuff+4,inData,inLen);
        g_abySendBuff[4+inLen] = mode;
	
	wLen = inLen+5;
	wCmd = MAKEWORD(PCI_KEY_USE_MODULE,PCI_MAC_ENC_DUKPT);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	if(0==iRet) 
	{
		memcpy(macOut,g_abyRecvBuff+7,8);
		memcpy(outKsn,g_abyRecvBuff+7+8,10);
	}
	
	Crc16CloseCom();
	return iRet;
}

int DLL_PciGetDesDukpt(BYTE keyNo, int inLen, BYTE *inData, BYTE *desOut, BYTE mode, BYTE *outKsn)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

    	if(authCheck(CurAppNo, MODULE_TYPE_PCI))
    		return App_Permissions_Denial;
	if(CurAppNo==0xff) 
	{
		iRet = readCurrAppNo(&CurAppNo);
		printf("readCurrAppNo is %d\r\n",iRet);
		printf("the cur app no is %02x\r\n",CurAppNo);
		if(iRet !=0) return PCI_AppNumOver_Err;
	}
	
        g_abySendBuff[0] = CurAppNo;
        g_abySendBuff[1] = keyNo; 
        g_abySendBuff[2] = (inLen/256);
        g_abySendBuff[3] = (inLen%256);
        if(inLen > 1024) return PCI_DataLen_Err;
        memcpy(g_abySendBuff+4,inData,inLen);
        g_abySendBuff[4+inLen] = mode;
	
	wLen = inLen+5;
	wCmd = MAKEWORD(PCI_KEY_USE_MODULE,PCI_DES_ENC_DUKPT);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	if(0==iRet) 
	{
		memcpy(desOut,g_abyRecvBuff+7,wLen-10-2);
		memcpy(outKsn,g_abyRecvBuff+7+wLen-10-2,10);
	}
	
	Crc16CloseCom();
	return iRet;
}

int DLL_PciGetPinByFixKey(BYTE keyNo,BYTE minLen,BYTE maxLen,BYTE cardNoLen, BYTE *cardNo,BYTE mode,BYTE *pinBlock ,BYTE mark,BYTE *amount,BYTE waitTimeSec,PinContext *ctx)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;
       int pinEntryStatus = 0;
	BYTE recvData[1];
	int iPinMaxLenCount = 0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

    	if(authCheck(CurAppNo, MODULE_TYPE_PCI))
    		return App_Permissions_Denial;
	if(CurAppNo==0xff) 
	{
		iRet = readCurrAppNo(&CurAppNo);
		printf("readCurrAppNo is %d\r\n",iRet);
		printf("the cur app no is %02x\r\n",CurAppNo);
		if(iRet !=0) return PCI_AppNumOver_Err;
	}
	
        g_abySendBuff[0] = CurAppNo;
        g_abySendBuff[1] = keyNo; 
        g_abySendBuff[2] = minLen;
        g_abySendBuff[3] = maxLen;
        g_abySendBuff[4] = cardNoLen;
        memcpy(g_abySendBuff+5,cardNo,cardNoLen);
        g_abySendBuff[5+cardNoLen] = mode;
        g_abySendBuff[6+cardNoLen] = waitTimeSec;

	
	wLen = cardNoLen+7;
	wCmd = MAKEWORD(PCI_KEY_USE_MODULE,PCI_PIN_ENC_FIXK);
	
	GetPin_Flag = 1;
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

//////
	while(1)
        {
        	iRet = RecvOneByte(recvData, 100);
		printf("RecvOneByte is :%d\r\n",iRet);
 		printf("recvData is :%02x\r\n",recvData[0]);
       	if(0==iRet && recvData[0] == 0x3c)
        	{
        		pinEntryStatus = 0;
			GetPin_Flag = 0;
        		break;
        	}
        	if(0==iRet && recvData[0] == 0x3E)
        	{	               
        		pinEntryStatus = 1;
        		break;
        	}
        }
        
        if(pinEntryStatus==0)
        {
	        if(Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT*2)==0) {
			iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	            	if(0 != iRet) {Crc16CloseCom(); return iRet;}
	        }
	        else 
	        {
			Crc16CloseCom();
			return Packet_Recv_Err;
	        }
        }

	sendPinEntryPromptMsg(1,0,amount,ctx);

        while(1)
        {
        	iRet = RecvOneByte(recvData, 100);
        	if(0==iRet && recvData[0] == 0x3b)
        	{
        		iPinMaxLenCount = 0;
    	        	sendPinEntryMsg(1,7,100,0);
 			GetPin_Flag = 0;
       		break;
        	}
        	if(0==iRet && recvData[0] == 0x00)
        	{    		
        		iPinMaxLenCount = 0;
    	        	sendPinEntryMsg(1,7,0,0);	               
        		continue;
        	}
        	if(0==iRet && (recvData[0] <= maxLen && recvData[0] >=1))
        	{
    	        if(recvData[0] == maxLen) iPinMaxLenCount++;
    	        if(iPinMaxLenCount < 2) sendPinEntryMsg(1,7,recvData[0],0);
        		continue;
        	}
        }        	

//////

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	if(0==iRet) 
	{
		memcpy(pinBlock,g_abyRecvBuff+7,wLen-2);
	}
	
	Crc16CloseCom();
	return iRet;
}
		
int DLL_PciOffLineEncPin(BYTE slot, BYTE minLen, BYTE maxLen, BYTE waitTimeSec, PUBLIC_KEY *pk, PinContext *ctx)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;
       int pinEntryStatus = 0;
	BYTE recvData[1];
	int iPinMaxLenCount = 0;
	unsigned char amount[15];

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

    	if(authCheck(CurAppNo, MODULE_TYPE_PCI))
    		return App_Permissions_Denial;
	
	if(CurAppNo==0xff) 
	{
		iRet = readCurrAppNo(&CurAppNo);
		printf("readCurrAppNo is %d\r\n",iRet);
		printf("the cur app no is %02x\r\n",CurAppNo);
		if(iRet !=0) return PCI_AppNumOver_Err;
	}
	
        g_abySendBuff[0] = CurAppNo;
        g_abySendBuff[1] = slot; 
        g_abySendBuff[2] = minLen;
        g_abySendBuff[3] = maxLen;
        g_abySendBuff[4] = waitTimeSec;
	 g_abySendBuff[5] = pk->ModulLen;
        memcpy(g_abySendBuff+6,pk->Modul,pk->ModulLen);
	 g_abySendBuff[6+248]=pk->ExponentLen;
	 memcpy(g_abySendBuff+6+248+1,pk->Exponent,pk->ExponentLen);
	
	wLen = sizeof(PUBLIC_KEY)+6;
	wCmd = MAKEWORD(PCI_KEY_USE_MODULE,PCI_OFFLINE_ENC_VER);
	
	GetPin_Flag = 1;
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

//////
	while(1)
        {
        	iRet = RecvOneByte(recvData, 100);
       	if(0==iRet && recvData[0] == 0x3c)
        	{
        		pinEntryStatus = 0;
 			GetPin_Flag = 0;
       		break;
        	}
        	if(0==iRet && recvData[0] == 0x3E)
        	{	               
        		pinEntryStatus = 1;
        		break;
        	}
        }
        
        if(pinEntryStatus==0)
        {
	        if(Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT*2)==0) {
			iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	            	if(0 != iRet) {Crc16CloseCom(); return iRet;}
	        }
	        else 
	        {
			Crc16CloseCom();
			return Packet_Recv_Err;
	        }
        }

	memset(amount,0,sizeof(amount));
	sendPinEntryPromptMsg(1,0,amount,ctx);

        while(1)
        {
        	iRet = RecvOneByte(recvData, 100);
        	if(0==iRet && recvData[0] == 0x3b)
        	{
        		iPinMaxLenCount = 0;
    	        	sendPinEntryMsg(1,7,100,0);
 			GetPin_Flag = 0;
       		break;
        	}
        	if(0==iRet && recvData[0] == 0x00)
        	{    		
        		iPinMaxLenCount = 0;
    	        	sendPinEntryMsg(1,7,0,0);	               
        		continue;
        	}
        	if(0==iRet && (recvData[0] <= maxLen && recvData[0] >=1))
        	{
    	        if(recvData[0] == maxLen) iPinMaxLenCount++;
    	        if(iPinMaxLenCount < 2) sendPinEntryMsg(1,7,recvData[0],0);
        		continue;
        	}
        }        	

//////
	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
    	if(iRet >= 256) iRet = 0-(iRet-256);
	
	Crc16CloseCom();
	return iRet;
}

int DLL_PciOffLinePlainPin(BYTE slot, BYTE minLen, BYTE maxLen, BYTE waitTimeSec, PinContext *ctx)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;
       int pinEntryStatus = 0;
	BYTE recvData[1];
	int iPinMaxLenCount = 0;
	unsigned char amount[15];

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

    	if(authCheck(CurAppNo, MODULE_TYPE_PCI))
    		return App_Permissions_Denial;
	if(CurAppNo==0xff) 
	{
		iRet = readCurrAppNo(&CurAppNo);
		printf("readCurrAppNo is %d\r\n",iRet);
		printf("the cur app no is %02x\r\n",CurAppNo);
		if(iRet !=0) return PCI_AppNumOver_Err;
	}
	
        g_abySendBuff[0] = CurAppNo;
        g_abySendBuff[1] = slot; 
        g_abySendBuff[2] = minLen;
        g_abySendBuff[3] = maxLen;
        g_abySendBuff[4] = waitTimeSec;

	
	wLen = 5;
	wCmd = MAKEWORD(PCI_KEY_USE_MODULE,PCI_OFFLINE_CLR_VER);
	
	GetPin_Flag = 1;
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

//////
	while(1)
        {
        	iRet = RecvOneByte(recvData, 100);
       	if(0==iRet && recvData[0] == 0x3c)
        	{
        		pinEntryStatus = 0;
 			GetPin_Flag = 0;
       		break;
        	}
        	if(0==iRet && recvData[0] == 0x3E)
        	{	               
        		pinEntryStatus = 1;
        		break;
        	}
        }
        
        if(pinEntryStatus==0)
        {
	        if(Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT*2)==0) {
			iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	            	if(0 != iRet) {Crc16CloseCom(); return iRet;}
	        }
	        else 
	        {
			Crc16CloseCom();
			return Packet_Recv_Err;
	        }
        }

	memset(amount,0,sizeof(amount));
	sendPinEntryPromptMsg(1,0,amount,ctx);

        while(1)
        {
        	iRet = RecvOneByte(recvData, 100);
        	if(0==iRet && recvData[0] == 0x3b)
        	{
        		iPinMaxLenCount = 0;
    	        	sendPinEntryMsg(1,7,100,0);
			GetPin_Flag = 0;
        		break;
        	}
        	if(0==iRet && recvData[0] == 0x00)
        	{    		
        		iPinMaxLenCount = 0;
    	        	sendPinEntryMsg(1,7,0,0);	               
        		continue;
        	}
        	if(0==iRet && (recvData[0] <= maxLen && recvData[0] >=1))
        	{
    	        if(recvData[0] == maxLen) iPinMaxLenCount++;
    	        if(iPinMaxLenCount < 2) sendPinEntryMsg(1,7,recvData[0],0);
        		continue;
        	}
        }        	

//////
	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
    	if(iRet >= 256) iRet = 0-(iRet-256);
	
	Crc16CloseCom();
	return iRet;
}
		
int DLL_PciWriteSignKey(BYTE fwrType, BYTE *keyData, BYTE *signData)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

	if(CurAppNo != 0xff) return App_Permissions_Denial;
	
        g_abySendBuff[0] = fwrType; 
        memcpy(g_abySendBuff+1,keyData, 264);
        memcpy(g_abySendBuff+1+264,signData, 256);

	wLen = 521;
	wCmd = MAKEWORD(PCI_KEY_LOAD_MODULE,PCI_KEY_LOAD_SIGN);

	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}
	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);

	Crc16CloseCom();
	return iRet;
}
		
int DLL_PciVerifySignature(BYTE fwrType, BYTE *verifyData)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

	if(CurAppNo != 0xff) return App_Permissions_Denial;
	
        g_abySendBuff[0] = fwrType; 
        memcpy(g_abySendBuff+1,verifyData,288);

	wLen = 289;
	wCmd = MAKEWORD(PCI_VERIFY_MODULE,PCI_VERIFY_SIGN);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}
	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
		
	Crc16CloseCom();
	return iRet;
}
			
int DLL_PciCheckSecurityChipStatus()
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

	if(CurAppNo != 0xff) return App_Permissions_Denial;
	
	wLen = 0;
	wCmd = MAKEWORD(PCI_VERIFY_MODULE,PCI_CHECK_STATUS);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	
	Crc16CloseCom();
	return iRet;
}


int DLL_PciPwdInput(BYTE pwdType, BYTE mode)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;
       unsigned char recvData[1];
       int count = 0;
       int pwdEntryStatus = 0;
	unsigned char amount[15];

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

	if(CurAppNo != 0xff) return App_Permissions_Denial;
	
        g_abySendBuff[0] = pwdType;
        g_abySendBuff[1] = mode;

	wLen = 2;
	wCmd = MAKEWORD(PCI_VERIFY_MODULE,PCI_PWD_INPUT);
	
	GetPin_Flag = 1;
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}
////////
	while(1)
        {
        	iRet = RecvOneByte(recvData, 100);
		printf("RecvOneByte is %d\r\n",iRet);
        	if(0==iRet && recvData[0] == 0x3C)
        	{
			printf("RecvOneByte 3C\r\n");
        		pwdEntryStatus = 0;
			GetPin_Flag = 0;
        		break;
        	}
        	if(0==iRet && recvData[0] == 0x3E)
        	{	               
			printf("RecvOneByte 3E\r\n");
	      		pwdEntryStatus = 1;
        		break;
        	}
        }
        
        if(pwdEntryStatus==0)
        {
	        if(Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT*2)==0) {
	        	iRet = 0 -MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	            if(0 != iRet) {Crc16CloseCom(); return iRet;}
	        }
	        else 
	        {
			Crc16CloseCom();
			return Packet_Recv_Err;
	        }
        }

	memset(amount,0,sizeof(amount));
       
	if(pwdType == 0 && mode == 0)//input passwordA and verify
        	sendPinEntryPromptMsg(2,1,amount,NULL);//Plesae Enter Administrator A's Password:
        else if(pwdType == 0 && mode == 1)//input passwordA and save
        	sendPinEntryPromptMsg(2,2,amount,NULL);//Please Enter Administrator A's New password:
        else if(pwdType == 1 && mode == 0)//input passwordB and verify
        	sendPinEntryPromptMsg(2,3,amount,NULL);//Plesae Enter Administrator B's Password:
        else if(pwdType == 1 && mode == 1)//input passwordB and save
        	sendPinEntryPromptMsg(2,4,amount,NULL);//Please Enter Administrator B's New password:

	while(1)
       {
        	iRet = RecvOneByte(recvData, 100);
		if(0==iRet && recvData[0] == 0x3d)
		{
        		sendPinEntryMsg(2,8,100,1);
			GetPin_Flag = 0;
			break;
		}
        	if(0==iRet && recvData[0] == 0x3b)
        	{
        		count++;
    	        
	    	        if(mode==0)
	    	        {
	    	        	sendPinEntryMsg(2,8,100,count);
				GetPin_Flag = 0;
	    	        	break;
	    	        }
	    	        else if(mode==1 && count==1) 
	    	        {
	    	        	sendPinEntryMsg(2,8,100,0);
	    	        }
	    	        else if(mode==1 && count==2)
	    	        {
	    	        	sendPinEntryMsg(2,8,100,count);
				GetPin_Flag = 0;
	    	        	break;
	    	        }
        		else continue;
        	}
        	if(0==iRet && recvData[0] == 0x00)
        	{	        		
    	        	sendPinEntryMsg(2,8,0,count);	               
        		continue;
        	}
        	if(0==iRet && (recvData[0] <= 8 && recvData[0] >=1))
        	{
    	        	sendPinEntryMsg(2,8,recvData[0],count);
        		continue;
        	}
        }
///////
					
	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
    	if(iRet >= 256) iRet = 0-iRet;
	
	Crc16CloseCom();
	return iRet;
}
		

int DLL_PciJumpToOs()
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
	if(CurAppNo != 0xff) return App_Permissions_Denial;

	wLen = 0;
	wCmd = MAKEWORD(PCI_VERIFY_MODULE,PCI_JUMPTO_OS);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	
	Crc16CloseCom();
	return iRet;
}
		
int DLL_PciStartDownloadOs()
{
 	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
	if(CurAppNo != 0xff) return App_Permissions_Denial;

	wLen = 0;
	wCmd = MAKEWORD(PCI_VERIFY_MODULE,PCI_DOWNLOAD_OS);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	
	Crc16CloseCom();
	return iRet;
}

int DLL_PciClearAppkeys(BYTE appNo)
{
 	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

	if(CurAppNo != 0xff) return App_Permissions_Denial;
	
        g_abySendBuff[0] = appNo;

	wLen = 1;
	wCmd = MAKEWORD(PCI_KEY_USE_MODULE,PCI_CLEAR_APP_KEYS);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	
	Crc16CloseCom();
	return iRet;
}

int DLL_PciClearAllAppkeys()
{
 	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
	if(CurAppNo != 0xff) return App_Permissions_Denial;

	wLen = 0;
	wCmd = MAKEWORD(PCI_KEY_USE_MODULE,PCI_CLEAR_ALL_APPS_KEYS);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	
	Crc16CloseCom();
	return iRet;
}

int DLL_PciGetKeyCheckValue(BYTE appNo,BYTE keyType,BYTE keyNo, BYTE *checkValue)
{
 	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

	if(CurAppNo != 0xff) return App_Permissions_Denial;
	
        g_abySendBuff[0] = appNo;
        g_abySendBuff[1] = keyType;
        g_abySendBuff[2] = keyNo;

	wLen = 3;
	wCmd = MAKEWORD(PCI_KEY_USE_MODULE,PCI_GET_MKEY_CHECKVALUE);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
    	if(0 == iRet) memcpy(checkValue, g_abyRecvBuff+7,wLen-2);
	
	Crc16CloseCom();
	return iRet;
}

int DLL_PciGetSignKeyCheckValue(BYTE keyType, BYTE *checkValue)
{
 	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));

	if(CurAppNo != 0xff) return App_Permissions_Denial;
	
        g_abySendBuff[0] = keyType;

	wLen = 1;
	wCmd = MAKEWORD(PCI_KEY_USE_MODULE,PCI_GET_SKEY_CHECKVALUE);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
    	if(0 == iRet) memcpy(checkValue, g_abyRecvBuff+7,wLen-2);
	
	Crc16CloseCom();
	return iRet;
}

