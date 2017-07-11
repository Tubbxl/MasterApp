#include <stdio.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "user.h"
#include "Dll_api.h"


int s_scan_fd = -1;
static int ScanKeyMode = 0;
#define SCANNER_PATH	"/dev/scanner"




extern BYTE CurAppNo;

void s_ScanOpenCom()
{
	Lib_UartInit(IMX6UL_COM3,9600);
	Lib_UartReset(IMX6UL_COM3);
}
void s_ScanCloseCom()
{
	Lib_UartClose(IMX6UL_COM3);
}

int s_ScanSend(unsigned char *pbySendData, unsigned short wDataLen, unsigned short wCmd)
{
    int iRet;
    iRet = Lib_UartSend(IMX6UL_COM3,pbySendData,wDataLen);
    if (iRet<0)
    {
        return 101;
    }
    return 0;
}
int ScanRecv(unsigned char *pbyRecvData, unsigned short *pwPacketetLen,unsigned short *pwCmd, unsigned long dwTimeoutMs)
{
    int i,iRet;
    unsigned short recv_len = 0;
    unsigned char tempbuf[1024]={0};	
	i=0;
    while (1)
    {
	iRet = Lib_UartRecv(IMX6UL_COM3,tempbuf[i],&recv_len,dwTimeoutMs,1);
	printf("%d\n",iRet);
	 if(iRet != 0) return 1;
	 if(iRet == 0)
	 {
	 	if(i>2&&i<1024)
	 	{
			if((((tempbuf[i-1])==0x0d))&&(((tempbuf[i])==0x0a))) 
			{
				memcpy(pbyRecvData,tempbuf,i-1);
				break;
			}
	 	}
		i++;
	 }
    }
    return 0;
}
int s_ScanRecv(unsigned char *pbyRecvData, unsigned short *pwPacketetLen,unsigned short *pwCmd, unsigned long dwTimeoutMs)
{
    int i,iRet;
    unsigned short recv_len = 0;
    while (1)
    {
	//iRet = Lib_UartRecv(IMX6UL_COM3,pbyRecvData,&recv_len,dwTimeoutMs,1024);
	iRet = Lib_ScanUartRecv(IMX6UL_COM3,pbyRecvData,&recv_len,dwTimeoutMs,1024);
	 if(iRet != 0) return 1;
	 if(iRet == 0)
	 {
		*pwPacketetLen = recv_len;
		break;
	 }
    }
    return 0;
}
int DLL_ScanOpen()
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

    	if(authCheck(CurAppNo, MODULE_TYPE_SCAN))
    		return App_Permissions_Denial;

	//todo
	//´ò¿ª´®¿Ú
	s_ScanOpenCom();  
  	if ((s_scan_fd = open(SCANNER_PATH, O_RDWR | O_NDELAY)) == -1) 
  	{
        	printf("open scanner error!\n");
            	return Scan_Open_Err;
	}
	iRet =  ioctl(s_scan_fd,CMD_POWER_ON);	
	if(iRet<0)
	{
        	printf("The scanner power supply error!\n");
            	return Scan_PowerOn_Err;
	}
    	printf("The scanner open successful!\n");
	return Scan_Open_Ok;	
}

int DLL_ScanClose()
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

    	if(authCheck(CurAppNo, MODULE_TYPE_SCAN))
    		return App_Permissions_Denial;
	//todo
	if(s_scan_fd<0)
	{
		printf("The scanner need't close!\n");
		return Scan_Not_Open;
	}
	iRet =  ioctl(s_scan_fd,CMD_CTL_STOP);	
	if(iRet<0)
	{
        	printf("The scanner stop error!\n");
            	return Scan_Stop_Err;
	}
	iRet =  ioctl(s_scan_fd,CMD_POWER_OFF);	
	if(iRet<0)
	{
        	printf("The scanner stop error!\n");
            	return Scan_PowerOff_Err;
	}
	close(s_scan_fd);
	s_ScanCloseCom();
	s_scan_fd = -1;
	return Scan_Close_Ok;	
}

int DLL_ScanModeSet(BYTE mode,BYTE *prefix, BYTE *suffix)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;
	unsigned char buf[100]={0};
    	if(authCheck(CurAppNo, MODULE_TYPE_SCAN))
    		return App_Permissions_Denial;	
	//todo
	printf("DLL_ScanModeSet[mode = %d prefix = %s suffix = %s]\n",mode,prefix,suffix);
	if(0x01==(mode&0x01))  //00
	{
		system("echo keywedge > /usr/ScanMode.conf");
		printf("mode keywedge");
	}
	else
	{
		system("echo not > /usr/ScanMode.conf");
		printf("mode notkeywedge");
	}
	if(0x02==(mode&0x02))
	{
		system("echo have_prefix_or_suffix >> /usr/ScanMode.conf");
		if(prefix !=NULL)
		{
			if(strlen(prefix)>4) return -1;
			sprintf(buf,"echo %s >> /usr/ScanMode.conf",prefix);
			system(buf);
		}
		else
		{
			system("echo noprefix >> /usr/ScanMode.conf");
		}
		if(suffix !=NULL)
		{
			if(strlen(prefix)>2) return -1;
			memset(buf,0x00,sizeof(buf));
			sprintf(buf,"echo %s >> /usr/ScanMode.conf",suffix);
			system(buf);	
		}
		else
		{
			system("echo nosuffix >> /usr/ScanMode.conf");
		}
	}
	else
	{
		system("echo no_prefix_or_suffix >> /usr/ScanMode.conf");
		system("echo   noprefix >> /usr/ScanMode.conf");
		system("echo   nosuffix  >> /usr/ScanMode.conf");
		
	}
	if(0x04==(mode&0x04))
	{
		system("echo haveenter >> /usr/ScanMode.conf");
	}
	else
	{
		system("echo noenter >> /usr/ScanMode.conf");
	}
	if(0x08==(mode&0x08))
	{
		;//open beep
	}
	if(0x10==(mode&0x10))
	{
		;//Restore factory default setting
		system("echo                            > /usr/ScanMode.conf");
	}
	system("echo end >> /usr/ScanMode.conf");	
	return 0;	
}

int DLL_ScanTrigger()
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

    	if(authCheck(CurAppNo, MODULE_TYPE_SCAN))
    		return App_Permissions_Denial;
	
	//todo
	if(s_scan_fd<0)
	{
		printf("Please open the scanner!\n");
		return Scan_Not_Open;
	}
	iRet =  ioctl(s_scan_fd,CMD_CTL_START);	
	if(iRet<0)
	{
        	printf("The scanner start error!\n");
            	return Scan_Trigger_Err;
	}

	return Scan_Trigger_Ok;	
}

int ScannerModeDeal(char * indata,char * outdata)
{
	FILE *fp;
	int i=0;
	char buf[212]={0};
	char tempbuf[512]={0};
	char mode=0;
	char fix = 0;
	char enter = 0;
	char prefix[5]={0};
	char suffix[3]={0};
	int len = 0;
	fp=fopen("/usr/ScanMode.conf","r");
	while(fscanf(fp,"%s",buf)!=EOF)
	{
		if(strstr(buf, "keywedge"))
		{
			printf("keywedge mode\n");
			mode =	0xAA; 
		}
		if(strstr(buf, "have_prefix_or_suffix"))
		{
			printf("have_prefix_or_suffix mode\n");
			fix = 0Xbb;
			memset(buf,0x00,sizeof(buf));
			if(fscanf(fp,"%s",buf)!=EOF)
			{
				if(strcmp("noprefix", buf) != 0)
				{
					strcpy(prefix,buf);
					printf("prefix=[%s]\n",prefix);
				}
			}
			memset(buf,0x00,sizeof(buf));
			if(fscanf(fp,"%s",buf)!=EOF)
			{
				if(strcmp("nosuffix", buf) != 0)
				{
					strcpy(suffix,buf);
					printf("suffix=[%s]\n",suffix);
				}
			}
		}
		if(strstr(buf, "haveenter"))
		{
			printf("haveenter mode\n");
			enter =0xcc;
		}
		if(strstr(buf, "end"))
			break;
		memset(buf,0x00,sizeof(buf));
	}
	fclose(fp);
	// deal data
	//prefix and suffix deal
	printf("indata = %s \n",indata);
	if(0Xbb == fix)
	{
		strcat(tempbuf,prefix);
		strcat(tempbuf,indata);
		strcat(tempbuf,suffix);
	}
	else
	{
		strcpy(tempbuf,indata);
	}
	len = strlen(tempbuf);
	printf("tempbuf = %s   len = %d \n",tempbuf,len);
	if( 0xcc == enter)
	{
		len+=1;
		tempbuf[len] = '\n';
	}
	if(0xAA == mode)
	{
		for(i=0;i<len;i++)//keywedge mode   register the data to input system
		{
			printf("%c\n",tempbuf[i]);
			sendKey(tempbuf[i]);
		}
	}
	else
	{
		memcpy(outdata,tempbuf,len);
	}
	return len;
}

int DLL_ScanRead(char *data)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;
	char tempbuf[1024]={0};
	char inbuf[1024]={0};
	unsigned short datalen = 0;
    	if(authCheck(CurAppNo, MODULE_TYPE_SCAN))
    		return App_Permissions_Denial;
	//todo
	if(s_scan_fd<0)
	{
		printf("Please open the scanner!\n");
		return Scan_Not_Open;
	}
    while (1)
    {
        iRet = s_ScanRecv(tempbuf,&wLen,0,8000);
        if (iRet == 0 )
        {
		break;
        }
	if(iRet != 0)
	{
		printf("Scanner read date failed %s\n",iRet); 
		return -1;
	}
    }
    if(wLen>3&&wLen<1024)
    	{
		memcpy(inbuf,tempbuf,wLen-2);
		datalen = ScannerModeDeal(inbuf,data);
		printf("DLL_ScanRead = %s len = %d \n",data,datalen);
	}
	
	iRet =  ioctl(s_scan_fd,CMD_READ_END);	
	iRet =  ioctl(s_scan_fd,CMD_CTL_STOP);
	return datalen;//return Scan_Read_Ok;	
}


// mode =1 open SacnKeyTrigger
// mode =0 close SacnKeyTrigger
int DLL_ScanKeySet(BYTE mode)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

    	if(authCheck(CurAppNo, MODULE_TYPE_SCAN))
    		return App_Permissions_Denial;
	//todo
	if(s_scan_fd<0)
	{
		printf("Please open the scanner!\n");
		return Scan_Not_Open;
	}	
	if(mode == 1)
	{
		iRet =  ioctl(s_scan_fd,CMD_SCAN_KEY_ENABLE);	
	}
	else if(mode == 0)
	{
		iRet =  ioctl(s_scan_fd,CMD_SCAN_KEY_DISABLE);	
	}

	return iRet;	
}


