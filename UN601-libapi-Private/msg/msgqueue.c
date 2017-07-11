#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/msg.h>  
#include <errno.h>  
#include "Msgqueue.h"
#include "user.h"
#include "dll_api.h"

int msgid = -1;  //��Ϣ����id

int MsgQueueInit()  
{  
    //������Ϣ����  
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);  //1234
	printf("msgid is %d\r\n",msgid);
    if(msgid == -1)  
    {  
        fprintf(stderr, "msgget failed with error: %d\n", errno);  
	 return Msgqueue_Open_Err;
    }  
    return 0;
}

int MsgQueueSend(char * sendBuf, int sendLen)
{
     msg_t msgToServer;  
     int i = 0;

    //����Ϣ������д��Ϣ 
    msgToServer.mtype= 1;    
    memcpy(msgToServer.mdata, sendBuf,sendLen);  

    printf("send data:\r\n");
    for( i = 0 ; i < sendLen; i++) printf("%02x ",sendBuf[i]);
    printf("\r\n");
    //����з�������  
    if(msgsnd(msgid, (void*)&msgToServer, sendLen, 0) == -1)  
    {  
        fprintf(stderr, "msgsnd failed\n");  
	 return Msgqueue_Send_Err;
    }  
    return 0;
}

int MsgQueueRecv(char * recvBuf, int *recvLen)
{  
    msg_t msgFromServer ;  
    ssize_t recvBytes ;  
    long int msgtype = 2; 
    int i = 0;
  
    //-----�������Է���������Ϣ  
    for(;;)
    {
	    recvBytes = msgrcv(msgid, &msgFromServer, MAX_SIZE, msgtype, 0);
	    //printf("recvBytes is %d\r\n",recvBytes);
	    if(recvBytes <= 0) continue;
	    
	    memcpy(recvBuf, msgFromServer.mdata, recvBytes) ;  
	    *recvLen = recvBytes;
	    printf("recv data:\r\n");
	    for( i = 0 ; i < recvBytes; i++) printf("%02x ",recvBuf[i]);
	    printf("\r\n");
	      
	     return 0;
    }
}  

int MsgQueueDelete()
{
//ɾ����Ϣ����  
    if(msgctl(msgid, IPC_RMID, 0) == -1)  
    {  
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");  
 	 return Msgqueue_Close_Err;
   }  
     return 0;
}

