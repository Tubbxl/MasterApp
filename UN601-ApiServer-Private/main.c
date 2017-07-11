#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include "dll_api.h"
#include "msgqueue.h"  
#include "user.h"
#include<pthread.h>
#include <linux/input.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <stdint.h>  
#include <sys/time.h>
#include <time.h>

int msgid = -1;  //消息队列id

#define RECV_PACKET_MAX_LEN 5050

#define DEBUG     1

BYTE recvBuf[MAX_SIZE];
BYTE sendBuf[MAX_SIZE];

int PciKeyLoadModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen);
int PciKeyUseModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen);
int PciVerifyModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen);
int PciRandModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen);
int SysModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen);
int IccModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen);
int MsrModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen);
int PiccModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen);
int PrinterModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen);
int ScanModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen);
int SerialModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen);
int ExternalSerialModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen);
int DownloadModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen);

void server(int, int) ;  

unsigned char GetPin_Flag = 0;

  
int   main(int argc, char** argv)  
{  

	pthread_t id;
	int i,ret;

	Crc16OpenComEx();

//  	DLL_PciJumpToOs();

  
    //创建消息队列  
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);  
    printf("msgid is %d\r\n",msgid);
    if (msgid < 0)  
    {  
        puts("Create msg queue error!\n") ;  
        exit(0) ;  
    }  

	//create thread to recv data from secure chip
	ret = pthread_create(&id, NULL, (void *)ComRecvThread,NULL);
	if(ret!=0){
	printf ("Create pthread error!\n");
	exit (1);
	}
	
    server(msgid, msgid) ;  
}  
  
void  server(int readfd, int writefd)  
{  
    msg_t* msgPtr ;  
    ssize_t  recvBytes ;  
    int iret = -1;
    unsigned short wLen = 0;
    int i = 0 ;
	
    for ( ; ; )  
    {  
        //从消息队列中读取来自客户的数据  
        msgPtr = malloc(MAX_SIZE + sizeof(long)) ;  
        recvBytes = msgrcv(readfd, msgPtr, MAX_SIZE, 1, 0) ; //阻塞读  
        printf("server recvBytes is %d\r\n",recvBytes);
        if (recvBytes <= 0)  
        {  
            puts("pathname missing") ;  
            continue ;  
        }  
        msgPtr->mdata[recvBytes] = '\0' ;  
  
        //分析消息，提取客户端数据 
  	memset(recvBuf,0,sizeof(recvBuf));  
	memcpy(recvBuf,msgPtr->mdata,recvBytes);
	printf("server recv data is:\r\n");
	for(i = 0 ; i < recvBytes; i++) printf("%02x ",recvBuf[i]);
	//printf("\r\n");
	
  	memset(sendBuf,0,sizeof(sendBuf));  
	
	switch(recvBuf[0])
	{
		case PCI_KEY_LOAD_MODULE:
		    iret = PciKeyLoadModuleFun(recvBuf+1,sendBuf+2,&wLen);
		    break;
		case PCI_KEY_USE_MODULE:
		    iret = PciKeyUseModuleFun(recvBuf+1,sendBuf+2,&wLen);
		    break;
		case PCI_VERIFY_MODULE:
		    iret = PciVerifyModuleFun(recvBuf+1,sendBuf+2,&wLen);
		    break;
		case PCI_RND_MODULE:
		    iret = PciRandModuleFun(recvBuf+1,sendBuf+2,&wLen);
		    break;
		case SYSTEM_MODULE:
		    iret = SysModuleFun(recvBuf+1,sendBuf+2,&wLen);
		    break;
		case ICC_MODULE:
		    iret = IccModuleFun(recvBuf+1,sendBuf+2,&wLen);
		    break;
		case MSR_MODULE:
		    iret = MsrModuleFun(recvBuf+1,sendBuf+2,&wLen);
		    break;
		case PICC_MODULE:
		    iret = PiccModuleFun(recvBuf+1,sendBuf+2,&wLen);
		    break;
		case PRINTER_MODULE:
		    iret = PrinterModuleFun(recvBuf+1,sendBuf+2,&wLen);
		    break;
		case SCAN_MODULE:
		    iret = ScanModuleFun(recvBuf+1,sendBuf+2,&wLen);
		    break;
		case SERIAL_MODULE:
		    iret = SerialModuleFun(recvBuf+1,sendBuf+2,&wLen);
		    break;
		case EXTERNAL_SERIAL_MODULE:
		    iret = ExternalSerialModuleFun(recvBuf+1,sendBuf+2,&wLen);
		    break;
		case DOWNLOAD_MODULE:
		    iret = DownloadModuleFun(recvBuf+1,sendBuf+2,&wLen);
		    break;
		case SIGNALTURE_MODULE:
		    iret = SiganltureModuleFun(recvBuf+1,sendBuf+2,&wLen);
		    break;
		default:
		    break;
	}

        msgPtr->mtype = 2 ; 
        sendBuf[0] = (ABS(iret))/256;
	 sendBuf[1] =(ABS(iret))%256;
	 memcpy(msgPtr->mdata,sendBuf,wLen+2);
	printf("server send data back is:\r\n");
	for(i = 0 ; i < wLen+2; i++) printf("%02x ",sendBuf[i]);
	printf("\r\n");

        if (msgsnd(writefd, msgPtr, wLen+2, IPC_NOWAIT) == -1)  
        {  
            puts("Send Error!");  
            exit(0);  
        }  
        
    }//for()  
}  


extern BYTE CurAppNo;

int PciKeyLoadModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen)
{
	int iRet = -1;
	unsigned char keyType, keyNo, keyLen, keyData[24], mode,mkeyNo;
	unsigned char bdk[16],ksn[10],bdkLen,ksnLen;
	
	switch(recvbuf[0])
	{
		case PCI_KEY_LOAD_SIGN:
			CurAppNo = recvbuf[3];
			iRet = DLL_PciWriteSignKey(recvbuf[4],recvbuf+5,recvbuf+5+264);
			*wLen = 0;
			break;
		case PCI_KEY_LOAD_MASTER:
			CurAppNo = recvbuf[3];
			keyType = recvbuf[4];
			keyNo = recvbuf[5];
			keyLen = recvbuf[6];
			mode = recvbuf[7];
			memcpy(keyData,recvbuf+8,keyLen);
			iRet = DLL_PciWriteMKey(keyType,keyNo,keyLen,keyData,mode);
			*wLen = 0;
			break;
		case PCI_KEY_LOAD_SESSION:
			CurAppNo = recvbuf[3];
			keyType = recvbuf[4];
			keyNo = recvbuf[5];
			keyLen = recvbuf[6];
			mkeyNo = recvbuf[7];
			mode = recvbuf[8];
			memcpy(keyData,recvbuf+9,keyLen);
			iRet = DLL_PciWriteSKey(keyType,keyNo,keyLen,keyData,mode,mkeyNo);
			*wLen = 0;
			break;
		case PCI_KEY_LOAD_DUKPTK:
			CurAppNo = recvbuf[3];
			keyType = recvbuf[4];
			keyNo = recvbuf[5];
			bdkLen = recvbuf[6];
			mode = recvbuf[7];
			memcpy(bdk,recvbuf+8,bdkLen);
			ksnLen = recvbuf[8+bdkLen];
			memcpy(ksn,recvbuf+9+bdkLen,ksnLen);
			iRet = DLL_PciWriteDukptKey(keyType,keyNo,bdkLen,bdk,ksnLen,ksn,mode);
			*wLen = 0;
			break;
	}
	return iRet;
}


int PciKeyUseModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen)
{
	int iRet = -1;
	BYTE keyNo, minLen, maxLen, cardNoLen, cardNo[50], mode, pinBlock[9], mark, amount[14], waitTimeSec;
	PinContext  ctx;
	PUBLIC_KEY pk;
	BYTE slot;
	int inLen;
	BYTE inData[1024];
	int i = 0;
	int sendLen = recvbuf[1]*256+recvbuf[2];

	memset(ctx.prompt,0,sizeof(ctx.prompt));
	memset(ctx.font,0,sizeof(ctx.font));
	memset(ctx.background,0,sizeof(ctx.background));
	
	memset(pk.Modul,0,sizeof(pk.Modul));
	memset(pk.Exponent,0,sizeof(pk.Exponent));
	
	switch(recvbuf[0])
	{
		case PCI_PIN_ENC_PINK:
			CurAppNo = recvbuf[3];
			keyNo = recvbuf[4];
			minLen = recvbuf[5];
			maxLen = recvbuf[6];
			cardNoLen = recvbuf[7];
			memcpy(cardNo,recvbuf+8,cardNoLen);
			mode = recvbuf[8+cardNoLen];
			waitTimeSec = recvbuf[9+cardNoLen];

			mark = recvbuf[10+cardNoLen];
			memcpy(amount,recvbuf+11+cardNoLen,14);
			memcpy(ctx.prompt,recvbuf+11+cardNoLen+14,sendLen-cardNoLen-4-3-14-8);
			memcpy(ctx.font,recvbuf+11+cardNoLen+14+sendLen-cardNoLen-4-3-14-8,4);
			memcpy(ctx.background,recvbuf+11+cardNoLen+14+sendLen-cardNoLen-4-3-14-8+4,3);

			iRet = DLL_PciGetPin(keyNo,minLen,maxLen,cardNoLen,cardNo,mode,sendbuf,mark,amount,waitTimeSec,&ctx);
			if(iRet==0) *wLen = 8;
			else *wLen = 0;
			break;
		case PCI_MAC_ENC_MACK:
			CurAppNo = recvbuf[3];
			keyNo = recvbuf[4];
			inLen = recvbuf[5]*256+recvbuf[6];
			memcpy(inData,recvbuf+7,inLen);
			mode = recvbuf[7+inLen];
			iRet = DLL_PciGetMac(keyNo,inLen,inData,sendbuf,mode);
			*wLen = 8;
			break;
		case PCI_DES_ENC_DESK:
			CurAppNo = recvbuf[3];
			keyNo = recvbuf[4];
			inLen = recvbuf[5]*256+recvbuf[6];
			memcpy(inData,recvbuf+7,inLen);
			mode = recvbuf[7+inLen];
			iRet = DLL_PciGetDes(keyNo,inLen,inData,sendbuf,mode);
			*wLen = sendLen;
			break;
		case PCI_PIN_ENC_DUKPT:
			CurAppNo = recvbuf[3];
			keyNo = recvbuf[4];
			minLen = recvbuf[5];
			maxLen = recvbuf[6];
			cardNoLen = recvbuf[7];
			memcpy(cardNo,recvbuf+8,cardNoLen);
			mode = recvbuf[8+cardNoLen];
			waitTimeSec = recvbuf[9+cardNoLen];

			mark = recvbuf[10+cardNoLen];
			memcpy(amount,recvbuf+11+cardNoLen,14);
			memcpy(ctx.prompt,recvbuf+11+cardNoLen+14,sendLen-cardNoLen-4-3-14-8);
			memcpy(ctx.font,recvbuf+11+cardNoLen+14+sendLen-cardNoLen-4-3-14-8,4);
			memcpy(ctx.background,recvbuf+11+cardNoLen+14+sendLen-cardNoLen-4-3-14-8+4,3);

			iRet = DLL_PciGetPinDukpt(keyNo,minLen,maxLen,cardNoLen,cardNo,mode,sendbuf,mark,amount,waitTimeSec,sendbuf+8,&ctx);
			if(iRet==0) *wLen = 8+10;
			else *wLen = 0;
			break;
		case PCI_MAC_ENC_DUKPT:
			CurAppNo = recvbuf[3];
			keyNo = recvbuf[4];
			inLen = recvbuf[5]*256+recvbuf[6];
			memcpy(inData,recvbuf+7,inLen);
			mode = recvbuf[7+inLen];
			iRet = DLL_PciGetMacDukpt(keyNo,inLen,inData,sendbuf,mode,sendbuf+8);
			if(iRet==0) *wLen = 8+10;
			else *wLen = 0;
		case PCI_DES_ENC_DUKPT:
			CurAppNo = recvbuf[3];
			keyNo = recvbuf[4];
			inLen = recvbuf[5]*256+recvbuf[6];
			memcpy(inData,recvbuf+7,inLen);
			mode = recvbuf[7+inLen];
			iRet = DLL_PciGetDesDukpt(keyNo,inLen,inData,sendbuf,mode,sendbuf+(inLen%8?inLen/8+1:inLen/8)*8);
			if(iRet==0) *wLen = (inLen%8?inLen/8+1:inLen/8+10)*8+10;
			else *wLen = 0;
			break;
		case PCI_PIN_ENC_FIXK:
			CurAppNo = recvbuf[3];
			keyNo = recvbuf[4];
			minLen = recvbuf[5];
			maxLen = recvbuf[6];
			cardNoLen = recvbuf[7];
			memcpy(cardNo,recvbuf+8,cardNoLen);
			mode = recvbuf[8+cardNoLen];
			waitTimeSec = recvbuf[9+cardNoLen];

			mark = recvbuf[10+cardNoLen];
			memcpy(amount,recvbuf+11+cardNoLen,14);
			memcpy(ctx.prompt,recvbuf+11+cardNoLen+14,sendLen-cardNoLen-4-3-14-8);
			memcpy(ctx.font,recvbuf+11+cardNoLen+14+sendLen-cardNoLen-4-3-14-8,4);
			memcpy(ctx.background,recvbuf+11+cardNoLen+14+sendLen-cardNoLen-4-3-14-8+4,3);

			iRet = DLL_PciGetPinByFixKey(keyNo,minLen,maxLen,cardNoLen,cardNo,mode,sendbuf,mark,amount,waitTimeSec,&ctx);
			if(iRet==0) *wLen = 8;
			else *wLen = 0;
			break;
		case PCI_OFFLINE_ENC_VER:
			CurAppNo = recvbuf[3];
			slot = recvbuf[4];
			minLen = recvbuf[5];
			maxLen = recvbuf[6];
			waitTimeSec = recvbuf[7];

			pk.ModulLen= recvbuf[8];
			memcpy(pk.Modul,recvbuf+9,pk.ModulLen);
			pk.ExponentLen = recvbuf[9+pk.ModulLen];
			memcpy(pk.Exponent,recvbuf+10+pk.ModulLen,pk.ExponentLen);
			memcpy(ctx.prompt,recvbuf+10+pk.ModulLen+pk.ExponentLen,sendLen-(253+5+4+3));//-7
			memcpy(ctx.font,recvbuf+10+pk.ModulLen+pk.ExponentLen+sendLen-(253+5+4+3),4);//-7
			memcpy(ctx.background,recvbuf+10+pk.ModulLen+pk.ExponentLen+sendLen-(253+5+4+3)+4,3);//

			iRet = DLL_PciOffLineEncPin(slot,minLen,maxLen,waitTimeSec,&pk,&ctx);
			*wLen = 0;
			break;
		case PCI_OFFLINE_CLR_VER:
			CurAppNo = recvbuf[3];
			slot = recvbuf[4];
			minLen = recvbuf[5];
			maxLen = recvbuf[6];
			waitTimeSec = recvbuf[7];

			
			memcpy(ctx.prompt,recvbuf+8,sendLen-5-4-3);
			memcpy(ctx.font,recvbuf+8+sendLen-5-4-3,4);
			memcpy(ctx.background,recvbuf+8+sendLen-5-4-3+4,3);

			iRet = DLL_PciOffLinePlainPin(slot,minLen,maxLen,waitTimeSec,&ctx);
			*wLen = 0;
			break;
		case PCI_CLEAR_APP_KEYS:
			CurAppNo = recvbuf[3];
			iRet = DLL_PciClearAppkeys(recvbuf[4]);
			*wLen = 0;
			break;
		case PCI_GET_MKEY_CHECKVALUE:
			CurAppNo = recvbuf[3];
			iRet = DLL_PciGetKeyCheckValue(recvbuf[4], recvbuf[5], recvbuf[6], sendbuf);
			*wLen = 4;
			break;
		case PCI_GET_SKEY_CHECKVALUE:
			CurAppNo = recvbuf[3];
			iRet = DLL_PciGetSignKeyCheckValue(recvbuf[4], sendbuf);
			*wLen = 32;
			break;
		case PCI_CLEAR_ALL_APPS_KEYS:
			CurAppNo = recvbuf[3];
			iRet = DLL_PciClearAllAppkeys();
			*wLen = 0;
			break;
	}
	return iRet;
}


int PciVerifyModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen)
{
	int iRet = -1;
	
	switch(recvbuf[0])
	{
		case PCI_VERIFY_SIGN:
			CurAppNo = recvbuf[3];
			iRet = DLL_PciVerifySignature(recvbuf[4], recvbuf+5);
			*wLen = 0;
			break;
		case PCI_CHECK_STATUS:
			CurAppNo = recvbuf[3];
			iRet = DLL_PciCheckSecurityChipStatus();
			*wLen = 0;
			break;
		case PCI_PWD_INPUT:
			CurAppNo = recvbuf[3];
			iRet = DLL_PciPwdInput(recvbuf[4], recvbuf[5]);
			break;
		case PCI_JUMPTO_OS:
			CurAppNo = recvbuf[3];
			iRet = DLL_PciJumpToOs();
			*wLen = 0;
			break;
		case PCI_DOWNLOAD_OS:
			CurAppNo = recvbuf[3];
			iRet = DLL_PciStartDownloadOs();
			*wLen = 0;
			break;
	}
	return iRet;
}


int PciRandModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen)
{
	int iRet = -1;
	
	switch(recvbuf[0])
	{
		case PCI_GET_RND:
			CurAppNo = recvbuf[3];
			iRet = DLL_PciGetRnd(sendbuf);
			*wLen = 8;
			break;
	}
	return iRet;
}

int SysModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen)
{
	int iRet = -1;
	switch(recvbuf[0])
	{
		case SYS_SETRTC:
			CurAppNo = recvbuf[3];
			iRet = DLL_SysSetDateTime(recvbuf+4);
			*wLen = 0;
			break;
		case SYS_GETRTC:
			CurAppNo = recvbuf[3];
			iRet = DLL_SysGetDateTime(sendbuf);
			*wLen = 6;
			break;
		case SYS_GETVER:
			CurAppNo = recvbuf[3];
			iRet = DLL_SysGetVersion(sendbuf);
			*wLen = 8;
			break;
		case SYS_CTLLED:
			CurAppNo = recvbuf[3];
			iRet = DLL_SysLedControl(recvbuf[4], recvbuf[5]);
			*wLen = 0;
			break;
		case SYS_SOUNDBEEP:
			CurAppNo = recvbuf[3];
			iRet = DLL_SysBeep();
			*wLen = 0;
			break;
		case SYS_SOUNDBEEF:
			CurAppNo = recvbuf[3];
			iRet = DLL_SysBeef(recvbuf[4], MAKEWORD(recvbuf[5],recvbuf[6]));
			*wLen = 0;
			break;
		case SYS_GETSN:
			CurAppNo = recvbuf[3];
			iRet = DLL_SysReadSecurityChipSN(sendbuf);
			*wLen = 13;
			break;
		case SYS_SOFTRESET:
			CurAppNo = recvbuf[3];
			iRet = DLL_SysSoftReset(recvbuf[4]);
			*wLen = 0;
			break;
		case SYS_WRITESN:
			CurAppNo = recvbuf[3];
			iRet = DLL_SysWriteSN(recvbuf[4], recvbuf+5);
			*wLen = 0;
			break;
		case SYS_READSN:
			CurAppNo = recvbuf[3];
			iRet = DLL_SysReadSN(sendbuf);
			*wLen = 12;
			break;
		case KB_INPUT_START:
			CurAppNo = recvbuf[3];
			iRet = DLL_KbInputStart();
			*wLen = 0;
			break;
		case KB_GET_KEY:
			CurAppNo = recvbuf[3];
			iRet = DLL_KbGetKey(sendbuf);
			*wLen = 1;
			break;
		case KB_INPUT_END:
			CurAppNo = recvbuf[3];
			iRet = DLL_KbInputStop();
			*wLen = 0;
			break;
		case SYS_REBOOT:
			CurAppNo = recvbuf[3];
			iRet = DLL_SysReboot();
			*wLen = 0;
			break;
		//battary	
		case SYS_GETCHARGINGSTATUS:
			CurAppNo = recvbuf[3];
			iRet = DLL_GetChargingStatus(sendbuf);
			*wLen = 8;
			break;
		case SYS_GETBATTARYSTATUS:
			CurAppNo = recvbuf[3];
			iRet = DLL_GetBatteryStatus(sendbuf);
			*wLen = 8;
			break;
	}
	return iRet;
}

int IccModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen)
{
	int iRet = -1;
	APDU_SEND apduSend;
	APDU_RECV apduRecv;
	
	switch(recvbuf[0])
	{
		case ICC_OPEN:
			CurAppNo = recvbuf[3];
			iRet = DLL_IccOpen(recvbuf[4],recvbuf[5],sendbuf);
			*wLen = sendbuf[0]+1;
			break;
		case ICC_CLOSE:
			CurAppNo = recvbuf[3];
			iRet = DLL_IccClose(recvbuf[4]);
			*wLen = 0;
			break;
		case ICC_COMMAND:
			CurAppNo = recvbuf[3];
			memcpy(apduSend.Command,recvbuf+5,4);
			apduSend.Lc = recvbuf[9]*256+recvbuf[10];
			memcpy(apduSend.DataIn,recvbuf+11,apduSend.Lc);
			apduSend.Le = recvbuf[11+apduSend.Lc]*256,recvbuf[11+apduSend.Lc+1];
			iRet = DLL_IccCommand(recvbuf[4],&apduSend,&apduRecv);
			memcpy(sendbuf,apduRecv.DataOut,apduRecv.LenOut);
			sendbuf[apduRecv.LenOut] = apduRecv.SWA;
			sendbuf[apduRecv.LenOut+1] = apduRecv.SWB;
			*wLen = apduRecv.LenOut+2;
			break;
		case ICC_CHECK:
			CurAppNo = recvbuf[3];
			iRet = DLL_IccCheck(recvbuf[4]);
			*wLen = 0;
			break;
		case ICC_SELETU:
			CurAppNo = recvbuf[3];
			iRet = DLL_IccSelectEtu(recvbuf[4],recvbuf[5]);
			*wLen = 0;
			break;
	}
	return iRet;
}

int MsrModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen)
{
	int iRet = -1;
    unsigned char track1Len[1];
    unsigned char track2Len[1];
    unsigned char track3Len[1];
    unsigned char track1[256];
    unsigned char track2[256];
    unsigned char track3[256];
	
	switch(recvbuf[0])
	{
		case MSR_OPEN:
			CurAppNo = recvbuf[3];
			iRet = DLL_McrOpen();
			*wLen = 0;
			break;
		case MSR_CLOSE:
			CurAppNo = recvbuf[3];
			iRet = DLL_McrClose();
			*wLen = 0;
			break;
		case MSR_CHECK:
			CurAppNo = recvbuf[3];
			iRet = DLL_McrCheck();
			*wLen = 0;
			break;
		case MSR_READ:
			CurAppNo = recvbuf[3];
			iRet = DLL_McrRead(track1Len,track1,track2Len,track2,track3Len,track3);
			sendbuf[0] = track1Len[0];
			sendbuf[1] = track2Len[0];
			sendbuf[2] = track3Len[0];
			memcpy(sendbuf+3,track1,track1Len[0]);
			memcpy(sendbuf+3+track1Len[0],track2,track2Len[0]);
			memcpy(sendbuf+3+track1Len[0]+track2Len[0],track3,track3Len[0]);
			*wLen = track1Len[0]+track2Len[0]+track3Len[0]+3;
			break;
		case MSR_RESET:
			CurAppNo = recvbuf[3];
			iRet = DLL_McrReset();
			*wLen = 0;
			break;
		case MSR_READ_VERSION:
			CurAppNo = recvbuf[3];
			iRet = DLL_McrReadVersion(sendbuf);
			*wLen = 20;
			break;
	}
	return iRet;
}

int PiccModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen)
{
	int iRet = -1;
	APDU_SEND apduSend;
	APDU_RECV apduRecv;
	BYTE cardType[2], serialNo[11];
	
	switch(recvbuf[0])
	{
		case PICC_OPEN:
			CurAppNo = recvbuf[3];
			iRet = DLL_PiccOpen();
			*wLen = 0;
			break;
		case PICC_CLOSE:
			CurAppNo = recvbuf[3];
			iRet = DLL_PiccClose();
			*wLen = 0;
			break;
		case PICC_CHECK:
			CurAppNo = recvbuf[3];
			iRet = DLL_PiccCheck(recvbuf[4], cardType, serialNo);
			memcpy(sendbuf,cardType,2);
			memcpy(sendbuf+2,serialNo,10);
			*wLen = 2+10;
			break;
		case PICC_COMMAND:
			CurAppNo = recvbuf[3];
			memcpy(apduSend.Command,recvbuf+4,4);
			apduSend.Lc = recvbuf[4+4]*256+recvbuf[4+5];
			memcpy(apduSend.DataIn,recvbuf+4+6,apduSend.Lc);
			apduSend.Le = recvbuf[4+6+apduSend.Lc]*256,recvbuf[4+6+apduSend.Lc+1];
			iRet = DLL_PiccCommand(&apduSend,&apduRecv);
			memcpy(sendbuf,apduRecv.DataOut,apduRecv.LenOut);
			sendbuf[apduRecv.LenOut] = apduRecv.SWA;
			sendbuf[apduRecv.LenOut+1] = apduRecv.SWB;
			*wLen = apduRecv.LenOut+2;
			break;
		case PICC_REMOVE:
			CurAppNo = recvbuf[3];
			iRet = DLL_PiccRemove();
			*wLen = 0;
			break;
		case PICC_HALT:
			CurAppNo = recvbuf[3];
			iRet = DLL_PiccHalt();
			*wLen = 0;
			break;
		case PICC_RESET:
			CurAppNo = recvbuf[3];
			iRet = DLL_PiccReset();
			*wLen = 0;
			break;
		case PICC_M1AUTHORITY:
			CurAppNo = recvbuf[3];
			iRet = DLL_PiccM1Authority(recvbuf[4], recvbuf[5], recvbuf+6, recvbuf+6+6);
			*wLen = 0;
			break;
		case PICC_M1READBLOCK:
			CurAppNo = recvbuf[3];
			iRet = DLL_PiccM1ReadBlock(recvbuf[4], sendbuf);
			*wLen = 16;
			break;
		case PICC_M1WRITEBLOCK:
			CurAppNo = recvbuf[3];
			iRet = DLL_PiccM1WriteBlock(recvbuf[4], recvbuf+4);
			*wLen = 0;
			break;
		case PICC_M1OPERATE:
			CurAppNo = recvbuf[3];
			iRet = DLL_PiccM1Operate(recvbuf[4], recvbuf[5], recvbuf+6, recvbuf[10]);
			*wLen = 0;
			break;
	}
	return iRet;
}

int PrinterModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen)
{
	int iRet = -1;
	int pixel,x;
	int tempValue;
	switch(recvbuf[0])
	{
		case PRINTER_INIT:
			CurAppNo = recvbuf[3];
			iRet = DLL_PrnInit();
			*wLen = 0;
			break;
		case PRINTER_SETSPACE:
			CurAppNo = recvbuf[3];
			iRet = DLL_PrnSetSpace(recvbuf[4], recvbuf[5]);
			*wLen = 0;
			break;
		case PRINTER_SETFONT:
			CurAppNo = recvbuf[3];
			iRet = DLL_PrnSetFont(recvbuf[4], recvbuf[5],recvbuf[6]);
			*wLen = 0;
			break;
		case PRINTER_GETFONT:
			CurAppNo = recvbuf[3];
			iRet = DLL_PrnGetFont(sendbuf, sendbuf+1, sendbuf+2);
			*wLen = 3;
			break;
		case PRINTER_STEP:
			CurAppNo = recvbuf[3];
			pixel = (recvbuf[7]*256+recvbuf[6])*65536 + recvbuf[5]*256 + recvbuf[4];
			iRet = DLL_PrnStep(pixel);
			*wLen = 0;
			break;
		case PRINTER_STR:
			CurAppNo = recvbuf[3];
			iRet = DLL_PrnStr(recvbuf+4);
			*wLen = 0;
			break;
		case PRINTER_LOGO:
			CurAppNo = recvbuf[3];
			iRet = DLL_PrnLogo(recvbuf+4);
			*wLen = 0;
			break;
		case PRINTER_SETLEFTINDENT:
			CurAppNo = recvbuf[3];
			x = (recvbuf[7]*256+recvbuf[6])*65536 + recvbuf[5]*256 + recvbuf[4];
			iRet = DLL_PrnSetLeftIndent(x);
			*wLen = 0;
			break;
		case PRINTER_CHECKSTATUS:
			CurAppNo = recvbuf[3];
			iRet = DLL_PrnCheckStatus();
			*wLen = 0;
			break;
		case PRINTER_SETINVERT:
			CurAppNo = recvbuf[3];
			iRet = DLL_PrnSetInvert(recvbuf[4]);
			*wLen = 0;
			break;
		case PRINTER_START:
			CurAppNo = recvbuf[3];
			iRet = DLL_PrnStart();
			*wLen = 0;
			break;
		case PRINTER_GETTEMP:
			CurAppNo = recvbuf[3];
			iRet = DLL_PrnGetTemperature(&tempValue);
			sendbuf[0] = tempValue/256;
			sendbuf[1] = tempValue%256;
			*wLen = 2;
			break;
		case PRINTER_SETTEMP:
			CurAppNo = recvbuf[3];
			tempValue = recvbuf[4]*256+recvbuf[5];
			iRet = DLL_PrnSetTemperature(tempValue);
			*wLen = 0;
			break;
		case PRINTER_SETGRAY:
			CurAppNo = recvbuf[3];
			iRet = DLL_PrnSetGray(recvbuf[4]);
			*wLen = 0;
			break;
		case PRINTER_SETMODE:
			CurAppNo = recvbuf[3];
			iRet = DLL_PrnSetMode(recvbuf[4]);
			*wLen = 0;
			break;
			
	}
	return iRet;
}

int ScanModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen)
{
	int iRet = -1;
	unsigned char prefix[5]={0};
	unsigned char suffix[3]={0};
	unsigned char *pprefix;
	unsigned char *psuffix;
	switch(recvbuf[0])
	{
		case SCAN_OPEN:
			CurAppNo = recvbuf[3];
			iRet = DLL_ScanOpen();
			*wLen = 0;
			break;
		case SCAN_CLOSE:
			CurAppNo = recvbuf[3];
			iRet = DLL_ScanClose();
			*wLen = 0;
			break;
		case SCAN_MODESET:
			CurAppNo = recvbuf[3];
			if(recvbuf[5]!=0)
			{
				memcpy(prefix,&recvbuf[6],recvbuf[5]);
				pprefix = prefix;
			}
			else
				pprefix =NULL;
			if(recvbuf[6+strlen(prefix)] !=0)
			{
				memcpy(suffix,&recvbuf[7+strlen(prefix)],recvbuf[6+strlen(prefix)]);
				psuffix = suffix;
			}
			else
				psuffix = NULL;
			iRet = DLL_ScanModeSet(recvbuf[4],pprefix, psuffix);;
			*wLen = 0;
			break;
		case SCAN_TRIGGER:
			CurAppNo = recvbuf[3];
			iRet = DLL_ScanTrigger();
			*wLen = 0;
			break;
		case SCAN_READ:
			CurAppNo = recvbuf[3];
			iRet = DLL_ScanRead(sendbuf);
			*wLen = strlen(sendbuf);
			break;
		case SCAN_KEYSET:
			CurAppNo = recvbuf[3];
			iRet = DLL_ScanKeySet(recvbuf[4]);
			*wLen = 0;
			break;
	}
	return iRet;
}

int SerialModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen)
{
	int iRet = -1;
	unsigned short sendLen=0,recvLen,timeout,expectLen;
	unsigned char recvData[1024];


	switch(recvbuf[0])
	{
		case SERIAL_SEND:
			CurAppNo = recvbuf[3];
			if(CurAppNo != 0xff && CurAppNo != 0xfe)
		    	{
		    		*wLen = 0;
		    		return App_Permissions_Denial;
		    	}
			sendLen = recvbuf[4]*256+recvbuf[5];
			iRet = Lib_UartSend(PINPAD_COM,recvbuf+5,sendLen);
			*wLen = 0;
			break;
		case SERIAL_RECV:
			CurAppNo = recvbuf[3];
			if(CurAppNo != 0xff && CurAppNo != 0xfe)
		    	{
		    		*wLen = 0;
		    		return App_Permissions_Denial;
		    	}
			expectLen = recvbuf[4]*256+recvbuf[5];
			timeout = recvbuf[6]*256+recvbuf[7];
			iRet = Lib_UartRecv(PINPAD_COM,recvData,&recvLen,timeout,expectLen);
			*wLen = recvLen;
			memcpy(sendbuf,recvData,recvLen);
			break;
	}
	return iRet;
}

int ExternalSerialModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen)
{
	int iRet = -1;
	unsigned short sendLen=0,recvLen,timeout,expectLen,paralen;
	unsigned char recvData[1024];
	unsigned char para[100] = {0};


	switch(recvbuf[0])
	{
		case EXTERNAL_SERIAL_OPEN:
			CurAppNo = recvbuf[3];
		    	if(authCheck(CurAppNo, MODULE_TYPE_EXTERNALSERIAL))
		    	{
		    		*wLen = 0;
		    		return App_Permissions_Denial;
		    	}
			iRet = Lib_UartInit(TRSM_COM, 115200);
			iRet = Lib_UartReset(TRSM_COM);
			*wLen = 0;
			break;
		case EXTERNAL_SERIAL_SEND:
			CurAppNo = recvbuf[3];
		    	if(authCheck(CurAppNo, MODULE_TYPE_EXTERNALSERIAL))
		    	{
		    		*wLen = 0;
		    		return App_Permissions_Denial;
		    	}
			sendLen = recvbuf[4]*256+recvbuf[5];
			iRet = Lib_UartSend(TRSM_COM,recvbuf+6,sendLen);
			*wLen = 0;
			break;
		case EXTERNAL_SERIAL_RECV:
			CurAppNo = recvbuf[3];
		    	if(authCheck(CurAppNo, MODULE_TYPE_EXTERNALSERIAL))
		    	{
		    		*wLen = 0;
		    		return App_Permissions_Denial;
		    	}
			expectLen = recvbuf[4]*256+recvbuf[5];
			timeout = recvbuf[6]*256+recvbuf[7];
			iRet = Lib_UartRecv(TRSM_COM,recvData,&recvLen,timeout,expectLen);
			*wLen = recvLen;
			memcpy(sendbuf,recvData,recvLen);
			break;
		case EXTERNAL_SERIAL_CLOSE:
			CurAppNo = recvbuf[3];
		    	if(authCheck(CurAppNo, MODULE_TYPE_EXTERNALSERIAL))
		    	{
		    		*wLen = 0;
		    		return App_Permissions_Denial;
		    	}
			iRet = Lib_UartClose(TRSM_COM);
			*wLen = 0;
			break;
		case EXTERNAL_SERIAL_OPEN_EX:
			CurAppNo = recvbuf[3];
		    	if(authCheck(CurAppNo, MODULE_TYPE_EXTERNALSERIAL))
		    	{
		    		*wLen = 0;
		    		return App_Permissions_Denial;
		    	}
			paralen = recvbuf[4]*256+recvbuf[5];
			memcpy(para,recvbuf+6,paralen);
			iRet = Lib_UartInitEx(TRSM_COM, para);
			iRet = Lib_UartReset(TRSM_COM);
			*wLen = 0;
			break;			
	}
	return iRet;
}

int DownloadModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen)
{
	int iRet = -1;
	switch(recvbuf[0])
	{
		case DOWNLOAD_APP:
			CurAppNo = recvbuf[3];
			iRet = DownloadApp(recvbuf+4);
			*wLen = 0;
			break;
		
	}
	return iRet;
}
int SiganltureModuleFun(unsigned char *recvbuf, unsigned char *sendbuf,unsigned short *wLen)
{
	int iRet = -1;
	unsigned short sendLen=0,recvLen,timeout,expectLen;
	unsigned char recvData[1024];


	switch(recvbuf[0])
	{
		case SIGNATURE_INIT:
			CurAppNo = recvbuf[3];
			sendLen = recvbuf[4]*256+recvbuf[5];
			iRet = DLL_HandSignInit();
			*wLen = 0;
			break;
		case SIGNATURE_GET_DATA:
			CurAppNo = recvbuf[3];
			iRet = DLL_HandSignGetData(sendbuf);
			*wLen = strlen(sendbuf);
			break;
			break;
	}
	return iRet;
}

#define DEVNAME "/dev/input/event0" 
#define KEYDOWN 1
#define KEYUP   0


int reportkey(int fd, uint16_t type, uint16_t keycode, int32_t value)  
{  
    struct input_event event;  
  
    event.type = type;  
    event.code = keycode;  
    event.value = value;  
  
    gettimeofday(&event.time, 0);  
  
    if (write(fd, &event, sizeof(struct input_event)) < 0) {  
        printf("report key error!\n");  
        return -1;  
    }  

	event.type = EV_SYN;  
	event.code = SYN_REPORT;  
	event.value = 0;  
	write(fd, &event, sizeof(event));    

  
    return 0;  
}  

#define KEY1             0x31
#define KEY2             0x32
#define KEY3             0x33
#define KEY4             0x34
#define KEY5             0x35
#define KEY6             0x36
#define KEY7             0x37
#define KEY8             0x38
#define KEY9             0x39
#define KEY0             0x30
#define KEYWELL        0x23
#define KEYCLEAR         0x08
#define KEYALPHA         0x07
#define KEYENTER         0x0d
#define KEYCANCEL        0x1b
#define KEYBACKSPACE     0x1c
#define KEYF1				 0x1e
#define KEYF2				 0x1f
#define KEYF3				 0x20
#define KEYF4				 0x21
#define KEYPOWER			 0x25
#define WAKEUP				0x26

int sendKey(unsigned char key)
{
    int k_fd;
    uint16_t keycode;    
    if((key>='0'&&key<='9')||(key>='a'&&key<='z')||(key>='A'&&key<='Z')||
			(key>=KEYF1&&key<=KEYF4)||//F1~F4
			key==KEYENTER||key==KEYCANCEL||key==KEYBACKSPACE||key==KEYCLEAR||key=='*'||key=='#'||key==','||key=='.')
    {
	    k_fd = open(DEVNAME, O_RDWR);  
	  
	    if (k_fd < 0) {  
			printf("open error!\n");  
			return k_fd;  
	    }
            if((key>='A'&&key<='Z')||(key=='*'||key=='#'))
			reportkey(k_fd, EV_KEY, KEY_LEFTSHIFT, KEYDOWN);

	    switch(key)
		{
			case ',':
				keycode = KEY_COMMA;
				break;
			case '.':
				keycode = KEY_DOT;
				break;
			case 'A':
			case 'a':
				keycode = KEY_A;
				break;
			case 'B':
			case 'b':
				keycode = KEY_B;
				break;
			case 'C':
			case 'c':
				keycode = KEY_C;
				break;
			case 'D':
			case 'd':
				keycode = KEY_D;
				break;
			case 'E':
			case 'e':
				keycode = KEY_E;
				break;
			case 'F':
			case 'f':
				keycode = KEY_F;
				break;
			case 'G':
			case 'g':
				keycode = KEY_G;
				break;
			case 'H':
			case 'h':
				keycode = KEY_H;
				break;
			case 'I':
			case 'i':
				keycode = KEY_I;
				break;
			case 'J':
			case 'j':
				keycode = KEY_J;
				break;
			case 'K':
			case 'k':
				keycode = KEY_K;
				break;
			case 'L':
			case 'l':
				keycode = KEY_L;
				break;
			case 'M':
			case 'm':
				keycode = KEY_M;
				break;
			case 'N':
			case 'n':
				keycode = KEY_N;
				break;
			case 'O':
			case 'o':
				keycode = KEY_O;
				break;
			case 'P':
			case 'p':
				keycode = KEY_P;
				break;
			case 'Q':
			case 'q':
				keycode = KEY_Q;
				break;
			case 'R':
			case 'r':
				keycode = KEY_R;
				break;
			case 'S':
			case 's':
				keycode = KEY_S;
				break;
			case 'T':
			case 't':
				keycode = KEY_T;
				break;
			case 'U':
			case 'u':
				keycode = KEY_U;
				break;
			case 'V':
			case 'v':
				keycode = KEY_V;
				break;
			case 'W':
			case 'w':
				keycode = KEY_W;
				break;
			case 'X':
			case 'x':
				keycode = KEY_X;
				break;
			case 'Y':
			case 'y':
				keycode = KEY_Y;
				break;
			case 'Z':
			case 'z':
				keycode = KEY_Z;
				break;
			case '0':
				keycode = KEY_0;
				break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				keycode = key-0x2F;
				break;
			case '#':
				keycode = KEY_3;
				break;
			case '*':
				keycode = KEY_8;
				break;
			case 0x1e:
				keycode = KEY_F1;
				break;
			case 0x1f:
				keycode = KEY_F2;
				break;
			case 0x20:
				keycode = KEY_F3;
				break;
			case 0x21:
				keycode = KEY_F4;
				break;
			case 0x0d:
				keycode = KEY_ENTER;
				break;
			case 0x1B:
				keycode = KEY_CANCEL;
				break;
			case 0x08:
			case 0x1c:
				keycode = KEY_BACKSPACE;
				break;
			default:break;
		}

		reportkey(k_fd, EV_KEY, keycode, KEYDOWN);  
		reportkey(k_fd, EV_KEY, keycode, KEYUP);
            if((key>='A'&&key<='Z')||(key=='*'||key=='#'))
			reportkey(k_fd, EV_KEY, KEY_LEFTSHIFT, KEYUP);  
	    close(k_fd);  
    }
}




extern unsigned char RecvCrcPack_Flag;
const BYTE g_abyKeyTable[11][9]={"1","2ABCabc","3DEFdef","4GHIghi","5JKLjkl","6MNOmno","7PQRSpqrs","8TUVtuv","9WXYZwxyz","0*","#,."};
const BYTE g_abyKeyIndexLen[11]={1,7,7,7,7,7,9,7,9,2,3};

unsigned char sleep_cnt = 0;

void ComRecvThread(void)
{
    	unsigned char abyCrc[2];
    	int i,iRet;
    	unsigned short wPacketLen = 0;
 	unsigned short recv_len;
	unsigned char tempKeyValue=0xff,keyValue;
	static int index = 0;
	while(1)
	{
		//getpin 需要转发长度
		while(GetPin_Flag)
		{
			continue;
		}
		while (1)
		{
			iRet = Lib_UartRecv(PINPAD_COM,g_abyRecvBuff_FromSecChip,&recv_len,10,1);
			if ((0==iRet) && (0x02==g_abyRecvBuff_FromSecChip[0]||0x03==g_abyRecvBuff_FromSecChip[0]))
			{
				break;
			}
			break;
		}
		if(iRet !=0 ) continue;
		iRet = Lib_UartRecv(PINPAD_COM,g_abyRecvBuff_FromSecChip+1,&recv_len,RECV_TIMEOUT,4);
		//for(i=0; i<5; i++)printf("receive1......%02x\r\n",g_abyRecvBuff_FromSecChip[i]);
		if (0 != iRet)
		{
			memset(g_abyRecvBuff_FromSecChip,0x00,sizeof(g_abyRecvBuff_FromSecChip));
			wPacketLen = 0;
			continue;
		}
		wPacketLen = g_abyRecvBuff_FromSecChip[3]*256 + g_abyRecvBuff_FromSecChip[4];
		if (wPacketLen > RECV_PACKET_MAX_LEN)
		{
			memset(g_abyRecvBuff_FromSecChip,0x00,sizeof(g_abyRecvBuff_FromSecChip));
			wPacketLen = 0;
			continue;
		}
		for (i=0; i<wPacketLen+2; i++)
		{
			iRet = Lib_UartRecv(PINPAD_COM,&g_abyRecvBuff_FromSecChip[i+5],&recv_len,RECV_TIMEOUT,1);
			if (0 != iRet)
			{
				break;;
			}
		}

		Crc16CCITT(&g_abyRecvBuff_FromSecChip[1], wPacketLen+4, abyCrc);
		if (0 != memcmp(abyCrc, &g_abyRecvBuff_FromSecChip[5+wPacketLen], 2))
		{
			memset(g_abyRecvBuff_FromSecChip,0x00,sizeof(g_abyRecvBuff_FromSecChip));
			continue;
		}
		if(0x03 == g_abyRecvBuff_FromSecChip[0])
		{
			/*k_fd = open(DEVNAME, O_RDWR);  

			if (k_fd < 0) {  
			printf("open error!\n");  
			return k_fd;  
			}  

			keycode = g_abyRecvBuff_FromSecChip[5]-0x31;  
			printf("keycode is %02x\r\n",keycode);
			reportkey(k_fd, EV_KEY, keycode, KEYDOWN);  
			reportkey(k_fd, EV_KEY, keycode, KEYUP);  

			close(k_fd);  */
			printf("keycode is %02x\r\n",keyValue);
			keyValue = g_abyRecvBuff_FromSecChip[5];
			if(keyValue == KEYALPHA)
			{
				switch(tempKeyValue)
				{
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
			                     index++;
			                     index %= g_abyKeyIndexLen[tempKeyValue-'1'];
						sendKey(KEYBACKSPACE);
						keyValue = g_abyKeyTable[tempKeyValue-'1'][index];
						sendKey(keyValue);
						break;
					case '0':
			                     index++;
			                     index %= g_abyKeyIndexLen[9];
						sendKey(KEYBACKSPACE);
						keyValue = g_abyKeyTable[9][index];
						sendKey(keyValue);
						break;
					case '#':
			                     index++;
			                     index %= g_abyKeyIndexLen[10];
						sendKey(KEYBACKSPACE);
						keyValue = g_abyKeyTable[10][index];
						sendKey(keyValue);
						break;
				}
				continue;
					
			}
			else if(keyValue == KEYPOWER)
			{
				DLL_GprsSetSleepMode(1);
				SetGpioInorOut(0);
				DLL_SysStandby();
				memset(g_abyRecvBuff_FromSecChip,0x00,sizeof(g_abyRecvBuff_FromSecChip));
			}
			else if(keyValue == WAKEUP)
			{
				SetGpioInorOut(1);
				DLL_OpenBacklight();  //打开背光
				DLL_GprsSetSleepMode(0);
				memset(g_abyRecvBuff_FromSecChip,0x00,sizeof(g_abyRecvBuff_FromSecChip));
			}
			index = 0;
			if(keyValue != WAKEUP)
			sendKey(keyValue);
			tempKeyValue = keyValue;
			continue;
		}

		RecvCrcPack_Flag = 1;
	}

}



