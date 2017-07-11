#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include "dll_api.h"
#include "msgqueue.h"  
#include "user.h"


#ifdef MSG_QUEUE

BYTE recvBuf[MAX_SIZE];
BYTE sendBuf[MAX_SIZE];


void server(int, int) ;  
  
int   main(int argc, char** argv)  
{  
    int  msgqid;  
  
    //创建消息队列  
    msgqid = msgget(MQ_KEY1, IPC_CREAT) ;  
    if (msgqid < 0)  
    {  
        puts("Create msg queue error!\n") ;  
        exit(0) ;  
    }  
  
    server(msgqid, msgqid) ;  
    exit(0) ;  
}  
  
void  server(int readfd, int writefd)  
{  
    FILE*    fp ;  
    pid_t    clientPid ;  
    msg_t* msgPtr ;  
    ssize_t  recvBytes ;  
    char*    pathStr ;      
    int iret = -1;

  
    for ( ; ; )  
    {  
        //从消息队列中读取来自客户的数据  
        msgPtr = malloc(MAX_SIZE + sizeof(long)) ;  
        recvBytes = msgrcv(readfd, msgPtr, MAX_SIZE, 1, 0) ; //阻塞读  
        if (recvBytes <= 0)  
        {  
            puts("pathname missing") ;  
            continue ;  
        }  
        msgPtr->mdata[recvBytes] = '\0' ;  
  
        //分析消息，提取客户端数据 
  	memset(recvBuf,0,sizeof(recvBuf));  
	memcpy(recvBuf,msgPtr->mdata,recvBytes);
	
	switch(recvBuf[0])
	{
		case PCI_KEY_LOAD_MODULE:
		    iret = 0;
		    break;
		case PCI_KEY_USE_MODULE:
		    break;
		case SYSTEM_MODULE:
		    iret = DLL_SysGetVersion(sendBuf);
		    break;
		default:
		    break;
	}

        msgPtr->mtype = 1 ; //msgPtr既作为接收消息 又用作发送消息  
        if ((fp = fopen(pathStr, "r")) == NULL)  
        {  
            //读取文件失败，返回给客户失败信息（在原消息内容后 添加错误信息）  
            snprintf(msgPtr->mdata + recvBytes, sizeof(msgPtr->mdata) -recvBytes,   
                    ": can't open!") ;  
  
            if (msgsnd(writefd, msgPtr, strlen(msgPtr->mdata), IPC_NOWAIT) == -1)  
            {  
                puts("Send Error!");  
                exit(0);  
            }  
        }  
        else  
        {  
            msgsnd(writefd, msgPtr, strlen(msgPtr->mdata), IPC_NOWAIT) ; //非阻塞写  
        }  
    }//for()  
}  
#else
int main()
{
	int iRet;
	unsigned char version[6];
	
	iRet = DLL_SysGetVersion(version);
	if(0==iRet)
	{
		printf("32550 Version:\n");
		printf("Boot:v%d.%d.%d\n",version[0],version[1],version[2]);
		printf("App:v%d.%d.%d\n",version[3],version[4],version[5]);
	}
	else 
		printf("Get 32550 Version Failed.\n");

	return 0;
}
#endif
