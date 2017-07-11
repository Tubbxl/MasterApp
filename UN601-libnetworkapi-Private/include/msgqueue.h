//-------------------ͷ�ļ�msgqueue.h ------------------  
#ifndef _MSGQUEUE_H_  
#define _MSGQUEUE_H_  
#include <sys/ipc.h> //����ftok  
#include <sys/msg.h>  
#include <sys/types.h>  
  
//��Ϣ���еĶ� дģʽ����  
#define MSG_W 0200  
#define MSG_R 0400  
  
//����������֪����Ϣ���м�  
#define MQ_KEY1 128L  
  
#define MAX_SIZE 1024//512  
  
typedef struct   
{  
    long mtype ;  
    char mdata[MAX_SIZE] ;  
} msg_t;

//mdata:wCmd,wLen,data.
typedef struct
{
	ushort wLen;
	ushort wCmd;
	unsigned char data[MAX_SIZE];
		
}mpara;
  
#endif  
