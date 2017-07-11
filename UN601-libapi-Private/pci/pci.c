#include <stdio.h>
#include<string.h>

#include "user.h"
#include "Dll_api.h"

extern BYTE g_CurAppNo;

int DLL_PciGetRnd(BYTE *rnd)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = PCI_RND_MODULE;
	g_abySendBuff[1] = PCI_GET_RND;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;
	
	g_abySendBuff[4] = g_CurAppNo;
	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	if(0==iRet) memcpy(rnd,g_abyRecvBuff+2,recvLen-2);
	
	return iRet;	
}

int DLL_PciWriteMKey(BYTE keyType, BYTE keyNo, BYTE keyLen, BYTE *keyData, BYTE mode)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 5+keyLen;
	g_abySendBuff[0] = PCI_KEY_LOAD_MODULE;
	g_abySendBuff[1] = PCI_KEY_LOAD_MASTER;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	 g_abySendBuff[4] = g_CurAppNo;
	 g_abySendBuff[5] = keyType;
        g_abySendBuff[6] = keyNo;
        g_abySendBuff[7] = keyLen;	
        g_abySendBuff[8] = mode;	       
	 memcpy(g_abySendBuff+9,keyData, keyLen);


	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_PciWriteDukptKey(BYTE keyType, BYTE keyNo, BYTE bdkLen, BYTE *bdk, BYTE ksnLen, BYTE *ksn, BYTE mode)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = bdkLen+ksnLen+6;
	g_abySendBuff[0] = PCI_KEY_LOAD_MODULE;
	g_abySendBuff[1] = PCI_KEY_LOAD_DUKPTK;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	 g_abySendBuff[4] = g_CurAppNo;
        g_abySendBuff[5] = keyType;
        g_abySendBuff[6] = keyNo;
        g_abySendBuff[7] = bdkLen;
        g_abySendBuff[8] = mode;	        
        memcpy(g_abySendBuff+9,bdk,bdkLen);
        g_abySendBuff[9+bdkLen] = ksnLen;
        memcpy(g_abySendBuff+9+bdkLen+1,ksn,ksnLen);


	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_PciWriteSKey(BYTE keyType, BYTE keyNo, BYTE keyLen, BYTE *keyData, BYTE mode, BYTE mkeyNo)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = keyLen+6;
	g_abySendBuff[0] = PCI_KEY_LOAD_MODULE;
	g_abySendBuff[1] = PCI_KEY_LOAD_SESSION;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	 g_abySendBuff[4] = g_CurAppNo;
        g_abySendBuff[5] = keyType;
        g_abySendBuff[6] = keyNo;
        g_abySendBuff[7] = keyLen;
        g_abySendBuff[8] = mkeyNo;
        g_abySendBuff[9] = mode;	        
        memcpy(g_abySendBuff+10,keyData, keyLen);
	        		

	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	
	return iRet;	
}

int DLL_PciGetPin(BYTE keyNo,BYTE minLen,BYTE maxLen,BYTE cardNoLen, BYTE *cardNo,BYTE mode,BYTE *pinBlock ,BYTE mark,BYTE *amount,BYTE waitTimeSec,PinContext *ctx)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = cardNoLen+6+1+14+strlen(ctx->prompt)+4+3+1;
	
	g_abySendBuff[0] = PCI_KEY_USE_MODULE;
	g_abySendBuff[1] = PCI_PIN_ENC_PINK;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	 g_abySendBuff[4] = g_CurAppNo;
        g_abySendBuff[5] = keyNo; 
        g_abySendBuff[6] = minLen;
        g_abySendBuff[7] = maxLen;
        g_abySendBuff[8] = cardNoLen;
        memcpy(g_abySendBuff+9,cardNo,cardNoLen);
        g_abySendBuff[9+cardNoLen] = mode;
        g_abySendBuff[10+cardNoLen] = waitTimeSec;

	g_abySendBuff[11+cardNoLen] = mark;
	memcpy(g_abySendBuff+11+cardNoLen+1,amount,14);
	
	memcpy(g_abySendBuff+11+cardNoLen+1+14,ctx->prompt,strlen(ctx->prompt));
	memcpy(g_abySendBuff+11+cardNoLen+1+14+strlen(ctx->prompt),ctx->font,4);
	memcpy(g_abySendBuff+11+cardNoLen+1+14+strlen(ctx->prompt)+4,ctx->background,3);



	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	if(0 == iRet) memcpy(pinBlock,g_abyRecvBuff+2,recvLen-2);
	
	return iRet;	
}

int DLL_PciGetMac(BYTE keyNo, int inLen, BYTE *inData, BYTE *macOut, BYTE mode)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = inLen+5;
	g_abySendBuff[0] = PCI_KEY_USE_MODULE;
	g_abySendBuff[1] = PCI_MAC_ENC_MACK;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	 g_abySendBuff[4] = g_CurAppNo;
        g_abySendBuff[5] = keyNo; 
        g_abySendBuff[6] = (inLen/256);
        g_abySendBuff[7] = (inLen%256);
        if(inLen > 1024) return PCI_DataLen_Err;
        memcpy(g_abySendBuff+8,inData, inLen);
        g_abySendBuff[8+inLen] = mode;


	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	if(0==iRet) memcpy(macOut,g_abyRecvBuff+2,recvLen-2);
	
	return iRet;	
}

int DLL_PciGetDes(BYTE keyNo, int inLen, BYTE *inData, BYTE *desOut, BYTE mode)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = inLen+5;
	g_abySendBuff[0] = PCI_KEY_USE_MODULE;
	g_abySendBuff[1] = PCI_DES_ENC_DESK;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	 g_abySendBuff[4] = g_CurAppNo;
        g_abySendBuff[5] = keyNo; 
        g_abySendBuff[6] = (inLen/256);
        g_abySendBuff[7] = (inLen%256);
        if(inLen > 1024) return PCI_DataLen_Err;
        memcpy(g_abySendBuff+8,inData, inLen);
        g_abySendBuff[8+inLen] = mode;


	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	if(0==iRet) memcpy(desOut,g_abyRecvBuff+2,recvLen-2);
	
	return iRet;	
}

int DLL_PciGetPinDukpt( BYTE keyNo,BYTE minLen, BYTE maxLen, BYTE cardNoLen, BYTE *cardNo,BYTE mode,BYTE *pinBlock,BYTE mark, BYTE *amount, BYTE waitTimeSec,BYTE *outKsn,PinContext *ctx)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = cardNoLen+6+1+14+strlen(ctx->prompt)+4+3+1;
	g_abySendBuff[0] = PCI_KEY_USE_MODULE;
	g_abySendBuff[1] = PCI_PIN_ENC_DUKPT;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	 g_abySendBuff[4] = g_CurAppNo;
        g_abySendBuff[5] = keyNo; 
        g_abySendBuff[6] = minLen;
        g_abySendBuff[7] = maxLen;
        g_abySendBuff[8] = cardNoLen;
        memcpy(g_abySendBuff+9,cardNo,cardNoLen);
        g_abySendBuff[8+cardNoLen] = mode;
        g_abySendBuff[10+cardNoLen] = waitTimeSec;

	g_abySendBuff[11+cardNoLen] = mark;
	memcpy(g_abySendBuff+11+cardNoLen+1,amount,14);
	memcpy(g_abySendBuff+11+cardNoLen+1+14,ctx->prompt,strlen(ctx->prompt));
	memcpy(g_abySendBuff+11+cardNoLen+1+14+strlen(ctx->prompt),ctx->font,4);
	memcpy(g_abySendBuff+11+cardNoLen+1+14+strlen(ctx->prompt)+4,ctx->background,3);



	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	if(0 == iRet) 
	{
		memcpy(pinBlock,g_abyRecvBuff+2,8);
		memcpy(outKsn,g_abyRecvBuff+2+8,10);
	}
	
	return iRet;	
}

int DLL_PciGetMacDukpt( BYTE keyNo, int inLen, BYTE *inData, BYTE *macOut, BYTE mode,BYTE *outKsn)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = inLen+5;
	g_abySendBuff[0] = PCI_KEY_USE_MODULE;
	g_abySendBuff[1] = PCI_MAC_ENC_DUKPT;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	 g_abySendBuff[4] = g_CurAppNo;
        g_abySendBuff[5] = keyNo; 
        g_abySendBuff[6] = (inLen/256);
        g_abySendBuff[7] = (inLen%256);
        if(inLen > 1024) return PCI_DataLen_Err;
        memcpy(g_abySendBuff+8,inData,inLen);
        g_abySendBuff[8+inLen] = mode;
	

	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	if(0==iRet) 
	{
		memcpy(macOut,g_abyRecvBuff+2,8);
		memcpy(outKsn,g_abyRecvBuff+2+8,10);
	}
	
	return iRet;	
}

int DLL_PciGetDesDukpt(BYTE keyNo, int inLen, BYTE *inData, BYTE *desOut, BYTE mode, BYTE *outKsn)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = inLen+5;
	g_abySendBuff[0] = PCI_KEY_USE_MODULE;
	g_abySendBuff[1] = PCI_DES_ENC_DUKPT;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

 	 g_abySendBuff[4] = g_CurAppNo;
        g_abySendBuff[5] = keyNo; 
        g_abySendBuff[6] = (inLen/256);
        g_abySendBuff[7] = (inLen%256);
        if(inLen > 1024) return PCI_DataLen_Err;
        memcpy(g_abySendBuff+8,inData,inLen);
        g_abySendBuff[8+inLen] = mode;
	

	
	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	if(0==iRet) 
	{
		memcpy(desOut,g_abyRecvBuff+2,recvLen-10-2);
		memcpy(outKsn,g_abyRecvBuff+2+recvLen-10-2,10);
	}
	
	return iRet;	
}

int DLL_PciGetPinByFixKey(BYTE keyNo,BYTE minLen,BYTE maxLen,BYTE cardNoLen, BYTE *cardNo,BYTE mode,BYTE *pinBlock ,BYTE mark,BYTE *amount,BYTE waitTimeSec,PinContext *ctx)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = cardNoLen+6+1+14+strlen(ctx->prompt)+4+3+1;
	g_abySendBuff[0] = PCI_KEY_USE_MODULE;
	g_abySendBuff[1] = PCI_PIN_ENC_FIXK;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	 g_abySendBuff[4] = g_CurAppNo;
        g_abySendBuff[5] = keyNo; 
        g_abySendBuff[6] = minLen;
        g_abySendBuff[7] = maxLen;
        g_abySendBuff[8] = cardNoLen;
        memcpy(g_abySendBuff+9,cardNo,cardNoLen);
        g_abySendBuff[9+cardNoLen] = mode;
        g_abySendBuff[10+cardNoLen] = waitTimeSec;

	g_abySendBuff[11+cardNoLen] = mark;
	memcpy(g_abySendBuff+11+cardNoLen+1,amount,14);
	memcpy(g_abySendBuff+11+cardNoLen+1+14,ctx->prompt,strlen(ctx->prompt));
	memcpy(g_abySendBuff+11+cardNoLen+1+14+strlen(ctx->prompt),ctx->font,4);
	memcpy(g_abySendBuff+11+cardNoLen+1+14+strlen(ctx->prompt)+4,ctx->background,3);



	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
	if(0 == iRet) 
	{
		memcpy(pinBlock,g_abyRecvBuff+2,8);
	}
	
	return iRet;	
}
		
int DLL_PciOffLineEncPin(BYTE slot, BYTE minLen, BYTE maxLen, BYTE waitTimeSec, PUBLIC_KEY *pk, PinContext *ctx)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 253+5+strlen(ctx->prompt)+4+3;
	g_abySendBuff[0] = PCI_KEY_USE_MODULE;
	g_abySendBuff[1] = PCI_OFFLINE_ENC_VER;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	 g_abySendBuff[4] = g_CurAppNo;
        g_abySendBuff[5] = slot; 
        g_abySendBuff[6] = minLen;
        g_abySendBuff[7] = maxLen;
        g_abySendBuff[8] = waitTimeSec;


	 g_abySendBuff[9] = pk->ModulLen;
        memcpy(g_abySendBuff+10,pk->Modul,pk->ModulLen);
	 g_abySendBuff[10+pk->ModulLen]=pk->ExponentLen;
	 memcpy(g_abySendBuff+10+pk->ModulLen+1,pk->Exponent,pk->ExponentLen);
	
	memcpy(g_abySendBuff+10+pk->ModulLen+1+pk->ExponentLen,ctx->prompt,strlen(ctx->prompt));
	memcpy(g_abySendBuff+10+pk->ModulLen+1+pk->ExponentLen+strlen(ctx->prompt),ctx->font,4);
	memcpy(g_abySendBuff+10+pk->ModulLen+1+pk->ExponentLen+strlen(ctx->prompt)+4,ctx->background,3);


	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
    	if(iRet >= 256|| iRet <=0) iRet = 0-iRet;

	return iRet;	
}

int DLL_PciOffLinePlainPin(BYTE slot, BYTE minLen, BYTE maxLen, BYTE waitTimeSec, PinContext *ctx)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 5+strlen(ctx->prompt)+4+3;
	g_abySendBuff[0] = PCI_KEY_USE_MODULE;
	g_abySendBuff[1] = PCI_OFFLINE_CLR_VER;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	 g_abySendBuff[4] = g_CurAppNo;
        g_abySendBuff[5] = slot; 
        g_abySendBuff[6] = minLen;
        g_abySendBuff[7] = maxLen;
        g_abySendBuff[8] = waitTimeSec;
	
	memcpy(g_abySendBuff+9,ctx->prompt,strlen(ctx->prompt));
	memcpy(g_abySendBuff+9+strlen(ctx->prompt),ctx->font,4);
	memcpy(g_abySendBuff+9+strlen(ctx->prompt)+4,ctx->background,3);


	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
    	if(iRet >= 256|| iRet <=0) iRet = 0-iRet;

	return iRet;	
}
		
int DLL_PciWriteSignKey(BYTE fwrType, BYTE *keyData, BYTE *signData)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 521+1;
	g_abySendBuff[0] = PCI_KEY_LOAD_MODULE;
	g_abySendBuff[1] = PCI_KEY_LOAD_SIGN;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

 	g_abySendBuff[4] = g_CurAppNo;
       g_abySendBuff[5] = fwrType; 
        memcpy(g_abySendBuff+6,keyData, 264);
        memcpy(g_abySendBuff+6+264,signData, 256);


	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value

	return iRet;	
}
		
int DLL_PciVerifySignature(BYTE fwrType, BYTE *verifyData)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 289+1;
	g_abySendBuff[0] = PCI_VERIFY_MODULE;
	g_abySendBuff[1] = PCI_VERIFY_SIGN;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
        g_abySendBuff[5] = fwrType; 
        memcpy(g_abySendBuff+6,verifyData,288);


	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value

	return iRet;	
}
			
int DLL_PciCheckSecurityChipStatus()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = PCI_VERIFY_MODULE;
	g_abySendBuff[1] = PCI_CHECK_STATUS;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;

	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value

	return iRet;	
}
		
int DLL_PciPwdInput(BYTE pwdType, BYTE mode)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 2+1;
	g_abySendBuff[0] = PCI_VERIFY_MODULE;
	g_abySendBuff[1] = PCI_PWD_INPUT;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;
        g_abySendBuff[5] = pwdType;
        g_abySendBuff[6] = mode;


	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
    	if(iRet >= 256) iRet = 0-iRet;

	return iRet;	
}
		

int DLL_PciJumpToOs()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = PCI_VERIFY_MODULE;
	g_abySendBuff[1] = PCI_JUMPTO_OS;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;


	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value

	return iRet;	
}
		
int DLL_PciStartDownloadOs()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = PCI_VERIFY_MODULE;
	g_abySendBuff[1] = PCI_DOWNLOAD_OS;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;


	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value

	return iRet;	
}

int DLL_PciClearAppkeys(BYTE appNo)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 1+1;
	g_abySendBuff[0] = PCI_KEY_USE_MODULE;
	g_abySendBuff[1] = PCI_CLEAR_APP_KEYS;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;


	g_abySendBuff[4] = g_CurAppNo;
        g_abySendBuff[5] = appNo;


	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value

	return iRet;	
}

int DLL_PciClearAllAppkeys()
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 0+1;
	g_abySendBuff[0] = PCI_KEY_USE_MODULE;
	g_abySendBuff[1] = PCI_CLEAR_ALL_APPS_KEYS;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;

	g_abySendBuff[4] = g_CurAppNo;


	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value

	return iRet;	
}

int DLL_PciGetKeyCheckValue(BYTE appNo,BYTE keyType,BYTE keyNo, BYTE *checkValue)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	

	wLen = 3+1;
	g_abySendBuff[0] = PCI_KEY_USE_MODULE;
	g_abySendBuff[1] = PCI_GET_MKEY_CHECKVALUE;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;


	g_abySendBuff[4] = g_CurAppNo;
        g_abySendBuff[5] = appNo;
        g_abySendBuff[6] = keyType;
        g_abySendBuff[7] = keyNo;


	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
    	if(0 == iRet) memcpy(checkValue, g_abyRecvBuff+2,recvLen-2);

	return iRet;	
}

int DLL_PciGetSignKeyCheckValue(BYTE keyType, BYTE *checkValue)
{
	int iRet=-1;
	unsigned short wLen = 0;
	unsigned short sendLen=0;
	unsigned short recvLen=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
	
	wLen = 1+1;
	g_abySendBuff[0] = PCI_KEY_USE_MODULE;
	g_abySendBuff[1] = PCI_GET_SKEY_CHECKVALUE;
	g_abySendBuff[2] = wLen/256;
	g_abySendBuff[3] = wLen%256;


  	g_abySendBuff[4] = g_CurAppNo;
      g_abySendBuff[5] = keyType;

	iRet = MsgQueueSend(g_abySendBuff, wLen+4);
	if(iRet != 0) return iRet;
	iRet = MsgQueueRecv(g_abyRecvBuff,  &recvLen);
	if(iRet != 0) return iRet;

	iRet = 0-MAKEWORD(g_abyRecvBuff[0], g_abyRecvBuff[1]);//return value
    	if(0 == iRet) memcpy(checkValue, g_abyRecvBuff+2,recvLen-2);

	return iRet;	
}

